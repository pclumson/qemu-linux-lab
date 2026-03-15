# qemu-linux-lab
**QEMU Hardware Emulation Lab** | Personal Project | 2026
- Configured ARM Linux environment using QEMU vexpress-a9 and Buildroot
- - Implemented custom character device driver in C with kernel module support
- - Demonstrated understanding of hardware abstraction layers and cross-compilation toolchains 



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
