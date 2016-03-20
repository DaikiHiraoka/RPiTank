#ifndef _CAMERA_H
#define _CAMERA_H

#include "../thread.h"
#define CAMERA_DT 0.2

int Camera_main(Command_Info *command, Thread_Arg *thread_arg);
extern "C" void *Thread_Camera(void *_thread_arg);
#endif
