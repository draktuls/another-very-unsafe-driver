/*

Original author: draktuls 
License: GPLv3
Code description:
	This source implements basic stack buffer overflow handler

*/

#include "stack_bof.h"

/*

The vanilla copy_from_user() BOF style is essentially gone for good
as there are compiler checks for copy calls to statically defined buffers with size
Which essentially puts artifical check before the copy call and if this one fails
then we trigger __copy_overflow and essentially do nothing and report the bug
And when trying to cheat our way through this we get compile time error
__bad_copy_to (This is a part of kernel source files)

*/

// Cant have stack cookies
__attribute__((no_stack_protector))
long stack_bof_handler(avud_request * request){
	
	long ret = 0;

	size_t size = request->input_len;
	void * input = request->input;

	if(request->input == NULL){
		pr_err("[AVUD] (stack_bof) stack_bof_handler: input is NULL!");
		ret = -EINVAL;
		goto exit;
	}

	if(size == 0 || size >= 4096){
		pr_err("[AVUD] (stack_bof) stack_bof_handler: input_len is 0 or is bigger than 4096!");
		ret = -EINVAL;
		goto exit;
	}

	unsigned char * temp = kzalloc(size, GFP_KERNEL);
	if (!temp)
    {
        pr_err("[AVUD] (stack_bof) stack_bof_handler: couldn't allocate temp memory!");
        ret = -ENOMEM;
		goto exit;
    }
	
	unsigned char data[STACK_OVERFLOW_SIZE] = {0};

	if(copy_from_user((void *)temp,(void *)input, size)){
		pr_err("[AVUD] (stack_bof) stack_bof_handler: cannot copy input buffer!");
		ret = -EINVAL;
	}

	// VULN HERE
	// we are passing raw input length into our STACK_OVERFLOW_SIZE sized buffer
	memcpy(data,temp,size);

	// This is a decoy message, otherwise the whole data buffer would be optimized out
	pr_info("[AVUD] (stack_bof) stack_bof_handler: data buffer is at %p",&data);
	
	exit:
	
	return ret;
}

EXPORT_SYMBOL(stack_bof_handler);
