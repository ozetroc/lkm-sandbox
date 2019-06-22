# lkm-sandbox

Loadable kernel modules sandbox used to learn about LKMs.

## Prerequisities


## Usage
Good way to observe kernel log is:
```bash
dmesg -wH
```

## List of modules
* 0_basic_module
   * initialization of basic module
   * usage of printk function

## Sources
[1] Writing a Linux Kernel Module by Derek Molloy: [Part 1](http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/#prettyPhoto) and [Part 2](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)

[2] [Linux Driver Tutorial: How to Write a Simple Linux Device Driver](https://www.apriorit.com/dev-blog/195-simple-driver-for-linux-os)

[3] [The Linux Kernel Module Programming Guide by Peter Jay Salzman and Ori Pomeranz](http://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN765)

## Tips
Add linux kernel include folders to Visual Studio Code
