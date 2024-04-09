/*

Original author: draktuls 
License: GPLv3
Code description:
	This source includes basic buffer overflow handler on the heap

*/

#include "heap_bof.h"

long heap_bof_handler(avud_request * request){
	
	long ret = 0;
	
	if(request->input == NULL){
		pr_err("[AVUD] (heap_bof) heap_bof_handler: input is NULL!");
		ret = -EINVAL;
		goto exit;
	}

	if(request->input_len == 0 || request->input_len > BOF_MAX_BUFFER_SIZE){
		pr_err("[AVUD] (heap_bof) heap_bof_handler: input length is 0 or bigger than %d",BOF_MAX_BUFFER_SIZE);
		ret = -EINVAL;
		goto exit;
	}

	size_t size = request->input_len + sizeof(heap_struct);

	heap_struct * chunk = kzalloc(size, GFP_KERNEL);
	if (!chunk)
    {
        pr_err("[AVUD] (heap_bof) heap_bof_handler: couldn't allocate memory!");
        ret = -ENOMEM;
		goto exit;
    }

	// VULN HERE
	// Here we essentially just assign wrong calculation into the data_size field
	// This causes the next copy_from_user to overflow by this specified amount
	// Let's say this should be a minimum size in the data_size but it's not properly checked
	chunk->data_size = size + HEAP_BOF_SIZE;
	
	if(copy_from_user((void *)chunk->data,(void *)request->input, chunk->data_size)){
		pr_err("[AVUD] (heap_bof) heap_bof_handler: cannot copy input buffer!");
		ret = -EINVAL;
		goto free_exit;
	}

	free_exit:

	kfree(chunk);
	
	exit:
	
	return ret;
}

EXPORT_SYMBOL(heap_bof_handler);
