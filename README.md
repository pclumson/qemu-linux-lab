# qemu-linux-lab
**QEMU Hardware Emulation Lab** | Personal Project | 2026
- Configured ARM Linux environment using QEMU vexpress-a9 and Buildroot
- - Implemented custom character device driver in C with kernel module support
- - Demonstrated understanding of hardware abstraction layers and cross-compilation toolchains 

# Folder Structure



# QEMU Hardware Emulation Lab

A hands-on project demonstrating hardware emulation concepts using QEMU, Buildroot, and custom Linux kernel modules. This lab was completed as part of systems programming skill development for embedded and avionics software roles.

---

## 🎯 Project Overview

This project sets up a minimal ARM Linux environment using QEMU vexpress-a9 emulation and Buildroot, then implements a simple character device driver to demonstrate understanding of hardware abstraction layers (HAL), cross-compilation toolchains, and kernel module development.

**Key Learning Outcomes:**
- QEMU hardware emulation configuration and boot process
- Buildroot customization for embedded Linux systems
- ARM cross-compilation toolchain usage
- Linux kernel module development in C
- Hardware abstraction layer concepts

---

## 📋 Prerequisites

### Host System Requirements
- Linux (Ubuntu/Debian recommended) or macOS
- 4GB+ RAM (8GB recommended for faster builds)
- 10GB+ free disk space

### Required Packages
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential libncurses-dev bison flex libssl-dev libffi-dev \
                 python3 python3-pip qemu-system-arm gcc-arm-linux-gnueabihf

# macOS (via Homebrew)
brew install qemu buildroot

```



🏗️ Build Instructions
Step 1: Clone Buildroot

git clone https://github.com/buildroot/buildroot.git
cd buildroot

Step 2: Configure Target Board

make qemu_vexpress_a9_defconfig
make menuconfig

Key Configuration Options:

    Target Architecture: ARM (Cortex-A9)
    Kernel: Linux 5.x or 6.x
    Root Filesystem: ext4
    Enable: CONFIG_MODULES=y, CONFIG_MODULE_UNLOAD=y

Step 3: Build System

make -j$(nproc)

Expected Output:

    output/images/zImage (Linux kernel)
    output/images/rootfs.ext4 (root filesystem)
    output/images/qemu_arm_vexpress.sh (launch script)

Step 4: Launch QEMU

./output/images/qemu_arm_vexpress.sh

Expected Boot Sequence:

[    0.000000] Booting Linux on physical CPU 0x0
[    0.000000] Linux version 5.x.x
[    0.000000] CPU: ARMv7 Processor [...]
[    1.500000] Starting init: /sbin/init exists
[    2.000000] Welcome to Buildroot
buildroot login:

🔧 Custom Character Device Driver
Source Code Location

drivers/char/simple_char_driver.c

Compilation

# Cross-compile for ARM
arm-linux-gnueabihf-gcc -D__KERNEL__ -DMODULE \
  -I./output/build/linux-*/include \
  -c simple_char_driver.c

# Copy module to QEMU VM
cp simple_char_driver.ko /tmp/

Loading & Testing

# Inside QEMU VM
insmod simple_char_driver.ko
dmesg | tail  # Should show: "Simple device registered with major number X"

# Create device node (if udev not running)
mknod /dev/simple_device c <major_number> 0
chmod 666 /dev/simple_device

# Test read/write
echo "Hello from QEMU" > /dev/simple_device
cat /dev/simple_device  # Should output: "Hello from QEMU"

📊 Technical Specifications
Component	Specification
Emulator	QEMU v7.2+
Target Board	ARM vexpress-a9
CPU	Cortex-A9 (ARMv7)
Kernel	Linux 5.x/6.x
RootFS	Buildroot-generated ext4
RAM Allocated	512MB - 1GB
Cross-Compiler	arm-linux-gnueabihf-gcc

📚 Lessons Learned
1. Emulation Performance Trade-offs

    QEMU emulation is significantly slower than native execution (typically 10-100x depending on workload)
    Timing-sensitive code may behave differently in emulation vs. real hardware
    Estimation Impact: When porting real-time avionics code, allocate additional time for performance validation and optimization

2. Buildroot Configuration Complexity

    Initial configuration requires understanding of target hardware requirements
    Kernel module support must be explicitly enabled
    Toolchain selection affects cross-compilation compatibility

3. Hardware Abstraction Layer Understanding

    Device drivers must handle hardware-specific registers and interrupts
    HAL provides consistent interface across different hardware platforms
    Proper error handling is critical for flight-critical applications

4. Common Challenges Encountered
Issue	Solution
Kernel module version mismatch	Match kernel headers from Buildroot build
Device node not created	Manually create with mknod if udev unavailable
Slow boot times	Increase RAM allocation (-m 1024)
Serial console not showing	Add console=ttyAMA0 to kernel command line
🔗 Related Resources

    QEMU Official Documentation
    Buildroot User Manual
    Linux Kernel Module Programming Guide
    ARM Architecture Reference

📝 Project Status
Milestone       	Status	Date
Buildroot Setup 	✅     Complete	March 2026
QEMU Boot	        ✅     Complete	March 2026
Device Driver	    ✅     Complete	March 2026
Documentation	    ✅     Complete	March 2026

📄 License
This project is for educational purposes.
All code is original work.


👤 Author
Prince Clumson-Eklu
Senior Full Stack Software Engineer
Wichita, KS
github.com/pclumson
🎓 Relevance to Software Estimator & Architect Role

This project demonstrates foundational knowledge relevant to the Software Estimator and Architect position:

Requirement	Project Evidence
C/C++ Systems Programming	Kernel module written in C with proper memory management
Hardware Emulation (QEMU)	Direct hands-on with QEMU ARM emulation
Buildroot Knowledge	Custom Buildroot configuration and cross-compilation
Hardware Abstraction	Device driver demonstrates HAL understanding
Technical Documentation	Comprehensive README with setup instructions
Estimation Awareness	Lessons learned section addresses performance trade-offs
📞 Contact

Questions about this project? Reach out at: 
