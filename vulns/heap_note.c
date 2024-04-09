/*

Original author: draktuls 
License: GPLv3
Code description:
	This source implements heap note system. 
	Which includes many different vulnerabilities.

*/

#include "heap_note.h"

void * heap_notes[HEAP_NOTE_ARRAY_SIZE];

static void data_release(void * ptr);
static long data_edit(void * data, size_t data_size, void * input);
static long data_view(void * data, size_t data_size, void * output);
static long heap_note_add(size_t index, size_t size, void * input, void * input_tag);
static long heap_note_edit(size_t index, void * input);
static long heap_note_free(size_t index);
static long heap_note_view(size_t index, void * output);
static long heap_note_change_tag(size_t index, void * new_tag);
static long heap_note_view_tag(size_t index, void * output);

static void data_release(void * ptr){

	// VULN HERE
	// Not directly a vulnerability, but since we can change our structure types
	// the code should have some form of check and branch into different possibilities
	// This way we are still reusing the original struct

	important_heap_struct * chunk = ptr;
	
	if(chunk->data != NULL){
		kfree(chunk->data);
		chunk->data = 0;
	}

}

static long data_edit(void * data, size_t data_size, void * input){
	long ret = 0;

	if(copy_from_user((void *)data,(void *)input, data_size)){
		pr_err("[AVUD] (heap_note) data_edit: cannot edit data!");
		ret = -EINVAL;
	}

	return ret;
}

static long data_view(void * data, size_t data_size, void * output){
	long ret = 0;

	if(copy_to_user((void *)output,(void *)data, data_size)){
		pr_err("[AVUD] (heap_note) data_view: cannot edit data!");
		ret = -EINVAL;
	}

	return ret;
}

long heap_note_add(size_t index, size_t size, void * input, void * input_tag){
	long ret = 0;

	important_heap_struct * temp = kzalloc(sizeof(important_heap_struct), GFP_KERNEL);
	if (!temp)
    {
        pr_err("[AVUD] (heap_note) heap_note_add: couldn't allocate memory!\n");
        ret = -ENOMEM;
		goto exit;
    }

	heap_notes[index] = temp;

	uint32_t tag_size = strnlen(input_tag, HEAP_CHUNK_TAG_SIZE);
	if(copy_from_user((void *)temp->tag,(void *)input_tag, tag_size)){
		pr_err("[AVUD] (heap_note) heap_note_add: cannot copy the tag!");
		ret = -EINVAL;
		goto free_main_exit;
	}

	temp->op.edit = data_edit;
	temp->op.release = data_release;
	temp->op.view = data_view;

	unsigned char * data_temp = kzalloc(size, GFP_KERNEL);
	if (!temp)
    {
        pr_err("[AVUD] (heap_note) heap_note_add: couldn't allocate data memory!\n");
        ret = -ENOMEM;
		goto free_main_exit;
    }

	temp->data = data_temp;
	temp->data_size = size;

	if(copy_from_user((void *)data_temp,(void *)input, size)){
		pr_err("[AVUD] (heap_note) heap_note_add: cannot copy input buffer!");
		ret = -EINVAL;
		goto free_all_exit;
	}

	temp->type = UNSAFE;

	goto exit;
	
	free_all_exit:

	kfree(data_temp);

	free_main_exit:

	// Free chunk
	kfree(temp);

	exit:

	return ret;
}

static long heap_note_edit(size_t index, void * input){

	// VULN HERE
	// Same as above we are not checking the structure type
	// this can lead to all sorts of primitives

	long ret = 0;

	if(heap_notes[index] == NULL){
		ret = -EINVAL;
		goto exit;
	}

	important_heap_struct * cur = heap_notes[index];
	void * data = cur->data;
	size_t data_size = cur->data_size;

	ret = cur->op.edit(data, data_size, input);

	exit:
	return ret;
}

inline long do_free_safe(important_heap_struct_safe * cur){
	long ret = 0;

	cur->op.release(cur->data);

	return ret;
}

inline long do_free_unsafe(important_heap_struct * cur){
	long ret = 0;

	cur->op.release(cur->data);

	return ret;
}

static long heap_note_free(size_t index){
	long ret = 0;

	if(heap_notes[index] == NULL){
		ret = -EINVAL;
		goto exit;
	}

	important_heap_struct * cur = heap_notes[index];

	if(cur->type == UNSAFE){
		ret = do_free_unsafe(cur);
	}else{
		ret = do_free_safe((important_heap_struct_safe *) cur);
	}

	kfree(heap_notes[index]);

	// VULN HERE
	// The data is not being zeroed out and this will result
	// into either UAF or double free scenarios

	exit:

	return ret;
}
static long heap_note_view(size_t index, void * output){
	long ret = 0;

	important_heap_struct * cur = heap_notes[index];
	void * data = cur->data;
	size_t data_size = cur->data_size;

	ret = cur->op.view(data, data_size, output);

	return ret;
}

inline long do_change_tag_safe(important_heap_struct_safe * cur, void * new_tag){
	long ret = 0;

	uint32_t tag_size = strnlen(new_tag, HEAP_CHUNK_TAG_SIZE);
	if(copy_from_user((void *)cur->tag,(void *)new_tag, tag_size)){
		pr_err("[AVUD] (heap_note) do_change_tag_safe: cannot copy the tag!");
		ret = -EINVAL;
		goto exit;
	}

	// VULN HERE
	// If we supply full length of the TAG SIZE we will overflow by 1 byte
	// to place a null byte
	// Depending on the current type this will modify different things

	cur->tag[tag_size] = '\0';

	exit:
	return ret;
}

