/* 
 * Write a simple version of the UNIX xargs program for xv6: its arguments describe a command to run, it reads lines from the standard input, and it runs the command for each line, appending the line to the command's arguments. 
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LINE 512

int main(int argc, char *argv[])
{
    char buf[MAX_LINE]; // input buffer
    char *args[MAXARG]; // command args array
    int i, n = 0, index = 0;

    // command args copy
    for (i = 1; i < argc; i++)
    {
        args[n++] = argv[i];
    }

    // read input by 1 char, seperate by line
    while (read(0, &buf[index], 1) == 1)
    {
        if (buf[index] == '\n')
        {
            buf[index] = '\0'; // end of string
            args[n] = buf; // add current line as arg
            args[n+1] = 0; // end with NULL

            if (fork() == 0)
            {
                exec(args[0], args); // execute command
                exit(0); // terminate child process
            }
            else
            {
                wait(0); // parent: wait for child to complete
            }
            index = 0; // reset buffer for next line
        }
        else
        {
            index++;
        }
    }
    exit(0);
}
