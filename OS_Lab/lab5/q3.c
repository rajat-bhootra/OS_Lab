#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TICKS 100    // Total simulation time (ticks)
#define STRIDE_CONST 10000.0 // Large constant for stride scheduling

// Process structure holds ID, tickets, allocated ticks, stride and pass value.
typedef struct {
    int id;         // Process ID (1...n)
    int tickets;    // Number of tickets (wi)
    int allocated;  // Number of ticks allocated by the scheduler
    double stride;  // Computed stride value (for stride scheduling)
    double pass;    // Current pass value (for stride scheduling)
} Process;

/* 
 * Lottery scheduling simulation:
 * At each tick, a random lottery is drawn between 1 and total tickets.
 * The process whose cumulative ticket count reaches or exceeds the lottery number wins.
 */
void simulate_lottery(Process procs[], int n) {
    int total_tickets = 0;
    for (int i = 0; i < n; i++) {
        total_tickets += procs[i].tickets;
    }
    for (int tick = 0; tick < TICKS; tick++) {
        int lottery = rand() % total_tickets + 1; // lottery number in [1, total_tickets]
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += procs[i].tickets;
            if (lottery <= sum) {
                procs[i].allocated++;
                break;
            }
        }
    }
}

/*
 * Stride scheduling simulation:
 * Each process is given a stride = STRIDE_CONST / tickets.
 * At each tick, the process with the smallest pass value is selected,
 * then its pass is incremented by its stride.
 */
void simulate_stride(Process procs[], int n) {
    // Initialize stride and pass for each process.
    for (int i = 0; i < n; i++) {
        procs[i].stride = STRIDE_CONST / procs[i].tickets;
        procs[i].pass = 0.0;
    }
    for (int tick = 0; tick < TICKS; tick++) {
        int min_idx = 0;
        // Find the process with the minimum pass value.
        for (int i = 1; i < n; i++) {
            if (procs[i].pass < procs[min_idx].pass)
                min_idx = i;
        }
        procs[min_idx].allocated++;
        procs[min_idx].pass += procs[min_idx].stride;
    }
}

/*
 * Main program:
 * Reads the number of processes and their ticket counts.
 * Then, based on the command-line argument, runs either lottery or stride scheduling.
 * Finally, it prints each process's allocated ticks, expected ticks, and the difference.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <lottery|stride>\n", argv[0]);
        return 1;
    }
    char *scheduler = argv[1];

    int n;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid number of processes.\n");
        return 1;
    }

    // Allocate process array.
    Process *procs = (Process *)malloc(n * sizeof(Process));
    if (!procs) {
        perror("malloc");
        return 1;
    }

    // Read ticket counts for each process.
    for (int i = 0; i < n; i++) {
        procs[i].id = i + 1;
        printf("Enter number of tickets for process %d: ", i + 1);
        if (scanf("%d", &procs[i].tickets) != 1 || procs[i].tickets <= 0) {
            fprintf(stderr, "Invalid ticket count for process %d.\n", i + 1);
            free(procs);
            return 1;
        }
        procs[i].allocated = 0;
        procs[i].stride = 0;
        procs[i].pass = 0.0;
    }

    srand(time(NULL)); // Seed random number generator.

    // Run the selected scheduler.
    if (strcmp(scheduler, "lottery") == 0) {
        simulate_lottery(procs, n);
        printf("\nLottery Scheduling Results (simulation ticks = %d):\n", TICKS);
    } else if (strcmp(scheduler, "stride") == 0) {
        simulate_stride(procs, n);
        printf("\nStride Scheduling Results (simulation ticks = %d):\n", TICKS);
    } else {
        fprintf(stderr, "Unknown scheduler type: %s\n", scheduler);
        free(procs);
        return 1;
    }

    // Calculate total tickets for expected CPU share.
    int total_tickets = 0;
    for (int i = 0; i < n; i++) {
        total_tickets += procs[i].tickets;
    }

    // Print results: actual allocated ticks versus expected ticks.
    for (int i = 0; i < n; i++) {
        double expected = ((double)procs[i].tickets / total_tickets) * TICKS;
        printf("Process %d: Tickets = %d, Allocated = %d, Expected = %.2f, Diff = %.2f\n",
               procs[i].id, procs[i].tickets, procs[i].allocated, expected, procs[i].allocated - expected);
    }

    free(procs);
    return 0;
}
