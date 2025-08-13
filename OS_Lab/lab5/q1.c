#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n, i, j;
    fprintf(stderr,"Enter the number of processes: ");
    scanf("%d", &n);

    srand(time(NULL));

    for (i = 1; i <= n; i++) {
        int start_time = rand() % 101;        // start time between 0 and 100
        int num_jobs = (rand() % 4) + 1;        // random number of jobs between 1 and 4

        printf("P%d %d", i, start_time);

        char job_type = 'C';                  // first job is always a CPU job
        for (j = 0; j < num_jobs; j++) {
            int duration = (rand() % 500) + 1;  // random duration between 1 and 500
            printf(" %c %d", job_type, duration);
            // Alternate the job type: if current is C then next should be I, and vice versa.
            job_type = (job_type == 'C') ? 'I' : 'C';
        }
        printf("\n");
    }
    return 0;
}
