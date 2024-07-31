#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#include "myQueue.h"

typedef struct node
{
    myQueue_buff_t* buffer;

    struct node* prev;
    struct node* next;
}node_t;

static node_t* head = NULL;

static uint32_t _generate_sequence(void) {
    static uint32_t sec = 0;
    return sec++;
}
static myQueue_buff_t* _alloc_buff(int size) {
    void* dataPtr = malloc(size * sizeof(myQueueData_t));
    myQueue_buff_t* buffPtr = malloc(sizeof(myQueue_buff_t));

    if (dataPtr == NULL || buffPtr == NULL) {
        MYQUEUE_ASSERT("_alloc_buff malloc failure");
        free(dataPtr);
        free(buffPtr);
        return NULL;
    }

    buffPtr->dataPtr = dataPtr;
    buffPtr->dataSize = size;
    buffPtr->seq = _generate_sequence();

    return buffPtr;
}
static void _free_buff(myQueue_buff_t* ptr) {
    free(ptr->dataPtr);
    free(ptr);
}

static node_t* _alloc_node(int bufSize) {
    node_t* nodePtr = malloc(sizeof(node_t));

    if (nodePtr == NULL) {
        MYQUEUE_ASSERT("_alloc_node failure");
        return NULL;
    }

    myQueue_buff_t* bufPtr = _alloc_buff(bufSize);

    if (bufPtr == NULL) {
        return NULL;
    }
    nodePtr->buffer = bufPtr;
    nodePtr->prev = NULL;
    nodePtr->next = NULL;

    return nodePtr;
}

static void _free_node(node_t* ptr) {
    _free_buff(ptr->buffer);
    free(ptr);
}
static node_t* _getTail()
{
    node_t* tail = head;

    if (tail != NULL) {
        while (tail->next != NULL) {
            tail = tail->next;
        }
    }

    return tail;
}

void myQueue_clear(void)
{
    node_t* base = head;
    while (base->next != NULL) {
        node_t* ptr = base->next;
        base->next = ptr->next;
        _free_node(ptr);
        ptr = NULL;
    }

    _free_node(head);
    head = NULL;
}
void myQueue_push(myQueueData_t* buffer, uint32_t size)
{
    node_t* newnode = _alloc_node(size);
        
    if (newnode == NULL) {
        // throw

        return;
    }

    memcpy(newnode->buffer->dataPtr, buffer, size);

    node_t* tail = _getTail();
    if (tail == NULL) {
        head = newnode;
    }
    else {
        tail->next = newnode;
        newnode->prev = tail;
    }
}
/**
 * @brief pop head. free buffer memory after it's usage by calling myQueue_free()
 */
myQueue_buff_t* myQueue_pop(void)
{
    myQueue_buff_t* pop = NULL;

    if (head != NULL) {
        pop = head->buffer;

        // assign new head and free head memory (do not free databuffer)
        if (head->next != NULL) {
            node_t* newhead = head->next;
            newhead->prev = NULL;
            free(head);
            head = newhead;
        }
        else {
            free(head);
            head = NULL;
        }

    }

    return pop;
}

void myQueue_free(myQueue_buff_t* ptr)
{
    _free_buff(ptr);
}
