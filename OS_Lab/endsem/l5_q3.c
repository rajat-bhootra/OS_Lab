#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define TICKS 100

typedef struct 
{
    int id;
    int ticket;
    int allocated; 
    double stride;
    double pass_value;
}Process;


void lottery(Process proc[],int n){
    int total_ticket = 0;
    for (int i = 0; i < n; i++)
    {
        total_ticket += proc[i].ticket;
    }

    
    for (int i = 0; i < TICKS; i++)
    {
        int lottery = rand()%total_ticket+1;
        int sum = 0;
        for (int i = 0; i < n; i++)
        {
            sum +=proc[i].ticket;
            if (lottery <= sum){
                proc[i].allocated++;
                break;
            }
        }        
    }
}

void stride(Process proc[],int n){
    for ( int i = 0; i < n; i++)
    {
        proc[i].stride = (10000/proc[i].ticket)*TICKS;
    }

    for (int i = 0; i < TICKS; i++)
    {
        int min_ind = 0;
        for (int i = 0; i < n; i++)
        {
            if (proc[i].pass_value < proc[min_ind].pass_value){
                min_ind = i;
            }
        }
        proc[min_ind].allocated++;
        proc[min_ind].pass_value+=proc[i].stride;
        
    }
    
    
}

int main(){
    int num = 3;
    Process *proc = (Process *)malloc(num*sizeof(Process));

    for (int i = 0; i < num; i++)
    {
        proc[i].id = i+1;
        int tickets;
        printf("enter tickets: ");
        scanf("%d",&tickets);
        proc[i].ticket = tickets;
        proc[i].allocated = 0;
    }

    int total_tickets;
    for (int i = 0; i < num; i++)
    {
        total_tickets+=proc[i].ticket;
    }
    
    // stride(proc,num);
    lottery(proc,num);

    for (int i = 0; i < num; i++)
    {
        double expected = ((double)proc[i].ticket/total_tickets)*TICKS;
        printf("process %d : tickets = %d ,allocated = %d ,expected = %.2f, diff = %.2f\n",proc[i].id, proc[i].ticket,proc[i].allocated,expected,expected-proc[i].allocated);
    }
    
    free(proc);
    
}