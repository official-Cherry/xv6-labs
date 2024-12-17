/*
 * Write a concurrent prime sieve program for xv6 using pipes and the design illustrated in the picture halfway down this page and the surrounding text.
 * This idea is due to Doug McIlroy, inventor of Unix pipes.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int filter(int p_left[2])
{
    close(p_left[1]); // close write end of parent pipe
    
    int prime, num;
    if(read(p_left[0], &prime, sizeof(prime)) == 0)
    {
        // close if no more number
        close(p_left[0]);
        return 0; // exit
    }

    printf("prime %d\n", prime);

    int p_right[2];
    pipe(p_right);

    if (fork() == 0)
    {
        // child: move to next filter
        close(p_left[0]); // close parent pipe
        filter(p_right);
    }
    else
    {
        // parent: pass num after filtering
        close(p_right[0]);
        while (read(p_left[0], &num, sizeof(num)) > 0)
        {
            // pass num, which not divided by prime
            if (num % prime != 0)
            {
                write(p_right[1], &num, sizeof(num));
            }
        }
        close(p_left[0]);
        close(p_right[1]);
        wait(0); // wait for child to finish
        return 0; // exit
    }
	return 0;
}

int main()
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        // child: execute first filter
        filter(p);
    }
    else
    {
        close(p[0]); // close read end of parent pipe
        for (int i = 2; i <= 280; i++)
        {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]); // close write end
        wait(0);
        exit(0);
    }
    return 0;
}

