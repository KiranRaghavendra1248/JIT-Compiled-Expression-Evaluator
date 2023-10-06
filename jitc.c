/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * jitc.c
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dlfcn.h>
#include "system.h"
#include "jitc.h"

/**
 * Needs:
 *   fork()
 *   execv()
 *   waitpid()
 *   WIFEXITED()
 *   WEXITSTATUS()
 *   dlopen()
 *   dlclose()
 *   dlsym()
 */

/* research the above Needed API and design accordingly */
struct jitc {
	void * handle;
};

int jitc_compile(const char *input, const char *output){
    pid_t pid = fork(); 
    /* pid = 0, N, -1 => returns 0 to child process, N=pid(child) for parent process, -1 to the parent process if fork failed*/
    if(0==pid){  /* if(!pid) */
        /* Child process - run .c file to generate the .so file using gcc command*/
        char * argv[7];
        argv[0] = "gcc";
        argv[1] = "-shared";
        argv[2] = "-o";
        argv[3] = (char *)output;
        argv[4] = "-fPIC";
        argv[5] = (char *)input;
        argv[6] = NULL;
        execv("/usr/bin/gcc", argv);
        return -1;
    }
    else if(pid>0){
        /* Parent Process - wait for execution of child process and finish*/
        int status;
        waitpid(pid, &status, 0);
        if ( WIFEXITED(status) ){
        int exit_status = WEXITSTATUS(status);        
        printf("Exit status of the child was %d\n", exit_status);}
        return 0;
    }
    else{
        TRACE("\nfork creation failed!!!\n");
        return -1;
    }
}

struct jitc *jitc_open(const char *pathname){
    void *handle = dlopen(pathname, RTLD_LAZY);
    struct jitc * ptr = (struct jitc *)malloc(sizeof(struct jitc));
    if(NULL==handle){
        free(ptr);
        return NULL;
    }
    ptr->handle = handle;
    return ptr;
}

void jitc_close(struct jitc *jitc){
    if(!dlclose(jitc->handle)){
        printf("Sucess");
    }
    else{
        printf("Fail");
    }
    free(jitc);
}

long jitc_lookup(struct jitc *jitc, const char *symbol){
    long address;
    address = (long)dlsym(jitc->handle, symbol);
    return address;
}