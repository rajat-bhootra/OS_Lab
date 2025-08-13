#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENT  10
#define MAX_PROCESS  10
typedef struct{
    char type[10];
    int time;
} Event;

typedef struct{
    char name[10];
    int arrivalTime;
    int eventCount;
    int totalTime ;
    Event event[MAX_EVENT];
    
} Process;

void read_file(char *filename, Process processes[],int *processCount){
    FILE *file = fopen(filename , "r");
    *processCount = 0;
    char wow[256];
    while(fgets(wow,sizeof(wow),file)  && *processCount < MAX_PROCESS){
        Process *p = &processes[*processCount];
        char *token = strtok(wow," ");
        if(!token){continue;}
        strcpy(p->name,token);
        token = strtok(NULL, " ");
        if (!token) continue;
        p->arrivalTime = atoi(token);
        p->eventCount = 0;
        p->totalTime = 0;
        while(token != strtok(NULL," ")){
            strcpy(p->event[p->eventCount].type, token);
            token = strtok(NULL, " ");
            if (!token) break;
            p->event[p->eventCount].time = atoi(token);
            p->totalTime += atoi(token);
            (p->eventCount)++;
        }
        (*processCount)++;
    }
    
}
int comp(const void *a, const void *b) {
    return ((Process*)a)->arrivalTime - ((Process*)b)->arrivalTime;
}

void fifo(Process processes[],int *processCount){ 
    qsort(processes, *processCount, sizeof(processes[0]), comp);
}

void print(Process processes[],int *processCount){
    for (int i = 0; i < *processCount; i++) {
        printf("Process Name: %s, Arrival Time: %d\n ", processes[i].name, processes[i].arrivalTime);
    }
}

void remove_process(Process processes[], int *processCount, int index) {
    if (index < 0 || index >= *processCount) {
        printf("Invalid index: %d\n", index);
        return;
    }
    for (int i = index; i < *processCount - 1; i++) {
        processes[i] = processes[i + 1];
    }
    (*processCount)--;  
}

int comp1(const void *a, const void *b) {
    return ((Process*)a)->totalTime - ((Process*)b)->totalTime;
}

void sjb(Process processes[], int *processCount) {
    int currentTime = 0;  
    int completed = 0;
    int pcount = *processCount;
    while (completed < pcount) {
        int availableCount = 0;
        Process availableProcesses[MAX_PROCESS];

        // Find processes that have arrived
        for (int i = 0; i < *processCount; i++) {
            if (processes[i].arrivalTime <= currentTime) {
                availableProcesses[availableCount++] = processes[i];
            }
        }

        // Sort only the available processes by total execution time
        if (availableCount > 0) {
            qsort(availableProcesses, availableCount, sizeof(Process), comp1);

            // Execute the shortest job
            Process *currentProcess = &availableProcesses[0];

            printf("Executing Process: %s | Start: %d | Completion: %d\n",
                   currentProcess->name, currentTime, currentTime + currentProcess->totalTime);

            currentTime += currentProcess->totalTime;
            completed++;

            // Remove the completed process from the original list
            for (int i = 0; i < *processCount; i++) {
                if (strcmp(processes[i].name, currentProcess->name) == 0) {
                    // Shift remaining processes
                    for (int j = i; j < *processCount - 1; j++) {
                        processes[j] = processes[j + 1];
                    }
                    (*processCount)--;
                    break;
                }
            }
        } else {
            // If no process is available, move time forward
            currentTime+=processes[completed].arrivalTime;
        }
    }
}

void fifoprint(Process processes[],int *processCount){
    int currentTime = 0;
    for (int i = 0; i < *processCount; i++) {
        printf("Process Name: %s, Arrival Time: %d ", processes[i].name, processes[i].arrivalTime);
        int responseTime = currentTime - processes[i].arrivalTime;
        // if(i==0){
        //     responseTime = 0;
        // }
        // else{responseTime = currentTime - processes[i].arrivalTime;}
        
        currentTime+= processes[i].totalTime;
        printf("Completed: %d , TurnAround: %d , ResponseTime: %d\n",currentTime , currentTime - processes[i].arrivalTime, responseTime);
    }
}

int main(){
    int processCount;
    Process processes[MAX_PROCESS];
    read_file("input.txt",processes, &processCount);
    print(processes, &processCount);
    printf("\n\nFIFO IMPLEMENTATION\n\n\n");
    fifo(processes, &processCount);
    fifoprint(processes, &processCount);
    printf("\n\nSJB IMPLEMENTATION\n\n\n");
    sjb(processes, &processCount);
}