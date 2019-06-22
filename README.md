# lkm-sandbox

Loadable kernel modules sandbox used to learn about LKMs.

## Prerequisities
Install kernel headers for your kernel version if they are missing in ```/usr/src/```
```bash
sudo apt-get install linux-headers-$(uname -r)
```


## Usage
```cd``` to each module directory and run ```make```

Good way to observe kernel log is:
```bash
dmesg -wH
```

## List of modules
* 0_basic_module
   * initialization of basic module
   * usage of printk function

## Sources
[1] [Linux Device Drivers, Third Edition, O’Reilly](https://www.oreilly.com/openbook/linuxdrive3/book/)

[2] [Linux Device Drivers Development, John Madieu](https://subscription.packtpub.com/book/networking_and_servers/9781785280009)

[3] Writing a Linux Kernel Module by Derek Molloy: [Part 1](http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/#prettyPhoto) and [Part 2](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)

[4] [Linux Driver Tutorial: How to Write a Simple Linux Device Driver](https://www.apriorit.com/dev-blog/195-simple-driver-for-linux-os)

[5] [The Linux Kernel Module Programming Guide by Peter Jay Salzman and Ori Pomeranz](http://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN765)

[6] [Device Drivers, Part 4: Linux Character Drivers, Open Source For You](https://opensourceforu.com/2011/02/linux-character-drivers/)

[7] [LINUX KERNEL DEVELOPMENT – CREATING A PROC FILE AND INTERFACING WITH USER SPACE](https://devarea.com/linux-kernel-development-creating-a-proc-file-and-interfacing-with-user-space/)

## Tips
* Add linux kernel include folders to Visual Studio Code. Paste following into c_cpp_properties.json file in section includePath. Adjust your kernel version.
```json
"/usr/src/linux-headers-4.15.0-52-generic/include/*",
"/usr/src/linux-headers-4.15.0-52-generic/include/linux/*"
```

