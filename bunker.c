#include "bunker.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
char** args;

static int bunker() {
    int isolated_process = fork();
    switch(isolated_process){
        case -1:
            perror("[FORK]\n");
            exit(EXIT_FAILURE);
        case 0:
            //chroot("/Bunker/fs");
            //chdir("/");
            //mount("proc", "proc", "proc", 0, "");
            ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
            if(execvp(args[0], args+1)) {
                perror("[EXECVP]");
            } 
        default:
                while(waitpid(child, &status, 0) && ! WIFEXITED(status)) {
                struct user_regs_struct regs; 
                ptrace(PTRACE_GETREGS, child, NULL, &regs);
                fprintf(stderr, "system call %s from pid %d\n", regs, child);
                ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
    return 0;
}

int main(int argc, char** argv) {
    args = argv + 1;
    pid_t init_pid = fork();
    auto int init_fork_status;    
    switch(init_pid) {
        case -1:
            perror("[FORK]\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("[ISOLATING]\n");
            void* clone_stack = alloc_stack();
            pid_t bunker_pid = clone(bunker, (char *)clone_stack + STACK_SIZE, clone_flags, 0);
            if (bunker_pid < 0){
                free(clone_stack);
                perror("[FAILED | CLONE ERROR]");
                exit(EXIT_FAILURE);
            } else {
                auto int bunker_status;
                if (waitpid(bunker_pid, &bunker_status, WUNTRACED | WCONTINUED) < 0) {
                    perror("[ERROR]");
                    exit(EXIT_FAILURE);
                }
                if (WIFEXITED(bunker_status))
                    printf("[SUCCESSFULLY ISOLATED | STATUS %d]\n", WEXITSTATUS(init_fork_status), getpid());
            }
        default:
            wait(&init_fork_status);
            if (WIFEXITED(init_fork_status)) {
                printf("[FINISHED] %d\n", WEXITSTATUS(init_fork_status));
                exit(EXIT_SUCCESS);
            }
    }
}