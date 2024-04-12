<h1 align="center">Another Very Unsafe Driver</h1>

# About <a name = "about"></a>
The point of this project is to provide a learning opportunity for exploit development inside kernel space. Depending on the corruption type you will face different exploitation problems. 

This isnt a novel idea and there are other great projects such as [HEVD](https://github.com/hacksysteam/HackSysExtremeVulnerableDriver/tree/master). But this focuses on Linux Kernel only.

This is just a small school project and utility in case I want to research stuff in an easy isolated environment without any other limitations.

# Getting Started <a name = "getting_started"></a>

## Prerequisites
In order to correctly install **AVUD** you first need to build your own kernel. There are many tutorials on how to do such a thing. 

I included 2 configs for easy and hardened version.
Easy version doesn't have many compile time protections enabled. Hardened version has everything ready and clang's CFI enabled.

Then you just have to modify this line inside the root **Makefile**.
```
KDIR := /home/kali/KernelBuilds/obj/linux-easy
```

And also populate `./images` folder. I suggest copying both compressed and uncompressed versions. These versions should be suffixed by unique name like these:
`bzImageHardened`. 

*This is then later used during running the image.*

Unless you want your system to die everytime your exploit fails you should utilize QEMU for debugging purposes.
This comes with another prerequisite you must do which is making custom `ramfs` (with busybox or similar). Then change the makefile:
```
RAMFS_LOCATION := /home/kali/KernelBuilds/initramfs/busybox
```

Or create your own entire setup nothing is stopping you.

## Compilation
The compilation is straight forward. You can issue:
```bash
make all
```

This will do 4 operations:
- Compile AVUD module and copy it into ramfs location
- Compile examples folder
- Compile available exploits folder
- Copy init into ramfs location and compress it into the cpio format

## Running the image
To run the compiled kernel image you can use the `run.sh` script. This accepts two arguments:

```bash
./run.sh Hardened 1
```
First argument is the suffix which should the image contain.

Second argument is either 1 or 0. This specifies a debug version which turns on or off KASLR.

### Protections
By default all images will be run with `smap`, `smep` and `kpti`. You can tune this in the `run.sh` script.

# Debug <a name="usage"></a>
The setup is entirely on you but in the `vanilla` exploit you can see an example setup in `.gdbinit`:
```
add-symbol-file ../../avud.ko 0xffffffffc0000000
target remote 127.0.0.1:1234
br *0xffffffffc00002a9
br stack_oobr_handler
br stack_bof_handler
c
```

Example setup used [pwndbg](https://github.com/pwndbg/pwndbg) framework and it assumes you will include debug symbols during kernel building.

# Vulnerabilities
The module contains these vulnerabilities:
- Buffer overflow and underflow (heap and stack)
- Integer overflow leading to bof on stack
- Out of bound reads (heap and stack)
- Race conditions (Not intentionally developed, there are just no locks)
- Type confusion (heap note)
- UAF (heap note)
- Double free (heap note)

Each vulnerable section should have a comment above it describing it. But it is possible that some are missing, which can be fixed.

# Deployment <a name = "deployment"></a>
I beg you, do not deploy this on any real system.

# Bugs
I haven't tested all the vulnerabilities and it is possible that they may not work as intended (heap note in particular - there is a lot of options). This could be flaw in my design or that some compile-time check is added to the final build.

In case this happens let me know in the issues and I'll make sure it works.
