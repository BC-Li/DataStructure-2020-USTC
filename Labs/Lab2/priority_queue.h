#include <stdlib.h>
#include <algorithm>

template <typename T>
struct Priority_Queue
{
private:
    T *data;
    size_t rear;
    size_t capacity;
    int (*cmp)(const T &a, const T &b);

public:
    static void init(struct Priority_Queue &q, size_t capacity, int (*cmp)(const T &a, const T &b));
    static void destroy(struct Priority_Queue &q);
    static bool enqueue(struct Priority_Queue &q, const T &e);
    static bool dequeue(struct Priority_Queue &q, T &e);
    static bool top(const struct Priority_Queue &q, T &e);
    static size_t length(const struct Priority_Queue &q);
    static bool empty(const struct Priority_Queue &q);
    static bool full(const struct Priority_Queue &q);
};

template <typename T>
void Priority_Queue<T>::init(struct Priority_Queue<T> &q, size_t capacity, int (*cmp)(const T &a, const T &b))
{
    q.data = (T*)malloc((capacity + 1)*sizeof(T));
    q.rear = 1;
    q.capacity = capacity;
    q.cmp = cmp;
    return;
}

template <typename T>
void Priority_Queue<T>::destroy(struct Priority_Queue<T> &q)
{
    free(q.data);
    q.rear = 1;
}

template <typename T>
bool Priority_Queue<T>::enqueue(struct Priority_Queue<T> &q, const T &e)
{
    if(full(q) == true){
        return false;
    }
    else{
        q.data[q.rear]=e;
        q.rear ++;
        T min = q.data[1];
        size_t min_recorder = 1;
        for(size_t i = 1;i<=q.rear-1;i++){
            if(q.cmp(q.data[i],min)<0){
                min_recorder = i;
                min = q.data[i];
            }
        }
        T switcher = q.data[min_recorder];
        q.data[min_recorder]=q.data[1];
        q.data[1]=switcher;
        // std::sort(q.data+1,q.data+q.rear);
        return true;
    }
}

template <typename T>
bool Priority_Queue<T>::dequeue(struct Priority_Queue<T> &q, T &e)
{
    if(empty(q)){
        return false;
    }
    e = q.data[1];
    q.data[1] = q.data[q.rear-1];
    q.rear --;
    T min = q.data[1];
    size_t min_recorder = 1;
    for(size_t i = 1;i<=q.rear-1;i++){
            if(q.cmp(q.data[i],min)<0){
                min_recorder = i;
                min = q.data[i];
            }
        }
        T switcher = q.data[min_recorder];
        q.data[min_recorder]=q.data[1];
        q.data[1]=switcher;
    return true;
}

template <typename T>
bool Priority_Queue<T>::top(const struct Priority_Queue<T> &q, T &e)
{
    if (empty(q))
        return false;

    e = q.data[1];
    return true;
}

template <typename T>
size_t Priority_Queue<T>::length(const struct Priority_Queue<T> &q)
{
    return q.rear - 1;
}

template <typename T>
bool Priority_Queue<T>::empty(const struct Priority_Queue<T> &q)
{
    if(q.rear == 1){
        return true;
    }
    return false;
}

template <typename T>
bool Priority_Queue<T>::full(const struct Priority_Queue<T> &q)
{
    if(length(q)==q.capacity){
        return true;
    }
    return false;
}
