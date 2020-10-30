#include "util/util.h"
#include "queue.h"
#include "priority_queue.h"

int customer_compare(const Customer &a, const Customer &b) {
    if (a.leave_time > b.leave_time) {
        return 1;
    }
    else if (a.leave_time < b.leave_time) {
        return -1;
    }
    else {
        return 0;
    }
}

double simulate(const size_t queue_capacity, const size_t num_servers, Queue<Customer> &arrival_flow)
{
    Queue<Customer> queue;
    Priority_Queue<Customer> leave_flow;

    int num_customers = 0;
    double total_queue_time = 0;

    // TODO: Initalize queue and leave_flow
    Queue<Customer>::init(queue,queue_capacity);
    Priority_Queue<Customer>::init(leave_flow,num_servers,customer_compare);

    while (!Queue<Customer>::empty(arrival_flow) || !Queue<Customer>::empty(queue) || !Priority_Queue<Customer>::empty(leave_flow))
    {
        double current_time = 0;
        
        Customer customer_to_arrive,customer_to_leave;
        // TODO: Get the next arriving and leaving customer
        Queue<Customer>::top(arrival_flow,customer_to_arrive);
        Priority_Queue<Customer>::top(leave_flow,customer_to_leave);
        //TODO: Check if a customer arrives before a customer leaves (Beware of empty)

        if(!Queue<Customer>::empty(arrival_flow) && Priority_Queue<Customer>::empty(leave_flow)){
                Queue<Customer>::dequeue(arrival_flow,customer_to_arrive);
                
                if(!Queue<Customer>::full(queue)){
                    Queue<Customer>::enqueue(queue,customer_to_arrive); 
                }
                current_time = customer_to_arrive.arrive_time;
            }
       
       
        if(!Queue<Customer>::empty(arrival_flow) && !Priority_Queue<Customer>::empty(leave_flow) && (customer_to_arrive.arrive_time<=customer_to_leave.leave_time))
        {
                Queue<Customer>::dequeue(arrival_flow,customer_to_arrive);
                if(!Queue<Customer>::full(queue)){
                    Queue<Customer>::enqueue(queue,customer_to_arrive); 
                }
                current_time = customer_to_arrive.arrive_time;
        }

        // // //TODO: Check if a customer leaves before a customer arrives (Beware of empty)
        if(Queue<Customer>::empty(arrival_flow) && (!Priority_Queue<Customer>::empty(leave_flow)))
        {
            //TODO: A customer leaves
            Priority_Queue<Customer>::dequeue(leave_flow,customer_to_leave);
            current_time = customer_to_leave.leave_time;
        }
        if(!Queue<Customer>::empty(arrival_flow) && !Priority_Queue<Customer>::empty(leave_flow)&& (customer_to_arrive.arrive_time>customer_to_leave.leave_time))
        {
            //TODO: A customer leaves
            Priority_Queue<Customer>::dequeue(leave_flow,customer_to_leave);
            current_time = customer_to_leave.leave_time;
        }

        // A customer is served only after it arrives or a server is available thanks to leaving of another customer
        if(!Queue<Customer>::empty(queue) && !Priority_Queue<Customer>::full(leave_flow))
        {
            Customer customer;
            // TODO: A customer stops waiting in the queue
            // TODO: Remember to set the leaving time of this customer
            Queue<Customer>::dequeue(queue,customer);
            customer.leave_time = current_time + customer.service_time;
            Priority_Queue<Customer>::enqueue(leave_flow,customer);
            // time waiting in queue
            double queue_time = current_time - customer.arrive_time;
            num_customers++;
            total_queue_time += queue_time;
        }
    }

    Queue<Customer>::destroy(arrival_flow);
    Queue<Customer>::destroy(queue);
    Priority_Queue<Customer>::destroy(leave_flow);

    return total_queue_time / num_customers;
}
