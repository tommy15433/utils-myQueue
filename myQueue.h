#pragma once

#include "stdbool.h"
#include "stdint.h"

/**
 * 
 */
#define MYQUEUE_ASSERT(X)	;
/**
 * typedef
*/
typedef uint16_t serverlist_addr;
typedef uint8_t myQueueData_t;

typedef struct {
	uint32_t seq;
	uint32_t dataSize;
	myQueueData_t* dataPtr;

} myQueue_buff_t;

void myQueue_clear(void);
void myQueue_push(myQueueData_t* buffer, uint32_t size);
/**
 * @brief
 * 
 * @return returns queue buffer. Free it by calling myQueue_free() after use.
 */
myQueue_buff_t* myQueue_pop(void);
/**
 * 
 */
void myQueue_free(myQueue_buff_t* ptr);
