
#ifdef queue
typedef struct queueElement{
	char packet[255];
	circ_queue_el * next;
}circ_queue_el;

typedef struct queue{
	circ_queue_el * last;
	circ_queue_el * top;
	int max_size;
	int curr_size;
}circ_queue;
void initQueue(circ_queue* queue, int size)
{
	queue->top = malloc(sizeof(circ_queue_el));
	circ_queue_el* curr = queue->top;
	for (int i = 0;  i < size-1;i++) {
		curr->next = malloc(sizeof(circ_queue_el));
		curr = curr->next;
	}
	curr->next = queue->top;
	queue->top = curr;

}
void sendElement()
{

}
#endif
