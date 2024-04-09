/*

Original author: draktuls 
License: GPLv3
Code description:
	This header is used by oobr handler

*/

#pragma once

#ifndef __HEAP_OOBR_H__
#define __HEAP_OOBR_H__

#include "../defines.h"

#define OOBR_MAX_READ 8192

extern long heap_oobr_handler(avud_request * request);

#endif