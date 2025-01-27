/**
 * 
* @file "RingBuffer.h"
* @author Madrick3
* @brief A protocol agnostic data buffer for various project .
* 
* @COMPONENT: RingBuffer
* @VERSION: 01.00.00
*/
/************************************************************
  Version History
  -----------------------------------------------------------
  Revision |  Author   |  Change ID  |  Description
  01.00.00 |  Madrick3 |  Skeleton   |  Initial Creation
************************************************************/

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

/************************************************************
  INCLUDES
************************************************************/
/* Only dependency - to track std_responsecodes in c programming projects */
#include "Platform_Types.h"

/************************************************************
  CONFIGURATION CHECKS
************************************************************/
#define RINGBUFFER_USECASE_INDEPENDENT 0
#define RINGBUFFER_USECASE_MEMPOOL 1

#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
 /* intentionally left blank */
#elif( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
 /* intentionally left blank */
#else 
  #error "Undefined Usecase - at least one of the memory allocation usecases must be configured!"
#endif

/************************************************************
  INCLUDES
************************************************************/
/* Only dependency - to track std_responsecodes in c programming projects */
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
#include "MemPool.h"
#endif
/************************************************************
  DEFINES
************************************************************/
/* A mempool is reserved for the RINGBUFFER, and is allocated through the linkerscript at compile time */
#if defined( RP2040 )
#define RINGBUFFER_SIZE_TYPE         uint32
#elif defined ( VIRTUAL_TARGET ) /* A mempool is allocated at runtime - for virtual test target */
#define RINGBUFFER_SIZE_TYPE         uint64
#endif

#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
  #define RINGBUFFER_STARTADDR (uint64) ringbuffer_start
  #define RINGBUFFER_SIZE      (uint64) 0x1000
  extern uint8 * ringbuffer_start;
#elif( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
  #define RINGBUFFER_SIZE      (uint64) 0x1000
#else
#endif /* RINGBUFFER_USECASE */

#define MOD_ID_RINGBUFFER 0xF1

/************************************************************
  ENUMS AND TYPEDEFS
************************************************************/
typedef struct RingBuffer_tag
{
  uint8 * buffer;
  RINGBUFFER_SIZE_TYPE elementSize; /* size of an element in the ringbuffer */
  RINGBUFFER_SIZE_TYPE capacity; /* maximum number of items in the ringbuffer */
  RINGBUFFER_SIZE_TYPE count; /* current number of items in the ringbuffer */
  RINGBUFFER_SIZE_TYPE head; /* read pointer - "dirty" data that needs to be popped */
  RINGBUFFER_SIZE_TYPE tail; /* write pointer - "clean" data that has been processed and can be overwritten */
  /* Depending on the usecase, we need to determine where data will be placed and how much data we can use. */
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
#elif( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
  MemPool memPool;
#endif
  /* A write or enqueue for adding data to the ringbuffer */
  Std_ErrorCode (*write) (void * self, uint8 * dataBuffer, RINGBUFFER_SIZE_TYPE size);
  /* A read ir dequeue for removing data from the ringbuffer */
  Std_ErrorCode (*read) (void * self, uint8 * dataBuffer, RINGBUFFER_SIZE_TYPE size);
} RingBuffer;

/************************************************************
  EXTERN FUNCTIONS
************************************************************/

/************************************************************
   DEFAULT CLASS FUNCTIONS
************************************************************/

/************************************************************
  GLOBAL FUNCTIONS
************************************************************/
/**
 * RingBuffer_Create
 * Create and Destroy are not parts of the RingBuffer object, as we are not relying on dynamic 
 * allocation. We COULD put create and destory in the ringbuffer object, but it doesn't make a whole 
 * lot of sense without dynamic allocation. If we split up the memory pool to multiple RingBuffers, 
 * then we will want to implement dynamic allocation within the MemoryPool
 * 
 * Returns a pointer to a 'new' RingBuffer structure. We do not utilize the heap, since we want this to
 * be memory safe - so we will not utilize Malloc either. We will statically allocate the memory space at link time,
 * and dynamically assign the data within that allocated region during runtime.
 * 
 * @param self: ringBuffer to store the new data in.
 * @param elementSize: size of the elements that will be placed in the ringbuffer.
 * @param numBufferSlots: number of slots that the buffer must hold at one time.
 * 
 * @return Std_ErrorCode
 */
extern Std_ErrorCode RingBuffer_Create( RingBuffer * self, RINGBUFFER_SIZE_TYPE elementSize, RINGBUFFER_SIZE_TYPE numBufferSlots);
#endif /* RINGBUFFER_H */