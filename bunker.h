#ifndef BUNKER
    #define BUNKER

    #define _GNU_SOURCE
    #include <sched.h>
    #include <unistd.h> 
    #include <stdio.h> 
    #include <stdlib.h>
    #include <signal.h>
    #define STACK_SIZE 8192

    typedef struct user_regs_struct registers;

    static void* alloc_stack() {
        void* stack = calloc(1, STACK_SIZE);
        if (!stack){
            perror("[MEMORY ALLOCATION FAILED]");
            exit(1);
        }
        return stack;
    }

    static int clone_flags = CLONE_NEWUSER 
                            | CLONE_NEWPID 
                            | CLONE_NEWUTS 
                            | SIGCHLD;

#endif                        