#include <iostream>
using namespace std;
#include "config.h"
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


static int ballStored [MAX]; //table may not be the best solution but it does the job quite well
                      //table initilized by zero 
static int numStored  = 0;
static int openR;
static int pickedBall;
static int TEMPO = 3;

static bool inTable(int element, int table[MAX])
{
    for( int i =0; i< MAX; i++)
    {
        if(table[i] == element)
        {
            return true;
        }   
    }
    return false;
}
static void readBall(int nosig)
{
    cout <<"**I received SIGUSR1**"<<endl;
    ssize_t bytesRead = read(openR, &pickedBall, sizeof(int));
    if (bytesRead == -1) {
        perror("Error reading from FIFO");
    }
    
    if(inTable(pickedBall,ballStored))
    {
        kill(getppid(),SIGUSR2);
        cout <<"**I sent SIGUSR2**" <<endl;
        close(openR);
        cout <<"loto gone"<<endl;
        exit(0);
        
    }
    else 
    {
        ballStored[numStored] = pickedBall;
        numStored++;
        cout << pickedBall <<" *ball added*" <<endl;
    }
}



static void stopLoto(int nosig)
{
    cout <<"SIGINT LOTO" <<endl;
    close(openR);
    kill(getppid(), SIGUSR2);
    exit(0);
}

static void autoSignal(int numSignal , void (* handler) (int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(numSignal, &action, NULL); //linux
}

static void display ()
{
    cout << "the list of number drawn: --------------- "<<endl;
    for (int i = 0 ; i<numStored; i++)
    {
        cout << ballStored[i] << " ";
    }
    cout <<"************";
    cout <<endl;
    cout <<endl;
}

static void howSleepShouldBe (int tmp)
{
    tmp = TEMPO;
    while(tmp !=0)
    {
        tmp=sleep(tmp);
    }
}

void L()
{
    

    
    openR=open("./fifo1",O_RDONLY);
    autoSignal(SIGUSR1,readBall);
    autoSignal(SIGINT,stopLoto);
    autoSignal(SIGQUIT,SIG_IGN);
    while(true)
    {
    	howSleepShouldBe(TEMPO);
    	display();
    }

}
