/*  parallelize: simple shell command parallelizer.

    Useful if you have a multiprocessor computer and need to run a batch of
        single-threaded CPU-intensive tasks, such as image processing or 
        media encoding.

    Pipe a list of commands to stdin.
    The only argument is the number of simultaneous processes to run.
    
    parallelize will execute that number of the input commands simultaneously
        until the list is exhausted.
        
    How to build:
    gcc -pthread parallelize.c -o parallelize
    
    Demo:
    (for i in 1 2 3 4 5 6 7 8 9 10; do echo "echo 'Delay $i'; sleep $i ; \
    echo 'Done $i'" ; done) | ./parallelize 4
    
    By Marco Arment <me@marco.org>, released into the public domain with no
        restrictions or conditions. I'd appreciate bug reports, though.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_LINE_LENGTH 1024

int             g_done = 0;
pthread_mutex_t g_mutex_launch;

void *thread_main(void *null)
{
    char line[MAX_LINE_LENGTH];
    pid_t child_pid;
    char *result;
    
    while (! g_done) {
        pthread_mutex_lock(&g_mutex_launch);

        if (! g_done && (result = fgets(line, MAX_LINE_LENGTH, stdin)) ) {
            if (! (child_pid = fork()) ) {
                system(line);
                exit(0);
            }
        } else g_done = 1;
        
        pthread_mutex_unlock(&g_mutex_launch);
        if (result) waitpid(child_pid, NULL, 0);
    }
    
    pthread_exit(NULL);
}

void child_after_fork() { g_done = 1; }

int main(int argc, const char **argv)
{
    pthread_t *threads;
    int       i, thread_count;
    
    if (argc < 2 || (thread_count = atoi(argv[1])) < 1) {
        fprintf(stderr, "Usage: parallelize {thread_count}\n");
        exit(1);
    }
    
    pthread_mutex_init(&g_mutex_launch, NULL);
    pthread_atfork(NULL, NULL, child_after_fork);

    threads = malloc(thread_count * sizeof(pthread_t));
    for (i = 0; i < thread_count; i++) {
        if (pthread_create(&threads[i], NULL, thread_main, NULL)) {
            fprintf(stderr, "parallelize: cannot create thread %d\n", i);
            exit(-1);
        }
    }

    pthread_exit(NULL);
}
