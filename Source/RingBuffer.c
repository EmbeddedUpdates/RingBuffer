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
  LOCAL DEFINES
************************************************************/
/* This cast is used often in the static functions, use this locally in this TU only.*/
#define SELF ((RingBuffer *)(self))

/************************************************************
  ENUMS AND TYPEDEFS
************************************************************/

/************************************************************
  LOCAL VARIABLES
************************************************************/
<<<<<<< HEAD
/************************************************************
  LOCAL FUNCTIONS
************************************************************/
static void RingBuffer_ClearMemPool(void)
=======

/************************************************************
  LOCAL FUNCTIONS
************************************************************/
static void RingBuffer_MemPool_ClearAll(void)
>>>>>>> c06be56 (trunk commit before refactor for mempool changes)
{
  uint8 * addr;
  /* Clear all of the memory in the RingBuffer memory pool */
  for( addr = (uint8 *)RINGBUFFER_MEMPOOL_STARTADDR ; \
    addr < (uint8*)(RINGBUFFER_MEMPOOL_STARTADDR + RINGBUFFER_MEMPOOL_SIZE); addr++)
  {
    *addr = 0;
  }
}

/**
<<<<<<< HEAD
=======
 * RingBuffer_MemPool_Allocate()
 * 
 * Reserve and allocate space from the memorypool that can fit the elementSize*numBufferslots in bytes.
 * 
 * @param rb (in/out): ringbuffer that will have it's buffer assigned by this function.
 * @param elementSize (in): size in bytes of the elements that will be stored in the RingBuffer/Mempool
 * @param numBufferSlots (in): number of elements that will be stored in the RingBuffer
 * 
 */
static Std_ErrorCode RingBuffer_MemPool_Allocate(RingBuffer * rb, RINGBUFFER_SIZE_TYPE elementSize, RINGBUFFER_SIZE_TYPE numBufferSlots)
{
  Std_ErrorCode retVal = E_OK;
  return retVal;
}


/**
>>>>>>> c06be56 (trunk commit before refactor for mempool changes)
 * RingBuffer_Write()
 * Adds the given data to the given RingBuffer. If the size is too big for the RingBuffer slots, then we will return NOTOK.
 * Static: because it must be called through the RingBuffer itself
 *  - NB: RB_OPT_001
 * 
 */
static Std_ErrorCode RingBuffer_Write(void * self, uint8 * dataBuffer, RINGBUFFER_SIZE_TYPE size)
{
  Std_ErrorCode retVal = E_OK;
  RINGBUFFER_SIZE_TYPE i; /* idx for access to ringbuffer entry */

  /* Check input parameters */
  if( (NULL == SELF) || (size > SELF->elementSize)  || (NULL == dataBuffer) )
  {
    retVal = E_NOT_OK;
  }

  if(SELF->capacity == SELF->count)
  {
    retVal = E_NOT_OK;
  }
  
  /* if all checks were succesfull, we can queue the data in the ringbuffer */
  if(E_OK == retVal)
  {
    SELF->count++;
    for(i = 0; i < size; i++)
    {
      *(SELF->buffer + SELF->tail + i) = dataBuffer[i];
    }
    SELF->tail = ((SELF->tail + SELF->elementSize) % (SELF->capacity * SELF->elementSize));

  }

  return retVal;
}

/**
 * NB: RB_OPT_001
 */
