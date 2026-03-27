#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEVICE_NAME "simple_char"
#define BUFFER_SIZE 256

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver for QEMU vexpress-a9");
MODULE_VERSION("1.0");

static int major_number;
static struct class *simple_class = NULL;
static struct device *simple_device = NULL;
static char device_buffer[BUFFER_SIZE];
static int buffer_size = 0;

// Function prototypes
static int dev_open(struct inode *inodep, struct file *filep);
static int dev_release(struct inode *inodep, struct file *filep);
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset);
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset);

// File operations structure
static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

/* 
 * Called when the device is opened by a userspace process.
 */
static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "simple_char: Device opened\n");
    return 0;
}

/* 
 * Called when data is read from the device.
 * Copies data from kernel space to userspace.
 */
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    int bytes_to_read;
    int bytes_read;

    // Calculate how much data is left to read
    bytes_to_read = min((int)(buffer_size - *offset), (int)len);

    if (bytes_to_read <= 0) {
        return 0; // End of file
    }

    // Copy data to userspace
    bytes_read = copy_to_user(buffer, device_buffer + *offset, bytes_to_read);
    
    if (bytes_read != 0) {
        printk(KERN_ERR "simple_char: Failed to copy %d bytes to user\n", bytes_read);
        return -EFAULT;
    }

    *offset += bytes_to_read;
    printk(KERN_INFO "simple_char: Read %d bytes from device\n", bytes_to_read);
    return bytes_to_read;
}

/* 
 * Called when data is written to the device.
 * Copies data from userspace to kernel space.
 */
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
    int bytes_to_write;

    // Limit write size to buffer capacity
    bytes_to_write = min((int)len, BUFFER_SIZE - 1);

    // Copy data from userspace
    if (copy_from_user(device_buffer, buffer, bytes_to_write)) {
        printk(KERN_ERR "simple_char: Failed to copy %d bytes from user\n", bytes_to_write);
        return -EFAULT;
    }

    device_buffer[bytes_to_write] = '\0'; // Null terminate
    buffer_size = bytes_to_write;

    printk(KERN_INFO "simple_char: Received %d bytes: '%s'\n", bytes_to_write, device_buffer);
    return bytes_to_write;
}

/* 
 * Called when the device is closed.
 */
static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "simple_char: Device closed\n");
    return 0;
}

/* 
 * Module Initialization
 */
static int __init simple_init(void)
{
    printk(KERN_INFO "simple_char: Initializing the character device\n");

    // Allocate a major number dynamically
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_char: Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "simple_char: Registered correctly with major number %d\n", major_number);

    // Create a device class
    simple_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(simple_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(simple_class);
    }
    printk(KERN_INFO "simple_char: Device class registered correctly\n");

    // Create the device node (/dev/simple_char)
    simple_device = device_create(simple_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(simple_device)) {
        class_destroy(simple_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(simple_device);
    }
    printk(KERN_INFO "simple_char: Device created successfully\n");

    return 0;
}

/* 
 * Module Cleanup
 */
static void __exit simple_exit(void)
{
    device_destroy(simple_class, MKDEV(major_number, 0));
    class_unregister(simple_class);
    class_destroy(simple_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_char: Goodbye!\n");
}

module_init(simple_init);
module_exit(simple_exit);
