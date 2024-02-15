#include <iostream>
#include "loto.h"
#include "draw.h"
#include "config.h"
using namespace std;
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int state = 1;
pid_t loto;
pid_t draw;

static void autoSignal(int numSignal , void (* handler) (int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(numSignal, &action, NULL); //linux
}

static void reactToSigquit(int nosig)
{
    cout << "I send a SIGQUIT to draw" <<endl;
    cout << "state "<< state <<endl;
    if(state == 0)
    {
        cout <<"I send SIGCONT"<< endl;

        kill(draw , SIGCONT);
        state = 1;
    }
    else
    {
        cout << "I send SIGSTOP"<< endl;
        kill(draw, SIGSTOP);
        state = 0;
    } 
}

static void reactToSigusr2(int nosig)
{
    kill(draw, SIGUSR2);
}




int main()
{
    /* Initialisation phase : link the signals to  its handlers + creating 2 processurs : Loto and Draw 
       destruction phase : destroy the canal fifo + destroy Loto and Draw (terminate after destruction done with linux)
                           self destroy
    */
    autoSignal(SIGUSR1,SIG_IGN);
    autoSignal(SIGUSR2,SIG_IGN);
    autoSignal(SIGPIPE, SIG_IGN);
   
    
    if((loto = fork())== 0)
    {
        L();
    }
    else{
        if((draw = fork())== 0)
        {
            D(loto);
        }
        else
        {
            autoSignal(SIGQUIT, reactToSigquit);
            autoSignal(SIGUSR2, reactToSigusr2);
            //waitpid(loto,NULL,0);
            //waitpid(draw,NULL,0);
            autoSignal(SIGINT,SIG_DFL);
            exit(0);


        }
    }
}
