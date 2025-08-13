// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// #define MAX_JOBS 20
// #define MAX_PROCS 100
// #define LINE_SIZE 256

// typedef enum { NEW, READY, RUNNING, BLOCKED, FINISHED } State;

// typedef struct {
//     char type;      // 'C' (CPU) or 'I' (I/O)
//     int duration;   // duration of this burst
// } Job;

// typedef struct {
//     char name[10];
//     int arrival_time;
//     int num_jobs;
//     Job jobs[MAX_JOBS];
//     int current_job;       // index of the current job
//     int remaining;         // remaining time in current burst
//     State state;
//     int first_cpu_time;    // time when CPU first allocated (for response time)
//     int finish_time;       // time when process finished
//     int rr_quantum;        // for Round Robin & MLFQ: remaining quantum
//     int priority;          // for MLFQ: current queue level (0=highest)
// } Process;

// /* -------- File Parsing --------
//    Each line is expected to be of the form:
//    P1 5 C 20 I 10 C 200 I 10
//    That is, process name, arrival time, then pairs (job type, duration).
// */
// int parse_file(const char *filename, Process procs[]) {
//     FILE *fp = fopen(filename, "r");
//     if (!fp) {
//         perror("fopen");
//         exit(EXIT_FAILURE);
//     }
//     char line[LINE_SIZE];
//     int count = 0;
//     while (fgets(line, sizeof(line), fp) && count < MAX_PROCS) {
//         if (strlen(line) < 2) continue;
//         Process *p = &procs[count];
//         p->num_jobs = 0;
//         p->current_job = 0;
//         p->state = NEW;
//         p->first_cpu_time = -1;
//         p->finish_time = -1;
//         p->priority = 0;  // for MLFQ, start at highest priority
//         // Tokenize the line.
//         char *token = strtok(line, " \t\n");
//         if (!token) continue;
//         strcpy(p->name, token);
//         token = strtok(NULL, " \t\n");
//         if (!token) continue;
//         p->arrival_time = atoi(token);
//         // Read job pairs.
//         while ((token = strtok(NULL, " \t\n")) != NULL) {
//             // token should be a job type character.
//             char jtype = token[0];
//             token = strtok(NULL, " \t\n");
//             if (!token) break;
//             int duration = atoi(token);
//             p->jobs[p->num_jobs].type = jtype;
//             p->jobs[p->num_jobs].duration = duration;
//             p->num_jobs++;
//         }
//         // Initialize the remaining time for the first job.
//         if (p->num_jobs > 0 && p->jobs[0].type == 'C') {
//             p->remaining = p->jobs[0].duration;
//         }
//         count++;
//     }
//     fclose(fp);
//     return count;
// }

// /* -------- Utility: Check if all processes finished -------- */
// int all_finished(Process procs[], int n) {
//     for (int i = 0; i < n; i++) {
//         if (procs[i].state != FINISHED)
//             return 0;
//     }
//     return 1;
// }

