/*
* Implement a user-level sleep program for xv6, along the lines of UNIX sleep command.
* Your sleep should pause for a user-specified of ticks.
* A tick is a notion of time defined by the xv6 kernel, namely the time between two interrupts from the timer chip.
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2) // none || too many
    {
        printf("Usage: sleep <ticks>\n");
        exit(1);
    }

    int ticks = atoi(argv[1]); // char array to integer
    if (ticks <= 0) // invalid ticks
    {
        printf("Error: ticks must be a positive number\n");
        exit(1);
    }

    sleep(ticks); // system call
    exit(0); // normal terminate
}
