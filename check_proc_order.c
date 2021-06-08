#include "types.h"
#include "user.h"
static int iterations = 50*1000*1000;
static double x=0;


int set_priority(int prio);

/*
 fork several several processes, with different priorities, and let them run.
The pattern (in prio_keys[] ):
2 @ prio 1
then 2 @ prio 0
then 1 at prio 3
and for desert, prio 1 again

The expected pattern is
* high prio processes finish before lower gets cpu.
* round robin at prio 0,1,2
*
*/
static int prio_keys[] = {1,1,0,0,2,2,1};


void check_proc_order(){
    printf(1, "parent run at pid %d\n", getpid());

    // I run at default prio 2
    int num_children = sizeof(prio_keys)/sizeof(int);
    for(int k = 0; k < num_children;k++) {
        int pid = fork();
        if (pid < 0) {
            printf(1, "fork failed");
            return;
        }
        if (pid == 0) {
            // in child
            int prio = prio_keys[k]; // index k inherited from parent
		    printf(1,"Child(%d) is setting prio: %d\n", getpid(), prio);
            set_priority(prio);
            // do something that takes cpu time
            for(k=0;k< iterations;k++){
                    x += k; // x is global so the compiler cannot remove this loop
            }
            printf(1,"\nChild(%d) DONE\n", getpid());
            exit();
        } else{
            // in parent
            // simply keep looping and forking more processes
        }
    }

    // all processes are on their way.
    // Wait here until all of them finished.
    for(int i = 0; i < num_children; i++) {
        wait();
    }
    printf(1,"\nPARENT finished\n");
}

