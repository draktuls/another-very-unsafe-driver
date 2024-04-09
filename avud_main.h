/*

Original author: draktuls 
License: GPLv3
Code description:
	This source defines basic ioctl handler function and includes needed vulnerability functions.

*/

#pragma once

#ifndef __AVUD_H__
#define __AVUD_H__

#include "vulns/stack_bof.h"
#include "vulns/stack_buf.h"
#include "vulns/stack_oobr.h"
#include "vulns/stack_int_overflow.h"

#include "vulns/heap_bof.h"
#include "vulns/heap_buf.h"
#include "vulns/heap_oobr.h"

#include "vulns/heap_note.h"

#include "defines.h"

extern int  __init avud_driver_init(void);
extern void __exit avud_driver_exit(void);
extern long avud_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif
