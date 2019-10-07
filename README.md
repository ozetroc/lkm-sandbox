# lkm-sandbox

Loadable kernel modules sandbox used to learn about LKMs.

## Prerequisities
Install kernel headers for your kernel version if they are missing in ```/usr/src/```
```bash
sudo apt-get install linux-headers-$(uname -r)
```

If gcc is not installed
```bash
sudo apt install gcc
```

If make is not installed
```bash
sudo apt install make
```


## Usage
```cd``` to each module directory and run ```make```

To clean output files use ```make clean```

To load module into the kernel use ```sudo make load```

To unload module from the kernel use ```sudo make unload```

Good method to observe kernel log in real time is following:
```bash
dmesg -wH
```

## List of modules
* 00_basic_module
   * initialization of basic module
   * usage of printk function
* 01_basic_tasklet
* 02_basic_sleep
* 03_basic_timers
   * improved kernel timer api [8]
* 04_basic_char
* 05_char_ioctl
* 06_char_sysfs
* 07_timers_conf
   * improved kernel timer api [8]


## Sources
[1] [Linux Device Drivers, Third Edition, O’Reilly](https://www.oreilly.com/openbook/linuxdrive3/book/)

[2] [Linux Device Drivers Development, John Madieu](https://subscription.packtpub.com/book/networking_and_servers/9781785280009)

[3] Writing a Linux Kernel Module by Derek Molloy: [Part 1](http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/#prettyPhoto) and [Part 2](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)

[4] [Linux Driver Tutorial: How to Write a Simple Linux Device Driver](https://www.apriorit.com/dev-blog/195-simple-driver-for-linux-os)

[5] [The Linux Kernel Module Programming Guide by Peter Jay Salzman and Ori Pomeranz](http://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN765)

[6] [Device Drivers, Part 4: Linux Character Drivers, Open Source For You](https://opensourceforu.com/2011/02/linux-character-drivers/)

[7] [LINUX KERNEL DEVELOPMENT – CREATING A PROC FILE AND INTERFACING WITH USER SPACE](https://devarea.com/linux-kernel-development-creating-a-proc-file-and-interfacing-with-user-space/)

[8] [Improving the kernel timers API](https://lwn.net/Articles/735887/)

## Tips
* Add linux kernel include folders to Visual Studio Code. Paste following into c_cpp_properties.json file in section includePath. Adjust your kernel version.
```json
"${workspaceFolder}/**",
"/usr/src/linux-headers-5.0.0-29-generic/*",
"/usr/src/linux-headers-5.0.0-29-generic/include/*",
"/usr/src/linux-headers-5.0.0-29-generic/include/linux/*",
"/usr/src/linux-headers-5.0.0-29-generic/include/asm-generic/*"
```

