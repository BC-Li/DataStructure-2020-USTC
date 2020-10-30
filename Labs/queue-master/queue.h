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
    // TODO
}

template <typename T>
void Queue<T>::destroy(struct Queue<T> &q)
{
    // TODO
}

template <typename T>
bool Queue<T>::enqueue(struct Queue<T> &q, const T &e)
{
    // TODO
    return false;
}

template <typename T>
bool Queue<T>::dequeue(struct Queue<T> &q, T &e)
{
    // TODO
    return false;
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
    // TODO
    return 0;
}

template <typename T>
bool Queue<T>::empty(const struct Queue<T> &q)
{
    // TODO
    return true;
}

template <typename T>
bool Queue<T>::full(const struct Queue<T> &q)
{
    // TODO
    return false;
}