// /* -------- FIFO Simulation --------
//    Non-preemptive: when CPU is free, pick the process that arrived first (among those READY).
// */
// void simulate_fifo(Process orig[], int n) {
//     // Make a copy of processes so that each simulation is independent.
//     Process procs[MAX_PROCS];
//     memcpy(procs, orig, n * sizeof(Process));
//     int time = 0;
//     int running_index = -1;  // index of process currently in CPU
//     while (!all_finished(procs, n)) {
//         // Check for arrivals: NEW -> READY when arrival_time <= time.
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == NEW && procs[i].arrival_time <= time)
//                 procs[i].state = READY;
//         }
//         // Update BLOCKED processes (I/O bursts).
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == BLOCKED) {
//                 procs[i].remaining--;
//                 if (procs[i].remaining <= 0) {
//                     // I/O finished; move to next job.
//                     procs[i].current_job++;
//                     if (procs[i].current_job < procs[i].num_jobs) {
//                         // The next job must be CPU.
//                         procs[i].remaining = procs[i].jobs[procs[i].current_job].duration;
//                         procs[i].state = READY;
//                     } else {
//                         procs[i].state = FINISHED;
//                         procs[i].finish_time = time;
//                     }
//                 }
//             }
//         }
//         // If CPU idle, pick a process from READY queue (FIFO order: lowest arrival time).
//         if (running_index == -1) {
//             int candidate = -1;
//             int earliest = 1e9;
//             for (int i = 0; i < n; i++) {
//                 if (procs[i].state == READY) {
//                     if (procs[i].arrival_time < earliest) {
//                         earliest = procs[i].arrival_time;
//                         candidate = i;
//                     }
//                 }
//             }
//             if (candidate != -1) {
//                 running_index = candidate;
//                 procs[running_index].state = RUNNING;
//                 if (procs[running_index].first_cpu_time < 0)
//                     procs[running_index].first_cpu_time = time;
//             }
//         }
//         // Run the process in CPU.
//         if (running_index != -1) {
//             procs[running_index].remaining--;
//             // If finished current CPU burst:
//             if (procs[running_index].remaining <= 0) {
//                 // Move to next job.
//                 procs[running_index].current_job++;
//                 if (procs[running_index].current_job < procs[running_index].num_jobs) {
//                     // Next burst is I/O.
//                     procs[running_index].state = BLOCKED;
//                     procs[running_index].remaining = procs[running_index].jobs[procs[running_index].current_job].duration;
//                 } else {
//                     procs[running_index].state = FINISHED;
//                     procs[running_index].finish_time = time + 1;
//                 }
//                 running_index = -1; // CPU becomes idle.
//             }
//         }
//         time++;
//     }
//     // Report metrics.
//     printf("FIFO Scheduling Results:\n");
//     for (int i = 0; i < n; i++) {
//         int response = procs[i].first_cpu_time - procs[i].arrival_time;
//         int turnaround = procs[i].finish_time - procs[i].arrival_time;
//         printf("%s: Arrival Time = %d, Finish Time = %d, Response Time = %d, Turnaround Time = %d\n",
//                procs[i].name,procs[i].arrival_time, procs[i].finish_time, response, turnaround);
//     }
// }

// /* -------- SJF Simulation --------
//    Non-preemptive: when CPU is free, pick READY process with smallest next CPU burst.
// */
// void simulate_sjf(Process orig[], int n) {
//     Process procs[MAX_PROCS];
//     memcpy(procs, orig, n * sizeof(Process));
//     int time = 0;
//     int running_index = -1;
//     while (!all_finished(procs, n)) {
//         // Update arrivals.
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == NEW && procs[i].arrival_time <= time)
//                 procs[i].state = READY;
//         }
//         // Update BLOCKED (I/O).
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == BLOCKED) {
//                 procs[i].remaining--;
//                 if (procs[i].remaining <= 0) {
//                     procs[i].current_job++;
//                     if (procs[i].current_job < procs[i].num_jobs) {
//                         procs[i].remaining = procs[i].jobs[procs[i].current_job].duration;
//                         procs[i].state = READY;
//                     } else {
//                         procs[i].state = FINISHED;
//                         procs[i].finish_time = time;
//                     }
//                 }
//             }
//         }
//         // If CPU idle, select READY process with shortest CPU burst (current remaining).
//         if (running_index == -1) {
//             int candidate = -1;
//             int shortest = 1e9;
//             for (int i = 0; i < n; i++) {
//                 if (procs[i].state == READY) {
//                     if (procs[i].remaining < shortest) {
//                         shortest = procs[i].remaining;
//                         candidate = i;
//                     }
//                 }
//             }
//             if (candidate != -1) {
//                 running_index = candidate;
//                 procs[running_index].state = RUNNING;
//                 if (procs[running_index].first_cpu_time < 0)
//                     procs[running_index].first_cpu_time = time;
//             }
//         }
//         // Run CPU.
//         if (running_index != -1) {
//             procs[running_index].remaining--;
//             if (procs[running_index].remaining <= 0) {
//                 procs[running_index].current_job++;
//                 if (procs[running_index].current_job < procs[running_index].num_jobs) {
//                     procs[running_index].state = BLOCKED;
//                     procs[running_index].remaining = procs[running_index].jobs[procs[running_index].current_job].duration;
//                 } else {
//                     procs[running_index].state = FINISHED;
//                     procs[running_index].finish_time = time + 1;
//                 }
//                 running_index = -1;
//             }
//         }
//         time++;
//     }
//     printf("SJF Scheduling Results:\n");
//     for (int i = 0; i < n; i++) {
//         int response = procs[i].first_cpu_time - procs[i].arrival_time;
//         int turnaround = procs[i].finish_time - procs[i].arrival_time;
//         printf("%s: Response Time = %d, Turnaround Time = %d\n",
//                procs[i].name, response, turnaround);
//     }
// }

// /* -------- STCF Simulation --------
//    Preemptive: at each tick, select from READY processes (and the running one) the one with
//    smallest remaining CPU burst time. If io_aware==1, the remaining time is computed as the sum
//    of the current CPU burst’s remaining time plus all future CPU bursts.
// */
// int total_remaining_cpu(Process *p) {
//     int total = p->remaining;
//     for (int j = p->current_job + 1; j < p->num_jobs; j++) {
//         if (p->jobs[j].type == 'C')
//             total += p->jobs[j].duration;
//     }
//     return total;
// }

// void simulate_stcf(Process orig[], int n, int io_aware) {
//     Process procs[MAX_PROCS];
//     memcpy(procs, orig, n * sizeof(Process));
//     int time = 0;
//     int running_index = -1;
//     while (!all_finished(procs, n)) {
//         // Arrivals.
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == NEW && procs[i].arrival_time <= time)
//                 procs[i].state = READY;
//         }
//         // Update BLOCKED.
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == BLOCKED) {
//                 procs[i].remaining--;
//                 if (procs[i].remaining <= 0) {
//                     procs[i].current_job++;
//                     if (procs[i].current_job < procs[i].num_jobs) {
//                         procs[i].remaining = procs[i].jobs[procs[i].current_job].duration;
//                         procs[i].state = READY;
//                     } else {
//                         procs[i].state = FINISHED;
//                         procs[i].finish_time = time;
//                     }
//                 }
//             }
//         }
//         // STCF selection: consider running process as well.
//         int candidate = -1;
//         int best = 1e9;
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == READY || procs[i].state == RUNNING) {
//                 int rem = io_aware ? total_remaining_cpu(&procs[i]) : procs[i].remaining;
//                 if (rem < best) {
//                     best = rem;
//                     candidate = i;
//                 }
//             }
//         }
//         // Preempt if needed.
//         if (candidate != -1) {
//             if (running_index != candidate) {
//                 // If switching, set old running (if any) back to READY.
//                 if (running_index != -1 && procs[running_index].state == RUNNING)
//                     procs[running_index].state = READY;
//                 running_index = candidate;
//                 procs[running_index].state = RUNNING;
//                 if (procs[running_index].first_cpu_time < 0)
//                     procs[running_index].first_cpu_time = time;
//             }
//         }
//         // Run CPU tick.
//         if (running_index != -1 && procs[running_index].state == RUNNING) {
//             procs[running_index].remaining--;
//             if (procs[running_index].remaining <= 0) {
//                 procs[running_index].current_job++;
//                 if (procs[running_index].current_job < procs[running_index].num_jobs) {
//                     procs[running_index].state = BLOCKED;
//                     procs[running_index].remaining = procs[running_index].jobs[procs[running_index].current_job].duration;
//                 } else {
//                     procs[running_index].state = FINISHED;
//                     procs[running_index].finish_time = time + 1;
//                 }
//                 running_index = -1;
//             }
//         }
//         time++;
//     }
//     printf("STCF (%s I/O awareness) Scheduling Results:\n", io_aware ? "with" : "without");
//     for (int i = 0; i < n; i++) {
//         int response = procs[i].first_cpu_time - procs[i].arrival_time;
//         int turnaround = procs[i].finish_time - procs[i].arrival_time;
//         printf("%s: Response Time = %d, Turnaround Time = %d\n",
//                procs[i].name, response, turnaround);
//     }
// }

// /* -------- Round Robin Simulation --------
//    Preemptive with a fixed quantum.
// */
// void simulate_rr(Process orig[], int n, int quantum) {
//     Process procs[MAX_PROCS];
//     memcpy(procs, orig, n * sizeof(Process));
//     // For RR, maintain a simple circular ready queue (indices stored in an array).
//     int ready_queue[MAX_PROCS];
//     int front = 0, rear = 0;
//     int time = 0;
//     int cpu = -1;  // index of process currently running
//     // Initially, processes in NEW are not in ready_queue.
//     while (!all_finished(procs, n)) {
//         // Check arrivals.
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == NEW && procs[i].arrival_time <= time) {
//                 procs[i].state = READY;
//                 ready_queue[rear++] = i;
//             }
//         }
//         // Update BLOCKED (I/O).
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == BLOCKED) {
//                 procs[i].remaining--;
//                 if (procs[i].remaining <= 0) {
//                     procs[i].current_job++;
//                     if (procs[i].current_job < procs[i].num_jobs) {
//                         procs[i].remaining = procs[i].jobs[procs[i].current_job].duration;
//                         procs[i].state = READY;
//                         ready_queue[rear++] = i;
//                     } else {
//                         procs[i].state = FINISHED;
//                         procs[i].finish_time = time;
//                     }
//                 }
//             }
//         }
//         // If CPU idle, fetch from ready_queue.
//         if (cpu == -1 && front < rear) {
//             cpu = ready_queue[front++];
//             procs[cpu].state = RUNNING;
//             procs[cpu].rr_quantum = quantum;
//             if (procs[cpu].first_cpu_time < 0)
//                 procs[cpu].first_cpu_time = time;
//         }
//         // Run CPU tick.
//         if (cpu != -1) {
//             procs[cpu].remaining--;
//             procs[cpu].rr_quantum--;
//             // If CPU burst finished.
//             if (procs[cpu].remaining <= 0) {
//                 procs[cpu].current_job++;
//                 if (procs[cpu].current_job < procs[cpu].num_jobs) {
//                     procs[cpu].state = BLOCKED;
//                     procs[cpu].remaining = procs[cpu].jobs[procs[cpu].current_job].duration;
//                 } else {
//                     procs[cpu].state = FINISHED;
//                     procs[cpu].finish_time = time + 1;
//                 }
//                 cpu = -1;
//             }
//             // If quantum expires but burst not finished, preempt.
//             else if (procs[cpu].rr_quantum <= 0) {
//                 procs[cpu].state = READY;
//                 ready_queue[rear++] = cpu;
//                 cpu = -1;
//             }
//         }
//         time++;
//     }
//     printf("Round Robin (quantum=%d) Scheduling Results:\n", quantum);
//     for (int i = 0; i < n; i++) {
//         int response = procs[i].first_cpu_time - procs[i].arrival_time;
//         int turnaround = procs[i].finish_time - procs[i].arrival_time;
//         printf("%s: Response Time = %d, Turnaround Time = %d\n",
//                procs[i].name, response, turnaround);
//     }
// }

// /* -------- Multi-Level Feedback Queue (MLFQ) Simulation --------
//    A simplified two-level MLFQ is implemented.
//    Level 0: time quantum = 5; Level 1: time quantum = 10.
//    Processes start at level 0. If a process exhausts its quantum without finishing its CPU burst,
//    it is demoted to level 1.
// */
// void simulate_mlfq(Process orig[], int n) {
//     Process procs[MAX_PROCS];
//     memcpy(procs, orig, n * sizeof(Process));
//     int time = 0;
//     // Two ready queues.
//     int queue0[MAX_PROCS], q0f = 0, q0r = 0;
//     int queue1[MAX_PROCS], q1f = 0, q1r = 0;
//     int cpu = -1;
//     int cpu_quantum = 0;
//     while (!all_finished(procs, n)) {
//         // Arrivals.
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == NEW && procs[i].arrival_time <= time) {
//                 procs[i].state = READY;
//                 procs[i].priority = 0;
//                 queue0[q0r++] = i;
//             }
//         }
//         // Update BLOCKED.
//         for (int i = 0; i < n; i++) {
//             if (procs[i].state == BLOCKED) {
//                 procs[i].remaining--;
//                 if (procs[i].remaining <= 0) {
//                     procs[i].current_job++;
//                     if (procs[i].current_job < procs[i].num_jobs) {
//                         procs[i].remaining = procs[i].jobs[procs[i].current_job].duration;
//                         procs[i].state = READY;
//                         // When returning from I/O, boost to higher priority.
//                         procs[i].priority = 0;
//                         queue0[q0r++] = i;
//                     } else {
//                         procs[i].state = FINISHED;
//                         procs[i].finish_time = time;
//                     }
//                 }
//             }
//         }
//         // If CPU idle, select from highest priority queue.
//         if (cpu == -1) {
//             int candidate = -1;
//             if (q0f < q0r) {
//                 candidate = queue0[q0f++];
//             } else if (q1f < q1r) {
//                 candidate = queue1[q1f++];
//             }
//             if (candidate != -1) {
//                 cpu = candidate;
//                 procs[cpu].state = RUNNING;
//                 if (procs[cpu].first_cpu_time < 0)
//                     procs[cpu].first_cpu_time = time;
//                 // Set quantum based on current priority.
//                 cpu_quantum = (procs[cpu].priority == 0) ? 5 : 10;
//             }
//         }
//         // Run CPU.
//         if (cpu != -1) {
//             procs[cpu].remaining--;
//             cpu_quantum--;
//             if (procs[cpu].remaining <= 0) {
//                 procs[cpu].current_job++;
//                 if (procs[cpu].current_job < procs[cpu].num_jobs) {
//                     procs[cpu].state = BLOCKED;
//                     procs[cpu].remaining = procs[cpu].jobs[procs[cpu].current_job].duration;
//                 } else {
//                     procs[cpu].state = FINISHED;
//                     procs[cpu].finish_time = time + 1;
//                 }
//                 cpu = -1;
//             }
//             else if (cpu_quantum <= 0) {
//                 // Demote process if not already at lowest level.
//                 if (procs[cpu].priority == 0)
//                     procs[cpu].priority = 1;
//                 procs[cpu].state = READY;
//                 if (procs[cpu].priority == 0)
//                     queue0[q0r++] = cpu;
//                 else
//                     queue1[q1r++] = cpu;
//                 cpu = -1;
//             }
//         }
//         time++;
//     }
//     printf("MLFQ Scheduling Results:\n");
//     for (int i = 0; i < n; i++) {
//         int response = procs[i].first_cpu_time - procs[i].arrival_time;
//         int turnaround = procs[i].finish_time - procs[i].arrival_time;
//         printf("%s: Response Time = %d, Turnaround Time = %d\n",
//                procs[i].name, response, turnaround);
//     }
// }

// /* -------- Main --------
//    Usage: ./scheduler_sim <input_file> <scheduler>
//    where scheduler is one of: fifo, sjf, stcf, stcfio, rr, mlfq.
// */
// int main(int argc, char *argv[]) {
//     if (argc < 3) {
//         fprintf(stderr, "Usage: %s <input_file> <scheduler>\n", argv[0]);
//         return 1;
//     }
//     Process procs[MAX_PROCS];
//     int n = parse_file(argv[1], procs);
//     if (n <= 0) {
//         fprintf(stderr, "No processes parsed.\n");
//         return 1;
//     }
//     char *sched = argv[2];
//     if (strcmp(sched, "fifo") == 0) {
//         simulate_fifo(procs, n);
//     } else if (strcmp(sched, "sjf") == 0) {
//         simulate_sjf(procs, n);
//     } else if (strcmp(sched, "stcf") == 0) {
//         simulate_stcf(procs, n, 0);
//     } else if (strcmp(sched, "stcfio") == 0) {
//         simulate_stcf(procs, n, 1);
//     } else if (strcmp(sched, "rr") == 0) {
//         simulate_rr(procs, n, 10); // time quantum = 10
//     } else if (strcmp(sched, "mlfq") == 0) {
//         simulate_mlfq(procs, n);
//     } else {
//         fprintf(stderr, "Unknown scheduler: %s\n", sched);
//         return 1;
//     }
//     return 0;
// }



#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <time.h>

#define MAXLINELENGTH 100
#define MAXPROC 100

struct process{
    int arrivaltime;
    char name[10];
    char jobs[100];
    
};
int totalproc=0;
int currproc=0;
struct process plist[MAXPROC];

void fileread(char * filename){
    FILE * file = fopen("input.txt","r");
    char line[MAXLINELENGTH];
    while (fgets(line,sizeof(line),file)){
        if (line[0]=='\n'){
            continue;
        }
        line[strcspn(line,"\n")]='\0';
        char * token = strtok(line, " ");
        strcpy(plist[currproc].name,token);
        token = strtok(NULL, " ");
        plist[currproc].arrivaltime = atoi(token);
        plist[currproc].jobs[0]='\0';
        while ((token=strtok(NULL," "))!=NULL){
            strcat(plist[currproc].jobs,token);
            strcat(plist[currproc].jobs," ");
        }
        currproc++;
    }
    totalproc=currproc;
    fclose(file);   
}

int comparestart(const void *a,const void *b){
    return ((struct process *)a)->arrivaltime - ((struct process *)b)->arrivaltime;  
}

void fifo(){
    printf("FIFOOOOOO===========\n");
    int ctime=0;
    qsort(plist,totalproc,sizeof(struct process),comparestart);
    for (int i = 0; i < totalproc; i++){
        if (plist[i].arrivaltime>ctime){
            ctime = plist[i].arrivaltime;
        }
        printf("JOB RUNNING %s\n", plist[i].name);
        printf("TURNAROUND TIME %d", ctime - plist[i].arrivaltime);
        char jos[MAXLINELENGTH];
        strcpy(jos, plist[i].jobs);
        char * token = strtok(jos, " ");
        while (token){
            int duration = atoi(strtok(NULL," "));
            printf("Running %c  job for duration %d \n",*token,duration);
            ctime+=duration;
            token = strtok(NULL, " ");
        }
        printf("TURN AROUND TIME %d \n", ctime - plist[i].arrivaltime);
    }
}

void generaterandom(){
    totalproc=rand()%10;
    for (int i = 0; i < totalproc; i++){
        sprintf(plist[i].name, "P%d", i);
        plist[i].arrivaltime=rand()%101;
        int numjob = rand()%3+3;
        plist[i].jobs[0]='\0';
        char job='C';
        for (int j = 0; j < numjob; j++){
            if (job=='C'){
                int duration = rand()%20;
                char jb[10];
                sprintf(jb, "%c %d ", 'C', duration);
                strcat(plist[i].jobs, jb);
                job='I';
            }
            else{
                int duration = rand()%20;
                char jb[10];
                sprintf(jb, "%c %d ", 'I', duration);
                strcat(plist[i].jobs, jb);
                job='C';
            }
            
        }
        
    }
    
}

int main(){

    srand(time(NULL));
    generaterandom();
    // fileread("input.txt");
    printf("total jobs -------------------------------------------%d\n", totalproc);
    fifo();
    for (int i = 0; i < totalproc; i++){
        printf("%d ", plist[i].arrivaltime);
        printf("%s\n", plist[i].jobs);
    }
    
    return 0;
}