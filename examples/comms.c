
#include "comms.h"

int avud_fd = -1;

int avud_ioctl(avud_request * req, unsigned int cmd){
	if(avud_fd == -2) return -2;
	return ioctl(avud_fd, cmd, req);
}

int BOF_request(avud_request * req){
	return avud_ioctl(req, IOCTL_BOF_STACK);
}