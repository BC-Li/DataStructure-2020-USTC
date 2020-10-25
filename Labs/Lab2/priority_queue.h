#include <stdlib.h>

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
        int temp = q.rear;
        q.data[q.rear] = e;
        while(q.cmp(q.data[temp],q.data[temp/2])>0){
            T c = q.data[temp];
            q.data[temp] = q.data[temp/2];
            q.data[temp/2] = c;
            temp = temp / 2;
        }
        q.rear ++;
        return true;
    }
}

template <typename T>
bool Priority_Queue<T>::dequeue(struct Priority_Queue<T> &q, T &e)
{
    e = q.data[1];
    q.data[1] = q.data[q.rear];
    for(int i = 1;i<q.rear;i++) {
        for (int j = i; j < q.rear; j++) {
            if (q.cmp(q.data[i], q.data[j]) > 0) {
                T c = q.data[i];
                q.data[i] = q.data[i + 1];
                q.data[i + 1] = c;
            }
        }
    }
    return true;
}

template <typename T>
bool Priority_Queue<T>::top(const struct Priority_Queue<T> &q, T &e)
{
    if(q.data[1]){
        e = q.data[1];
        return true;
    }
    else {
        return false;
    }
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
    if(q.rear == q.capacity + 1){
        return true;
    }
    return false;
}
