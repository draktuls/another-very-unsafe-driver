/*

Original author: draktuls 
License: GPLv3
Code description:
	This source defines important structures, values for certain functions and IOCTL numbers.
    Also includes all needed library modules.

*/

#pragma once

#ifndef __AVUD_DEFINES_H__
#define __AVUD_DEFINES_H__

#ifndef EXAMPLE_DEF
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>              
#include <linux/ioctl.h>
#include <linux/err.h>
#include <linux/miscdevice.h>

#endif

// Defines
#define DEVICE_NAME "avud"
#define CLASS_NAME  "avud"
#define HEAP_NOTE_ARRAY_SIZE 32
#define HEAP_CHUNK_TAG_SIZE 20
#define HEAP_BOF_SIZE 32
#define HEAP_BUF_SIZE 248
#define HEAP_OOBR_STRUCT_SIZE 1016

// Structures

typedef struct {	
    void * input;
    void * output;
    
    size_t input_len;
    size_t output_len;
    
    int input_offset;
    int output_offset;

    char * tag;
    size_t input_index;
} avud_request;

typedef struct{
    size_t data_size;
    unsigned char data[HEAP_BOF_SIZE];
} heap_struct;

typedef struct{
    size_t data_size;
    unsigned char data[HEAP_BUF_SIZE];
} heap_struct_buf;

typedef struct{
    unsigned char data[HEAP_OOBR_STRUCT_SIZE];
    size_t data_size;
} heap_struct_oobr;

typedef struct{
    void (* release)(void * chunk);
    long (* view)(void * data, size_t data_size, void * input);
    long (* edit)(void * data, size_t data_size, void * output);
} chunk_operations;

typedef struct{
    int type;
    unsigned char tag[HEAP_CHUNK_TAG_SIZE];
    unsigned char * data;
    size_t data_size;
    chunk_operations op;

} important_heap_struct;

typedef struct{
    int type;
    unsigned char * data;
    size_t data_size;
    unsigned char tag[HEAP_CHUNK_TAG_SIZE];
    chunk_operations op;

} important_heap_struct_safe;

// IOCTLs
#define IOCTL_MAGIC '\x69'
#define IOCTL_BOF_STACK _IOWR(IOCTL_MAGIC,0, avud_request )
#define IOCTL_OOBR_STACK _IOWR(IOCTL_MAGIC,1, avud_request )
#define IOCTL_BUF_STACK _IOWR(IOCTL_MAGIC,2, avud_request )
#define IOCTL_INT_OVERFLOW_STACK _IOWR(IOCTL_MAGIC,3, avud_request )

#define IOCTL_BOF_HEAP _IOWR(IOCTL_MAGIC,30, avud_request )
#define IOCTL_OOBR_HEAP _IOWR(IOCTL_MAGIC,31, avud_request )
#define IOCTL_BUF_HEAP _IOWR(IOCTL_MAGIC,32, avud_request )

#define IOCTL_NOTE_ADD _IOWR(IOCTL_MAGIC,60, avud_request )
#define IOCTL_NOTE_EDIT _IOWR(IOCTL_MAGIC,61, avud_request )
#define IOCTL_NOTE_FREE _IOWR(IOCTL_MAGIC,62, avud_request )
#define IOCTL_NOTE_VIEW _IOWR(IOCTL_MAGIC,63, avud_request )
#define IOCTL_NOTE_EDIT_TAG _IOWR(IOCTL_MAGIC,64, avud_request )
#define IOCTL_NOTE_VIEW_TAG _IOWR(IOCTL_MAGIC,65, avud_request )
#define IOCTL_NOTE_SWITCH_SAFE _IOWR(IOCTL_MAGIC,66, avud_request )

#endif