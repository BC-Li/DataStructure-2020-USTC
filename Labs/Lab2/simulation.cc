#include "util/util.h"

#include "queue.h"
#include "priority_queue.h"

// TODO: Comparison function for struct Customer

double simulate(const size_t queue_capacity, const size_t num_servers, Queue<Customer> &arrival_flow)
{
    Queue<Customer> queue;
    Priority_Queue<Customer> leave_flow;

    int num_customers = 0;
    double total_queue_time = 0;

    // TODO: Initalize queue and leave_flow

    while (!Queue<Customer>::empty(arrival_flow) || !Queue<Customer>::empty(queue) || !Priority_Queue<Customer>::empty(leave_flow))
    {
        /* TODO: Simulate
        *  1. Customers arrive and wait in the queue
        *  2. Customers are served
        *  3. Customers leave
        */
    }

    Queue<Customer>::destroy(arrival_flow);
    Queue<Customer>::destroy(queue);
    Priority_Queue<Customer>::destroy(leave_flow);

    return total_queue_time / num_customers;
}
