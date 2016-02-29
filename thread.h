#ifndef _THREAD_H
#define _THREAD_H

<<<<<<< HEAD
#include <time.h>// needed for getrusage
#include <pthread.h>
/*
#include "./server/server.h"
#include "./server/command_analysis.h"
*/
=======
#include <pthread.h>
/*
#include "./server/server.h"
*/
#include "./server/command_analysis.h"
>>>>>>> hiraoka/master

#define THREAD 2
#define DELTA_T 0.02 /* [s] */

#define NSEC_PER_SEC  1000000000 // = 1 [sec]
#define INTERVAL  10000000 // [ns]

<<<<<<< HEAD

/*コマンド情報の構造体*/
typedef struct{
    char large_type;
    char small_type;
    char spare3;
    char left_command;
    char right_command;
    char OP3;
    char OP4;
    char spare4;
    char error_code1;
    char error_code2;
}Command_Info;

=======
>>>>>>> hiraoka/master
typedef struct{
  pthread_mutex_t mutex;
  int recv_flag;
  int end_flag;
  Command_Info command;
  char recv_data[19];
}Thread_Arg;

int make_new_thread(int thNum, pthread_t *thread, Thread_Arg *thread_arg);
<<<<<<< HEAD
void Command_Copy(Command_Info org, Command_Info *copy);
void *Thread_Server(void *_thread_arg);
void *Thread_Motor(void *_thread_arg);
void *Thread_Camera(void *_thread_arg);
=======
void *Thread_Server(void *_thread_arg);
void *Thread_Motor(void *_thread_arg);
>>>>>>> hiraoka/master

#endif
