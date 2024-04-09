/*

Original author: draktuls 
License: GPLv3
Code description:
	This source contains basic ioctl handler and needed structures and defines for correct module compile.

*/

#include "avud_main.h"

// Necessary structures

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .unlocked_ioctl = avud_ioctl
};

static struct miscdevice avud_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &fops
	};
	

// AVUD uses ioctl to decide which vuln to trigger
long avud_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	
	avud_request request;
	memset(&request, 0, sizeof(avud_request));
	
	// Copy the avud struct from the user
	if (copy_from_user((void *)&request, (void *)arg, sizeof(avud_request)))
    {
        pr_err("[AVUD] (ioctl) avud_ioctl: Couldn't copy request!");
        return -EINVAL;
    }
	
	switch(cmd){
		
		// Stack
		case IOCTL_BOF_STACK:
			ret = stack_bof_handler(&request);
			break;
		case IOCTL_OOBR_STACK:
			ret = stack_oobr_handler(&request);
			break;
		case IOCTL_BUF_STACK:
			ret = stack_buf_handler(&request);
			break;	
		case IOCTL_INT_OVERFLOW_STACK:
			ret = stack_int_overflow_handler(&request);
			break;
		
		// Heap	
		case IOCTL_BOF_HEAP:
			ret = heap_bof_handler(&request);
			break;
		case IOCTL_OOBR_HEAP:
			ret = heap_oobr_handler(&request);
			break;
		case IOCTL_BUF_HEAP:
			ret = heap_buf_handler(&request);
			break;
		
		// Heap note
		case IOCTL_NOTE_ADD:
			ret = heap_note_handler(CTL_ADD, &request);
			break;
		case IOCTL_NOTE_EDIT:
			ret = heap_note_handler(CTL_EDIT, &request);
			break;
		case IOCTL_NOTE_FREE:
			ret = heap_note_handler(CTL_FREE, &request);
			break;
		case IOCTL_NOTE_VIEW:
			ret = heap_note_handler(CTL_VIEW, &request);
			break;
		case IOCTL_NOTE_EDIT_TAG:
			ret = heap_note_handler(CTL_EDIT_TAG, &request);
			break;
		case IOCTL_NOTE_VIEW_TAG:
			ret = heap_note_handler(CTL_VIEW_TAG, &request);
			break;
		case IOCTL_NOTE_SWITCH_SAFE:
			ret = heap_note_handler(CTL_SWITCH_SAFE, &request);
			break;

		default:
			ret = -ENOIOCTLCMD;
			pr_err("[AVUD] (ioctl) avud_ioctl: Couldn't find the requested cmd!");
			break;
	}
	
	return ret;
}

// Init

int __init avud_driver_init(void)
{
	int error;

    error = misc_register(&avud_misc);
    if (error != 0) {
        pr_err("[AVUD] (init) avud_driver_init: misc_register failed!");
        return error;
    }
    
    /*for(int i = 0; i < sizeof(heap_notes); i++){
		heap_notes[i] = 0;
	}*/
 
    pr_info("[AVUD] (init) avud_driver_init: device init done!");
    return 0;
    
}

// Exit

void __exit avud_driver_exit(void)
{
	misc_deregister(&avud_misc);
    pr_info("[AVUD] (exit) avud_driver_exit: misc_deregister done!");
}
 
module_init(avud_driver_init);
module_exit(avud_driver_exit);

// Module info
MODULE_AUTHOR("draktuls");
MODULE_DESCRIPTION("Another Very Unsafe Driver");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