static Std_ErrorCode RingBuffer_Read(void * self, uint8 * dataBuffer, RINGBUFFER_SIZE_TYPE size)
{
  Std_ErrorCode retVal = E_OK;
  RINGBUFFER_SIZE_TYPE i; /* idx for access to ringbuffer entry */

  /* Check input parameters */
  if( (NULL == SELF) || (size > SELF->elementSize)  || (NULL == dataBuffer) )
  {
    retVal = E_NOT_OK;
  }

  /* in the case where the RB is empty, we should reply NOT_OK */
  if(0 == SELF->count)
  {
    retVal = E_NOT_OK;
  }
  
  /* If all checks were successful, then we can dequeue the data. */
  if(E_OK == retVal)
  {
    SELF->count--;
    for(i = 0; i < size; i++)
    {
      dataBuffer[i] = *(SELF->buffer + SELF->head + i);
    }

    SELF->head = ((SELF->head + SELF->elementSize) % (SELF->capacity * SELF->elementSize));

  }

  return retVal;
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
<<<<<<< HEAD
 * 
 */
Std_ErrorCode RingBuffer_Create( RingBuffer * self, RINGBUFFER_SIZE_TYPE elementSize)
{
  Std_ErrorCode retVal = E_OK;

  if( NULL == self )
=======
 * @param numBufferSlots: number of slots that the buffer must hold at one time.
 * 
 */
Std_ErrorCode RingBuffer_Create( RingBuffer * self, RINGBUFFER_SIZE_TYPE elementSize, RINGBUFFER_SIZE_TYPE numBufferSlots)
{
  Std_ErrorCode retVal = E_OK;
  (void) numBufferSlots;

  /* Param checks are performed first*/
  /* 
    1. Self must not be null
    2. ElementSize should be a power of 2 (i.e. 1, 2, 4, 8, 16) for memory fit.
    3. numBufferSlots should not be 0, and should not request more than is allocated by linker
  */
  if( ( NULL == self )                                                    || 
      (elementSize == 0) || (elementSize % 2 != 0 && elementSize != 1)    || 
      (numBufferSlots == 0 )|| (numBufferSlots > RINGBUFFER_MEMPOOL_SIZE) )
>>>>>>> c06be56 (trunk commit before refactor for mempool changes)
  {
    retVal = E_NOT_OK;
  }

<<<<<<< HEAD
  /* ElementSize must be a power of two (or 1) (for runtime efficiency when we implement binary search)*/
  if(elementSize % 2 != 0 && elementSize != 1)
  {
    retVal = E_NOT_OK;
  }
  /* elements can not take up only zero space, and fitting less than 2 elements in the buffer is a dumb idea. */
  if(elementSize == 0 || elementSize > RINGBUFFER_MEMPOOL_SIZE/2)
=======
  /* We must determine if there is enough space left in the memory pool */
  /* We should request space from the memory pool */
  /* elements can not take up only zero space, and fitting less than 2 elements in the buffer is a dumb idea. */
  if(elementSize > RINGBUFFER_MEMPOOL_SIZE/2)
>>>>>>> c06be56 (trunk commit before refactor for mempool changes)
  {
    retVal = E_NOT_OK;
  }

  if(E_OK == retVal)
  {
    /* Lets set the capacity for the RingBuffer */
    self->capacity = RINGBUFFER_MEMPOOL_SIZE/elementSize;
    self->elementSize = elementSize; 
    self->count = 0;

    /* All of mempool is allocated for one RingBuffer currently. Will change in the future. */
    self->buffer = (uint8 *)RINGBUFFER_MEMPOOL_STARTADDR;
    self->head = 0;
    self->tail = 0;

    /* Assign function pointers */
    self->write = RingBuffer_Write;
    self->read = RingBuffer_Read;
  }

  /* 
    This is broken out to a different if condition due to the importance of the RingBuffer_ClearMemPool function call. 
    If RingBuffer_ClearMemPool is called AND we want to have multiple RingBuffers using the same mempool, then we would
    have a problem with the below line. In the future, it is likely necessary to add a new retVal assignment above, and
    consider full or partial clear of the mempool below later.
  */
  if(E_OK == retVal)
  {
    /* Clear all of the memory in the RingBuffer memory pool */
<<<<<<< HEAD
    RingBuffer_ClearMemPool();
=======
    RingBuffer_MemPool_ClearAll();
>>>>>>> c06be56 (trunk commit before refactor for mempool changes)
  }

  return retVal;
}


/* NBs */
/**
 * RB_OPT_001
 * We should consider a function-like macro to hide the passing of 'self', so that users can not pass in the wrong RB.
 *            #define write(y,z) write() <-- This won't work.
 *            #define CALL(obj, method, ...) (obj)->method(obj, ##__VA_ARGS__)
 *          Maybe the above is pretty enough to avoid mistakes by users, hard to say. Will probably mess up all peek-hinting
 *
 */

/**
 * RB_OPT_002
 * SELF macro is used for local functions ("read" and "write") to simplify access to self object/struct.
 *      #define SELF ((RingBuffer *)(self))
 *      This is defined in this file only. No includes are permitted after this line is defined at the top of this file. 
 */
