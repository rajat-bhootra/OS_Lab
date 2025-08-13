#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TICKS 100

typedef struct{
   int id;
   int tickets;
   int allocated;
   double stride;
   double passvalue;
} Process;

void lottery(Process procs[], int n){
    int Total_tickets = 0;
    for (int i = 0; i < n; i++){
       Total_tickets += procs[i].tickets;
    }
    for (int tick = 0; tick < TICKS; tick++){
       int lottery = rand() % Total_tickets + 1;
       int sum = 0;
       for (int i = 0; i < n; i++){
           sum += procs[i].tickets;
           if (lottery <= sum){
               procs[i].allocated++;
               break;
           }
       }     
    }        
}

void stride(Process procs[], int n){
    for (int i = 0; i < n; i++){
        procs[i].stride = (10000/procs[i].tickets) * TICKS;
    }
     
    for (int ticks = 0; ticks < TICKS; ticks++){
        int min_index = 0;
        for (int i = 0; i < n; i++){
            if (procs[i].passvalue < procs[min_index].passvalue){
                min_index = i;
            }
        }
        procs[min_index].allocated++;
        procs[min_index].passvalue+= procs[min_index].stride;   
    }
}


int main(){
    int n;
    printf("Enter n : ");
    scanf("%d",&n);

    Process *procs = (Process *)malloc(n * sizeof(Process));

    srand(time(NULL));

    for (int i = 0; i<n; i++){
        procs[i].id = i+1;
        int ticket;
        scanf("%d",&ticket);
        procs[i].tickets = ticket;
        procs[i].allocated =0;
        procs[i].stride =0;
        procs[i].passvalue =0.0;
    }

    lottery(procs,n);
    // stride(procs,n);
    
    int Total_tickets = 0;
    for (int i = 0; i < n; i++){
       Total_tickets += procs[i].tickets;
    }
 
    for (int i = 0; i < n; i++) {
        double expected = ((double)procs[i].tickets / Total_tickets) * TICKS;
        printf("Process %d: Tickets = %d, Allocated = %d, Expected = %.2f, Diff = %.2f\n",
               procs[i].id, procs[i].tickets, procs[i].allocated, expected, procs[i].allocated - expected);
    }

    free(procs);
    return 0;
}