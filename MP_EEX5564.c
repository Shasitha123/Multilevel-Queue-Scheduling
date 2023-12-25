#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100
#define AGE_THRESHOLD 5 // Ageing threshold

typedef struct {
    int process_id;
    int burst_time;
    int priority;
    int waiting_time; // Used for ageing algorithm
} Process;

typedef struct {
    Process queue[MAX_QUEUE_SIZE];
    int front, rear;
} Queue;

void enqueue(Queue *q, Process p) {
    if ((q->rear + 1) % MAX_QUEUE_SIZE == q->front) {
        printf("Queue overflow\n");
        exit(1);
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->queue[q->rear] = p;
}

Process dequeue(Queue *q) {
    if (q->front == q->rear) {
        printf("Queue underflow\n");
        exit(1);
    }
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->queue[q->front];
}

void executeProcess(Process p) {
    printf("Executing process %d with burst time %d and priority %d\n", p.process_id, p.burst_time, p.priority);
}

void aging(Queue *q) {
    for (int i = q->front + 1; i <= q->rear; ++i) {
        q->queue[i].waiting_time++;
        if (q->queue[i].waiting_time >= AGE_THRESHOLD) {
            // Increase priority by moving to a higher priority queue
            enqueue(q - 1, dequeue(q));
        }
    }
}

int main() {
    Queue highPriorityQueue = {{}, 0, 0};
    Queue mediumPriorityQueue = {{}, 0, 0};
    Queue lowPriorityQueue = {{}, 0, 0};

    int totalProcesses;
    printf("Enter the total number of processes: ");
    scanf("%d", &totalProcesses);

    for (int i = 0; i < totalProcesses; ++i) {
        Process p;
        p.process_id = i + 1;
        printf("Enter burst time for process %d: ", p.process_id);
        scanf("%d", &p.burst_time);
        printf("Enter priority for process %d: ", p.process_id);
        scanf("%d", &p.priority);
        p.waiting_time = 0; // Initialize waiting time for ageing algorithm

        // Assign processes to queues based on priority
        if (p.priority >= 7) {
            enqueue(&highPriorityQueue, p);
        } else if (p.priority >= 4) {
            enqueue(&mediumPriorityQueue, p);
        } else {
            enqueue(&lowPriorityQueue, p);
        }
    }

    printf("\nExecuting processes:\n");

    while (highPriorityQueue.front != highPriorityQueue.rear ||
           mediumPriorityQueue.front != mediumPriorityQueue.rear ||
           lowPriorityQueue.front != lowPriorityQueue.rear) {

        if (highPriorityQueue.front != highPriorityQueue.rear) {
            Process p = dequeue(&highPriorityQueue);
            executeProcess(p);
            aging(&mediumPriorityQueue);
            aging(&lowPriorityQueue);
        } else if (mediumPriorityQueue.front != mediumPriorityQueue.rear) {
            Process p = dequeue(&mediumPriorityQueue);
            executeProcess(p);
            aging(&lowPriorityQueue);
        } else {
            // Process from low priority queue
            Process p = dequeue(&lowPriorityQueue);
            executeProcess(p);
        }
    }

    return 0;
}
