
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>

/* Remember to initilize the queue before using it */
void initialize_queue(struct pqueue_t * q) {
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t * q) {
	return (q->head == NULL);
}

/* Get PCB of a process from the queue (q).
 * Return NULL if the queue is empty */
struct pcb_t * de_queue(struct pqueue_t * q) {
	struct pcb_t * proc = NULL;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition
	
	// YOUR CODE HERE
	pthread_mutex_lock(&q->lock);
	if (!empty(q)){
		proc = q->head->data;
		struct qitem_t* temp = q->head->next;
		free(q->head);
		q->head = temp;
		if (q->head == NULL) q->tail = NULL;
	}
	//printf("de_queue test: %d %d \n", proc->arrival_time, proc->burst_time);
	pthread_mutex_unlock(&q->lock);
	return proc;
}

/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t * q, struct pcb_t * proc) {
	// TODO: Update new process
	// Remember to use 'lock' to avoid race condition
	// YOUR CODE HERE
	pthread_mutex_lock(&q->lock);
	struct qitem_t * newItem = (struct qitem_t *)malloc(sizeof(struct qitem_t));
	newItem->data = proc;
	if (newItem->data->activate == 0){ // if the priority does not matter (for the in_queue)
		if (!empty(q))
			q->tail->next = newItem;
		else q->head = newItem;
		q->tail = newItem;
	} else { // if the priority matters (for the ready_queue)
		if (!empty(q)){
			struct qitem_t * temp = q->head; // take the head and prev node for looping
			struct qitem_t * prev = NULL;
			while(temp != NULL){ // while the final node is not reached
				// if the priority of the current node is higher or equal to the new node
				if (temp->data->priority <= newItem->data->priority){ 
					prev = temp;
					temp = temp->next;
				} else break; // if the priority of the current node is lower than the new node
			}

			if (temp == q->head){ // if we have to insert to the head
				newItem->next = q->head; // set the head to be the newItem
				q->head = newItem;
			} else { // if we insert to a middle of the queue
				prev->next = newItem; 
				newItem->next = temp;
				if (prev == q->tail) q->tail = newItem; // if the node is the tail, set the new tail to be the newItem
			}
		} else { // if the queue is empty
			q->head = newItem;
			q->tail = newItem;
		}
	}
	pthread_mutex_unlock(&q->lock);
}

void print_queue(struct pqueue_t * q){
	struct qitem_t * temp = q->head;
	while(temp != NULL){
		printf("queue item: %d %d %d \n", temp->data->arrival_time, temp->data->burst_time, temp->data->priority);
		temp = temp->next;
	}
}


