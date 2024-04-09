#pragma once

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <linux/kernel.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>

// AVUD sources
#define EXAMPLE_DEF
#include "../defines.h"

extern int avud_fd;

extern int avud_ioctl(avud_request * req, unsigned int cmd);
extern int BOF_request(avud_request * req);