#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCS 50
#define NAME_LEN 32

typedef enum { READY, RUNNING, WAIT, DEAD, TERMINATED } PState;

typedef struct
{
    int pid;
    char name[NAME_LEN];
    int cpu_total;
    int io_start;
    int io_dur;

    PState state;
    int cpu_done;
    int io_left;
    int time_finished;
    int total_io_used;
    int total_waiting;
    int turnaround;
    char note[32];
} Proc;

typedef struct
{
    int pid;
    int time_to_kill;
} KillEvent;

typedef struct
{
    int items[MAX_PROCS];
    int head, tail, count;
} Queue;

void q_init(Queue *q)
{
    q->head = 0;
    q->tail = -1;
    q->count = 0;
}
int q_empty(Queue *q) { return q->count == 0; }

void q_push(Queue *q, int x)
{
    if (q->count >= MAX_PROCS) return;
    q->tail = (q->tail + 1) % MAX_PROCS;
    q->items[q->tail] = x;
    q->count++;
}

int q_pop(Queue *q)
{
    if (q_empty(q)) return -1;
    int v = q->items[q->head];
    q->head = (q->head + 1) % MAX_PROCS;
    q->count--;
    return v;
}

int q_remove(Queue *q, int x)
{
    int new_arr[MAX_PROCS], newc = 0, removed = 0;
    for (int i = 0; i < q->count; i++)
    {
        int v = q->items[(q->head + i) % MAX_PROCS];
        if (v == x && !removed)
        {
            removed = 1;
            continue;
        }
        new_arr[newc++] = v;
    }
    if (!removed) return 0;

    q->count = newc;
    q->head = 0;
    q->tail = newc - 1;
    for (int i = 0; i < newc; i++)
        q->items[i] = new_arr[i];
    return 1;
}

int q_peek_i(Queue *q, int i)
{
    if (i >= q->count) return -1;
    return q->items[(q->head + i) % MAX_PROCS];
}

int find_pid(Proc p[], int n, int pid)
{
    for (int i = 0; i < n; i++)
        if (p[i].pid == pid)
            return i;
    return -1;
}


/*********** MAIN ***********/
int main()
{
    Proc p[MAX_PROCS];
    KillEvent kills[MAX_PROCS];
    int n, nk;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        printf("\nProcess %d:\n", i + 1);
        printf("PID: ");
        scanf("%d", &p[i].pid);
        printf("Name: ");
        scanf("%s", p[i].name);
        printf("CPU Burst: ");
        scanf("%d", &p[i].cpu_total);
        printf("I/O Start (-1 = NONE): ");
        scanf("%d", &p[i].io_start);
        printf("I/O Duration: ");
        scanf("%d", &p[i].io_dur);

        p[i].state = READY;
        p[i].cpu_done = 0;
        p[i].io_left = 0;
        p[i].time_finished = -1;
        p[i].total_io_used = 0;
        p[i].total_waiting = 0;
        p[i].turnaround = 0;
        strcpy(p[i].note, "OK");
    }

    printf("\nEnter number of kill events: ");
    scanf("%d", &nk);

    for (int i = 0; i < nk; i++)
    {
        printf("Kill %d PID: ", i + 1);
        scanf("%d", &kills[i].pid);
        printf("Kill Time: ");
        scanf("%d", &kills[i].time_to_kill);
    }

    Queue ready, wait;
    q_init(&ready);
    q_init(&wait);

    for (int i = 0; i < n; i++)
        q_push(&ready, i);

    int clock = 0, finished = 0, running = -1;

    printf("\n--- FCFS Simulation ---\n");

    while (finished < n)
    {
        printf("\n[t=%d]\n", clock);

        /******** IO QUEUE ********/
        int wc = wait.count;
        for (int k = 0; k < wc; k++)
        {
            int idx = q_pop(&wait);
            if (p[idx].state == DEAD || p[idx].state == TERMINATED)
                continue;

            p[idx].io_left--;
            if (p[idx].io_left <= 0)
            {
                p[idx].state = READY;
                q_push(&ready, idx);
                printf("\tIO done -> PID %d READY\n", p[idx].pid);
            }
            else
                q_push(&wait, idx);
        }

        /******** KILL EVENTS ********/
        for (int i = 0; i < nk; i++)
        {
            if (kills[i].time_to_kill == clock)
            {
                int idx = find_pid(p, n, kills[i].pid);
                if (idx != -1 && p[idx].state != DEAD && p[idx].state != TERMINATED)
                {
                    p[idx].state = DEAD;
                    p[idx].time_finished = clock;
                    p[idx].turnaround = clock;
                    sprintf(p[idx].note, "KILLED at %d", clock);
                    finished++;

                    if (running == idx)
                        running = -1;

                    q_remove(&ready, idx);
                    q_remove(&wait, idx);

                    printf("\tKILLED PID %d\n", p[idx].pid);
                }
            }
        }

        /******** CPU PICK ********/
        if (running == -1)
        {
            while (!q_empty(&ready))
            {
                int idx = q_pop(&ready);
                if (p[idx].state == READY)
                {
                    running = idx;
                    p[idx].state = RUNNING;
                    printf("\tCPU -> PID %d (%s)\n", p[idx].pid, p[idx].name);
                    break;
                }
            }
        }

        /******** WAIT TIME ********/
        for (int i = 0; i < ready.count; i++)
        {
            int idx = q_peek_i(&ready, i);
            if (idx != -1 && p[idx].state == READY)
                p[idx].total_waiting++;
        }

        /******** CPU RUN ********/
        if (running != -1)
        {
            Proc *r = &p[running];
            r->cpu_done++;

            printf("\tRunning: PID %d %d/%d\n", r->pid, r->cpu_done, r->cpu_total);

            if (r->io_start >= 0 && r->cpu_done == r->io_start && r->io_dur > 0)
            {
                r->state = WAIT;
                r->io_left = r->io_dur;
                r->total_io_used += r->io_dur;
                q_push(&wait, running);
                printf("\tPID %d -> IO WAIT (%d)\n", r->pid, r->io_dur);
                running = -1;
            }

            else if (r->cpu_done >= r->cpu_total)
            {
                r->state = TERMINATED;
                r->time_finished = clock + 1;
                r->turnaround = r->time_finished;
                printf("\tDONE PID %d\n", r->pid);
                finished++;
                running = -1;
            }
        }
        else
            printf("\tCPU idle\n");

        clock++;
    }

    /******** FINAL OUTPUT ********/
    printf("\n--- FINAL TABLE ---\n");
    printf("| PID | Name     | CPU | IO_used | Waiting | Turnaround |    Status     |\n");
    printf("|-----|----------|-----|---------|---------|------------|-------------|\n");

    /* print ALL processes (including killed ones) */
    for (int i = 0; i < n; i++)
    {
        Proc *x = &p[i];
        int waiting = x->turnaround - x->cpu_total;  // as you required (Option B)

        printf("| %3d | %-8s | %3d | %7d | %7d | %10d | %-11s |\n",
               x->pid, x->name, x->cpu_total, x->total_io_used,
               waiting, x->turnaround, x->note);
    }

    return 0;
}
