#include <stdlib.h>

template <typename T>
struct Queue
{
private:
    T *data;
    size_t front;
    size_t rear;
    size_t capacity;

public:
    static void init(struct Queue &q, size_t capacity);
    static void destroy(struct Queue &q);
    static bool enqueue(struct Queue &q, const T &e);
    static bool dequeue(struct Queue &q, T &e);
    static bool top(const struct Queue &q, T &e);
    static size_t length(const struct Queue &q);
    static bool empty(const struct Queue &q);
    static bool full(const struct Queue &q);
};

template <typename T>
void Queue<T>::init(struct Queue<T> &q, size_t capacity)
{
    q.data = (T*)malloc((capacity+1)*sizeof(T));
    q.front = 0;
    q.rear = 1;
    return;
}

template <typename T>
void Queue<T>::destroy(struct Queue<T> &q)
{
    free(q.data);
    q.rear = 0;
    q.front = 0;
}

template <typename T>
bool Queue<T>::enqueue(struct Queue<T> &q, const T &e)
{
    if(full(q) == true){
        return false;
    }
    else{
        q.data[q.rear] = e;
        q.rear = (q.rear + 1) % q.capacity;
        return true;
    }
}

template <typename T>
bool Queue<T>::dequeue(struct Queue<T> &q, T &e)
{
    if(empty(q) == true){
        return false;
    }
    else{
        e = q.data[q.front];
        q.front = (q.front+1)%q.capacity;
        return true;
    }
}

template <typename T>
bool Queue<T>::top(const struct Queue<T> &q, T &e)
{
    if (empty(q))
        return false;

    e = q.data[q.front];
    return true;
}

template <typename T>
size_t Queue<T>::length(const struct Queue<T> &q)
{
    return (q.rear-q.front+q.capacity)%q.capacity;
}

template <typename T>
bool Queue<T>::empty(const struct Queue<T> &q)
{
    if(q.front == q.rear){
        return true;
    }
    return false;
}

template <typename T>
bool Queue<T>::full(const struct Queue<T> &q)
{
    if(length(q) == q.capacity){
        return true;
    }
    return false;
}
