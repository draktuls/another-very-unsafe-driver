#!/bin/sh

if [ $# != 2 ]; then
	echo "./run.sh <bzimage_suffix> <Debug>"
	echo "(Run this script from repo!)"
	echo "EXAMPLES:"
	echo "	./run.sh Hardened 1"
	echo "	./run.sh Normal 0"
	exit 0
fi

if [ "$2" -eq '1' ]; then
	echo "Chosen debug (NO KASLR)"
	KASLR="nokaslr"
else
	echo "Chosen non debug (KASLR)"
	KASLR="kaslr"
fi

if [ ! -e "./images/bzImage$1" ]; then
	echo "The bzImage doesn't exist!"
	exit 1
fi

echo "bzImage appears to be correct!"

qemu-system-x86_64 \
    -append "console=ttyS0 quiet loglevel=3 oops=panic panic=-1 pti=on $KASLR" \
    -initrd "./images/initramfs.cpio.gz" \
	-kernel "./images/bzImage$1" \
    -m 128M \
    -monitor /dev/null \
    -nographic \
    -cpu qemu64,+smep,+smap,+rdrand \
    -smp cores=2 \
    -s \

