#ifndef QUEUE_H
#define QUEUE_H

typedef struct qnode QueueNode;
struct qnode {
	void      *elem;  /*<< a pointer to the element  */
	QueueNode *next;  /*<< the next node in the list */
};

/**
 * Enqueue the specified element.
 *
 * @param[in/out]  queue  the queue; <code>*queue == NULL</code> if empty
 * @param[in]      elem   the element to enqueue
 */
void  enqueue(QueueNode **queue, void *elem);

/* Dequeue an element.
 *
 * @param[in/out]  queue  the queue; <code>*queue == NULL</code> if empty
 * @return the element dequeued, or <code>NULL</code> if the queue is empty
 */
void *dequeue(QueueNode **queue);

#endif /* QUEUE_H */
