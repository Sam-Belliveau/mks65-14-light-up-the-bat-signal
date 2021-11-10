#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

static void handle_signal(int signal)
{
    const static char LOG_ERROR_MESSAGE[] = "Unable to Write To Log File!\n";
    const static char ERROR_MESSAGE[] = "This Program Has Ended due to a SIGINT signal!\n\n";
    int log;

    if (signal == SIGUSR1)
    {
        printf("No, I will not leave! [PPID: %d]\n", getppid());
    }

    if (signal == SIGINT)
    {
        log = open("./signal.log", O_CREAT | O_APPEND | O_WRONLY);

        if(log < -1) 
        {
            log = STDERR_FILENO;
            write(log, LOG_ERROR_MESSAGE, sizeof(LOG_ERROR_MESSAGE));
        }

        write(log, ERROR_MESSAGE, sizeof(ERROR_MESSAGE));

        exit(signal);
    }
}

int main(void)
{
    int t;
    __pid_t pid, ppid;

    pid = getpid();
    ppid = getppid();

    signal(SIGINT, handle_signal);
    signal(SIGUSR1, handle_signal);

    while(1)
    {
        t = time(NULL);
        printf("%d - [PID: %d] [PPID: %d]\n", t, pid, ppid);
        sleep(1);
    }

    return 0;
}