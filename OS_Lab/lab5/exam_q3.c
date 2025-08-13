#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PROCESSES 3

typedef struct {
    int pid;
    int burst_time;
    int arrival_time;
    int completion_time;
    int turnaround_time;
} Process;

// Function to generate random processes
void generate_processes(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("P%d ",processes[i].pid);
        processes[i].burst_time = rand() % 15 + 1;
        printf("burst time : %d ",processes[i].burst_time);
        processes[i].arrival_time = rand() % 5;
        printf("arrival time : %d \n",processes[i].arrival_time);
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
    }
}

// Comparator for sorting by arrival time (FIFO)
int compare_arrival(const void *a, const void *b) {
    return ((Process *)a)->arrival_time - ((Process *)b)->arrival_time;
}

// Comparator for sorting by burst time (SJF)
int compare_burst(const void *a, const void *b) {
    return ((Process *)a)->burst_time - ((Process *)b)->burst_time;
}

// FIFO Scheduling
void fifo_scheduling(Process processes[], int n) {
    qsort(processes, n, sizeof(Process), compare_arrival);
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time)
            current_time = processes[i].arrival_time;
        
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
    }
}

// SJF Scheduling
void sjf_scheduling(Process processes[], int n) {
    qsort(processes, n, sizeof(Process), compare_burst);
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time)
            current_time = processes[i].arrival_time;
        
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
    }
}

// STCF Scheduling
void stcf_scheduling(Process processes[], int n) {
    int remaining_time[MAX_PROCESSES];
    bool completed[MAX_PROCESSES] = {false};
    for (int i = 0; i < n; i++)
        remaining_time[i] = processes[i].burst_time;
    
    int completed_count = 0, current_time = 0;
    while (completed_count < n) {
        int shortest = -1;
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival_time <= current_time &&
                (shortest == -1 || remaining_time[i] < remaining_time[shortest])) {
                shortest = i;
            }
        }
        
        if (shortest == -1) {
            current_time++;
            continue;
        }
        
        remaining_time[shortest]--;
        if (remaining_time[shortest] == 0) {
            completed[shortest] = true;
            completed_count++;
            processes[shortest].completion_time = current_time + 1;
            processes[shortest].turnaround_time = processes[shortest].completion_time - processes[shortest].arrival_time;
        }
        current_time++;
    }
}

// Function to calculate and print average turnaround time
void calculate_average_turnaround(Process processes[], int n, const char *policy) {
    int total_turnaround = 0;
    for (int i = 0; i < n; i++)
        total_turnaround += processes[i].turnaround_time;
    
    printf("%s Average Turnaround Time: %.2f\n", policy, (float)total_turnaround / n);
}

int main() {
    Process processes[MAX_PROCESSES];
    
    int n = MAX_PROCESSES;
    srand(time(NULL));
    generate_processes(processes, n);
    
    Process fifo_processes[MAX_PROCESSES];
    Process sjf_processes[MAX_PROCESSES];
    Process stcf_processes[MAX_PROCESSES];
    
    for (int i = 0; i < n; i++) {
        fifo_processes[i] = processes[i];
        sjf_processes[i] = processes[i];
        stcf_processes[i] = processes[i];
    }
    
    fifo_scheduling(fifo_processes, n);
    sjf_scheduling(sjf_processes, n);
    stcf_scheduling(stcf_processes, n);
    
    calculate_average_turnaround(fifo_processes, n, "FIFO");
    calculate_average_turnaround(sjf_processes, n, "SJF");
    calculate_average_turnaround(stcf_processes, n, "STCF");
    
    return 0;
}
