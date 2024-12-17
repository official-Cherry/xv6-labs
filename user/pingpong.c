/* Write a user-level program that uses xv6 system calls to ''ping-pong'' a byte between two processes over a pair of pipes, one for each direction.
 * The parent should send a byte to the child; the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit; the parent should read the byte from the child, print "<pid>: received pong", and exit.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() 
{
    int p1[2]; // pipe1 (parent->child)
    int p2[2]; // pipe2 (child->parent)
    char buffer = 'A'; // send byte

    // initialize pipe
    pipe(p1);
    pipe(p2);

    int pid = fork(); // make child process

    if(pid > 0) // parent process
    {
        close(p1[0]); // parent: p1 close read
        close(p2[1]); // parent: p2 close write

        write(p1[1], &buffer, 1); // send byte to child
        read(p2[0], &buffer, 1); // receive from child

        printf("%d: received pong\n", getpid());
        close(p1[1]);
        close(p2[0]);
    }
    else if(pid == 0) // child process
    {
        close(p1[1]); // child: p1 close write
        close(p2[0]); // child: p2 close read

        read(p1[0], &buffer, 1); // receive from parent
        printf("%d: received ping\n", getpid());
        write(p2[1], &buffer, 1); // send to parent

        close(p1[0]);
        close(p2[1]);
    }
    else
    {
        printf("fork failed\n");
        exit(1);
    }

    exit(0);
}