inline long do_change_tag_unsafe(important_heap_struct * cur, void * new_tag){
	long ret = 0;

	uint32_t tag_size = strnlen(new_tag, HEAP_CHUNK_TAG_SIZE);
	if(copy_from_user((void *)cur->tag,(void *)new_tag, tag_size)){
		pr_err("[AVUD] (heap_note) do_change_tag_unsafe: cannot copy the tag!");
		ret = -EINVAL;
		goto exit;
	}

	// VULN HERE
	// If we supply full length of the TAG SIZE we will overflow by 1 byte
	// to place a null byte
	// Depending on the current type this will modify different things

	cur->tag[tag_size] = '\0';

	exit:
	return ret;
}

static long heap_note_change_tag(size_t index, void * new_tag){
	long ret = 0;

	if(heap_notes[index] == NULL){
		ret = -EINVAL;
		goto exit;
	}

	important_heap_struct * cur = heap_notes[index];

	if(cur->type == UNSAFE){
		do_change_tag_unsafe(cur,new_tag);
	}else{
		do_change_tag_safe((important_heap_struct_safe *)cur,new_tag);
	}

	exit:

	return ret;
}

inline long do_view_tag_safe(important_heap_struct_safe * cur,void * output){
	long ret = 0;

	size_t size = strnlen(cur->tag, HEAP_CHUNK_TAG_SIZE);

	if(copy_to_user((void *)output,(void *)cur->tag,size)){
		pr_err("[AVUD] (heap_note) do_view_tag_safe: cannot view the tag!");
		ret = -EINVAL;
	}

	return ret;
}

inline long do_view_tag_unsafe(important_heap_struct * cur,void * output){
	long ret = 0;

	size_t size = strnlen(cur->tag, HEAP_CHUNK_TAG_SIZE);

	if(copy_to_user((void *)output,(void *)cur->tag,size)){
		pr_err("[AVUD] (heap_note) do_view_tag_unsafe: cannot view the tag!");
		ret = -EINVAL;
	}

	return ret;
}

static long heap_note_view_tag(size_t index, void * output){
	long ret = 0;

	if(heap_notes[index] == NULL){
		ret = -EINVAL;
		goto exit;
	}

	important_heap_struct * cur = heap_notes[index];
	
	if(cur->type == UNSAFE){
		do_view_tag_unsafe(cur,output);
	}else{
		do_view_tag_safe((important_heap_struct_safe *)cur,output);
	}

	size_t size = strlen(cur->tag);

	if(copy_to_user((void *)output,(void *)cur->tag,size)){
		pr_err("[AVUD] (heap_note) heap_note_view_tag: cannot view the tag!");
		ret = -EINVAL;
	}

	exit:

	return ret;
}

static long heap_note_switch_safe(size_t index){
	long ret = 0;

	if(heap_notes[index] == NULL){
		ret = -EINVAL;
		goto exit;
	}

	important_heap_struct * cur = heap_notes[index];

	important_heap_struct_safe * cur_safe = kzalloc(sizeof(important_heap_struct), GFP_KERNEL);
	if (!cur_safe)
    {
        pr_err("[AVUD] (heap_note) heap_note_switch_safe: couldn't allocate new memory!\n");
        ret = -ENOMEM;
		goto exit;
    }

	// VULN HERE 
	// We are copying the original UNSAFE type structure
	// since the variable positions are changed
	// other parts of the code will be impacted aswell - providing new primitives
	// that were not possible before

	memcpy(cur_safe, cur, sizeof(important_heap_struct));
	cur_safe->type = SAFE;

	kfree(cur);

	heap_notes[index] = cur_safe;

	exit:

	return ret;
}

long heap_note_handler(size_t cmd, avud_request * request){
	
	long ret = 0;
	
	switch(cmd){
		case CTL_ADD:
			ret = heap_note_add(request->input_index, request->input_len, request->input, request->tag);
			break;

		case CTL_EDIT:
			ret = heap_note_edit(request->input_index, request->input);
			break;
		
		case CTL_FREE:
			ret = heap_note_free(request->input_index);
			break;
		
		case CTL_VIEW:
			if(request->output == NULL){
				pr_err("[AVUD] (heap_note) heap_note_handler: cannot provide NULLptr as output!");
				ret = -EINVAL;
				goto exit;
			}
			ret = heap_note_view(request->input_index, request->output);
			break;
		
		case CTL_EDIT_TAG:
			ret = heap_note_change_tag(request->input_index, request->tag);
			break;

		case CTL_VIEW_TAG:
			if(request->output == NULL){
				pr_err("[AVUD] (heap_note) heap_note_handler: cannot provide NULLptr as output!");
				ret = -EINVAL;
				goto exit;
			}
			ret = heap_note_view_tag(request->input_index, request->output);
			break;

		case CTL_SWITCH_SAFE:
			ret = heap_note_switch_safe(request->input_index);
			break;
	}

	exit:

	return ret;
	
}

EXPORT_SYMBOL(heap_note_handler);
