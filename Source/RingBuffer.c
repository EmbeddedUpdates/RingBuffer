/**
 * 
* @file "RingBuffer.c"
* @author Madrick3
* @brief Basic implementation of a Ring Buffer
* 
* @COMPONENT: RingBuffer
* @VERSION: 01.00.00
*/
/************************************************************
  Version History
  -----------------------------------------------------------
  Revision |  Author   |  Change ID      |  Description
  01.00.00 |  pflaherty|  --             |  Initial Creation
************************************************************/

/************************************************************
  DEFINES
************************************************************/

/************************************************************
  INCLUDES
************************************************************/
#include "RingBuffer.h"

/************************************************************
  ENUMS AND TYPEDEFS
************************************************************/

/************************************************************
  LOCAL VARIABLES
************************************************************/
/************************************************************
  LOCAL FUNCTIONS
************************************************************/
static void RingBuffer_ClearMemPool(void)
{
  uint8 * addr;
  /* Clear all of the memory in the RingBuffer memory pool */
  for( addr = (uint8 *)RINGBUFFER_MEMPOOL_STARTADDR ; \
    addr < (uint8*)(RINGBUFFER_MEMPOOL_STARTADDR + RINGBUFFER_MEMPOOL_SIZE); addr++)
  {
    *addr = 0;
  }
}
/************************************************************
  GLOBAL FUNCTIONS
************************************************************/

/**
 * RingBuffer_Create
 * Returns a pointer to a 'new' RingBuffer structure. We do not utilize the heap, since we want this to
 * be memory safe - so we will not utilize Malloc either. We will statically allocate the memory space at link time,
 * and dynamically assign the data within that allocated region during runtime.
 * 
 * @param self: ringBuffer to store the new data in.
 * @param elementSize: size of the elements that will be placed in the ringbuffer.
 * 
 */
Std_ErrorCode RingBuffer_Create( RingBuffer * self, RINGBUFFER_SIZE_TYPE elementSize)
{
  (void) self;
  Std_ErrorCode retVal = E_OK;
  /* ElementSize must be a power of two (or 1) (for runtime efficiency when we implement binary search)*/
  if(elementSize % 2 != 0 && elementSize != 1)
  {
    retVal = E_NOT_OK;
  }
  /* elements can not take up only zero space, and fitting less than 2 elements in the buffer is a dumb idea. */
  if(elementSize == 0 || elementSize > RINGBUFFER_MEMPOOL_SIZE/2)
  {
    retVal = E_NOT_OK;
  }

  if(E_OK == retVal)
  {
    /* Lets set the capacity for the RingBuffer */
    self->capacity = RINGBUFFER_MEMPOOL_SIZE/elementSize;

    /* All of mempool is allocated for one RingBuffer currently. Will change in the future. */
    self->buffer = (uint8 *)RINGBUFFER_MEMPOOL_STARTADDR;

    /* Clear all of the memory in the RingBuffer memory pool */
    RingBuffer_ClearMemPool();
  }

  return retVal;
}
