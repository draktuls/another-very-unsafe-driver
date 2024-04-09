# Objects for comp
obj-m := avud.o
STACK_VULNS := vulns/stack_bof.o vulns/stack_buf.o vulns/stack_oobr.o vulns/stack_int_overflow.o
HEAP_VULNS := vulns/heap_bof.o vulns/heap_buf.o vulns/heap_oobr.o vulns/heap_note.o 
avud-objs := avud_main.o $(STACK_VULNS) $(HEAP_VULNS)

PWD := $(shell pwd)

# Setup for kernel directory
# Building the module for specific version means
# we gotta change it here aswell

# Default points into current kernel
#KDIR := /lib/modules/$(shell uname -r)/build
KDIR := /home/kali/KernelBuilds/obj/linux-easy

# Where should the driver, example and exploits reside
RAMFS_LOCATION := /home/kali/KernelBuilds/initramfs/busybox

# Example name
EXAMPLE_NAME := main

.PHONY: all

clean:
	$(info --------------- --------------- CLEAN --------------- ---------------)
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	cd examples && $(MAKE) clean

initramfs:
	$(info --------------- --------------- INITRAMFS --------------- ---------------)
	cp init $(RAMFS_LOCATION)/init
	cd $(RAMFS_LOCATION) && find .| cpio -oHnewc | gzip > $(PWD)/images/initramfs.cpio.gz

example:
	$(info --------------- --------------- EXAMPLES --------------- ---------------)
	cd examples && $(MAKE) clean && $(MAKE) all name=$(EXAMPLE_NAME) && cp $(EXAMPLE_NAME) $(RAMFS_LOCATION)/$(EXAMPLE_NAME)

driver: 
	$(info --------------- --------------- DRIVER --------------- ---------------)
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	cp $(obj-m) $(RAMFS_LOCATION)/$(obj-m)

# We pass ramfs location so the copy part is done inside the exploit's makefile
exploits:
	$(info --------------- --------------- EXPLOIT --------------- ---------------)
	$(MAKE) -C exploit/ RAMFS_LOCATION=$(RAMFS_LOCATION) all

all_compile:
	$(info --------------- --------------- COMPILING ALL --------------- ---------------)
	$(MAKE) driver
	$(MAKE) example
	$(MAKE) exploits
	$(MAKE) initramfs

all_clean: 
	$(MAKE) all_compile
	$(MAKE) clean

all: 
	$(MAKE) all_compile
