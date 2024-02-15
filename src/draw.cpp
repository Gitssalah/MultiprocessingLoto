#include <iostream>
using namespace std;
#include "draw.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>


int openW;
static int ballwritten;
const int TEMPO = 1;
int static state = 1;
int table[49];
int k =0;

static void changeState(int nosig)
{
    if(nosig == SIGCONT){state = 1;}
    if(nosig == SIGSTOP) {state = 0;}
}


static void stopDraw (int nosig)
{
    if(nosig == SIGUSR2)
    {
        close(openW);
        cout << "++Similar numbers has been picked++" <<endl;
        exit(0);
    }
    if(nosig == SIGINT)
    {
        cout <<"draw SIGINT";
        close(openW);
        exit(0);
    }
}

static void autoSignal(int numSignal , void (* handler) (int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(numSignal, &action, NULL); //linux
}

static void howSleepShouldBe (int tmp)
{
    tmp = TEMPO;
    while(tmp !=0)
    {
        tmp=sleep(tmp);
    }
}

void D(pid_t loto)
{
    autoSignal(SIGCONT,changeState);
    autoSignal(SIGSTOP,changeState);
    autoSignal(SIGUSR2,stopDraw);
    autoSignal(SIGINT,stopDraw);

    openW=open("./fifo1", O_WRONLY);
    while(true)
    {
        howSleepShouldBe(TEMPO);
            if(state == 1)
        {
            ballwritten = rand()%49 +1;
            ssize_t bytesWritten = write(openW, &ballwritten, sizeof(int));
            if (bytesWritten == -1) {
                perror("Error writing to FIFO");
            }
            kill(loto, SIGUSR1);
            cout << "**I sent SIGUSR1**" <<endl;
            cout << "*Number picked* : " << ballwritten <<endl;
            cout.flush();
            sleep(1);
        }
    }
}