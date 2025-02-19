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
/** 
 * In the case where the RingBuffer manually handles its own memory space, we need to use this pointer.
 *  NB: Consider defining this variable in this scope to a linkage symbol provided by the linkerscript. 
 *      This should be reconsidered before moving RingBuffer_Independent to an embedded system.
 */
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
int8 * ringbuffer_start;
#elif( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
#else
#endif /* RINGBUFFER_USECASE */

/************************************************************
  LOCAL FUNCTIONS
************************************************************/

#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
/**
 * RingBuffer_MemPool_ClearAll()
 * Local helper function that will clear the Ringbuffer's memory pool in the case where it is handled by the Ringbuffer directly.
 * In RINGBUFFER_USECASE_MEMPOOL, the MEMPOOL component is used instead.
 * DEBT: We should rename this function, as the mempool concept has grown beyond what this function originally intended.
 */
static void RingBuffer_MemPool_ClearAll(void)
{
  uint8 * addr;
  /* Clear all of the memory in the RingBuffer memory pool */
  for( addr = (uint8 *)RINGBUFFER_STARTADDR ; \
    addr < (uint8*)(RINGBUFFER_STARTADDR + RINGBUFFER_SIZE); addr++)
  {
    *addr = 0;
  }
}
#endif
/**
 * RingBuffer_Write()
 * Adds the given data to the given RingBuffer. If the size is too big for the RingBuffer slots, then we will return NOTOK.
 * Static: because it must be called through the RingBuffer itself
 * @param self: pointer to RingBuffer structure that the data will be written to.
 * @param dataBuffer: pointer to the databuffer that should be stored in the RingBuffer. 
 *  - NB: RB_OPT_003
 * @param size: amount of bytes that must be copied from dataBuffer to the RingBuffer.
 * 
 * @return Std_ErrorCode - E_OK if the copy is successful. E_NOT_OK if the copy is not successful.
 *  - NB: RB_OPT_001
 */
static Std_ErrorCode RingBuffer_Write(void * self, int8 * dataBuffer, RINGBUFFER_SIZE_TYPE size)
{
  Std_ErrorCode retVal = E_OK;
  RINGBUFFER_SIZE_TYPE i; /* idx for access to ringbuffer entry */

  /* Check input parameters */
  if( (NULL == SELF) || (size > SELF->elementSize)  || (NULL == dataBuffer) || (SELF->capacity == SELF->count) )
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
 * RingBuffer_Read()
 * Pops the next data off of the Ringbuffer and stores it into the databuffer given. Decrements the count of data in the RingBuffer, and move the head pointer.
 *
 * @param self: pointer to the RingBuffer that is being used. Data will come from the Ringbuffer.
 * @param databuffer: pointer to a buffer that we will store the data in and give it to the caller
 * @param size: number of bytes that will be read from the RingBuffer, should match the elementSize.
 * 
 * @return Std_ErrorCode: E_OK if the read was performed and the databuffer is populated, E_NOT_OK if the read is not performed.
 * NB: RB_OPT_001
 */
static Std_ErrorCode RingBuffer_Read(void * self, int8 * dataBuffer, RINGBUFFER_SIZE_TYPE size)
{
  Std_ErrorCode retVal = E_OK;
  RINGBUFFER_SIZE_TYPE i; /* idx for access to ringbuffer entry */

  /* Check input parameters */
  if( (NULL == SELF) || (size != SELF->elementSize)  || (NULL == dataBuffer) || (0 == SELF->count) )
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
 * @param numBufferSlots: number of slots that the buffer must hold at one time.
 * 
 * @return Std_ErrorCode, E_OK for a successful creation, E_NOT_OK for error.
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
      (numBufferSlots == 0 )|| (numBufferSlots > RINGBUFFER_SIZE) )
  {
    retVal = E_NOT_OK;
  }

  /* We must determine if there is enough space left in the memory pool */
  /* We should request space from the memory pool */
  /* elements can not take up only zero space, and fitting less than 2 elements in the buffer is a dumb idea. */
  if(elementSize > RINGBUFFER_SIZE/2)
  {
    retVal = E_NOT_OK;
  }

  if(E_OK == retVal)
  {
    /* Lets set the capacity for the RingBuffer */
    self->capacity = RINGBUFFER_SIZE/elementSize;
    self->elementSize = elementSize; 
    self->count = 0;

    /* All of mempool is allocated for one RingBuffer currently. Will change in the future. */
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
    self->buffer = (int8 *)RINGBUFFER_STARTADDR;
#elif( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
    retVal = MemPool_GetGlobalMemPool(&self->memPool);
    self->buffer = self->memPool->alloc(self->memPool, elementSize * numBufferSlots, MOD_ID_RINGBUFFER);
#endif
    if(NULL == self->buffer)
    {
      retVal = E_NOT_OK;
    }

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
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
    RingBuffer_MemPool_ClearAll();
#endif
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

/**
 * RB_OPT_003
 * dataBuffer and other pointers are derefenced with minimal checks.
 *      For example, we only check that the pointer is not null, but what if the address is still invalid and may cause an exception?
 *      We haven't implemented any checks anywhere in here for this at all - is it overkill to check that the address is 'reasonable'?
 *      We could consider implementing a platform specific check, implemented by the BRS module that include a more robust check.
 *      This would help with some edge case platforms where 0x00000000 is a valid memory address (i.e. interrupt vector table), and maybe for debugging
 *      we would want to read that data back.
 * 
 *      Counterpoint - we could implement this more robust check only for very specific checks, like if we want to debug an exceptioon and report it over serial/uart.
 *      Counter-Counterpoint - if we implement such a check, we can easily implement a user-register read like a debugger, so that a request can be made for addr+length
 *          we can then verify the user input to make sure that it is reasonable. In this case, that check could be performed by the portion of the program verifying the
 *          user's input.
 */

