#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>

static char stack[1048576];

typedef struct rlimit rlimit;
typedef struct sandbox {
    char** args; //TODO
    int process_id;
    int parent_process_id;
    int run_process;
} sandbox;

static int handler(int reg) {
    //TODO
}
static void set_resource_limits(int cur, int max) {
    rlimit* new_limit = calloc(1, sizeof(rlimit));
    new_limit->rlim_cur = cur;
    new_limit->rlim_max = max;
    setrlimit(RLIMIT_CPU, new_limit);
}

static int sandboxed_process() {
    sandbox* bunk = calloc(1, sizeof(sandbox));
    bunk->process_id = getpid();
    bunk->parent_process_id = getppid();
    printf("[SETTING RESOURCE LIMITS FOR] %d\n", bunk->parent_process_id);
    set_resource_limits(1, 1);
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    if (bunk->run_process = fork() == 0) {
        while(1) {
                printf("[PASSING]\n");
        }
    }
    int status;
    while(waitpid(bunk->run_process, &status, 0)) {
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, bunk->run_process, NULL, &regs);
        printf("[SYSTEM CALL] %d [PID] %d\n", regs.orig_rax, bunk->run_process);
        ptrace(PTRACE_SYSCALL, bunk->run_process, NULL, NULL);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("[ARGUMENT ERROR]\n");
        exit(1);
    }
    pid_t process = fork();
    if (process < 0) {
        printf("[FORK ERROR]");
        exit(1);
    }
    else if (process == 0) {
        printf("[PROCESSING]\n");
       // void* stack = calloc(1, 1024*1024);
        pid_t sandbox_pid = clone(sandboxed_process, stack + (1024*1024), CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD, NULL);
        int status;
        waitpid(sandbox_pid, &status, 0);
        //free(stack);
        exit(0);
    }
    else {
        wait(NULL);
        printf("[SUCESSS]\n");
    }
}
