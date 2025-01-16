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
  DEFINES
************************************************************/
/* A mempool is reserved for the RINGBUFFER, and is allocated through the linkerscript at compile time */
#if defined( RP2040 )
#define RINGBUFFER_MEMPOOL_STARTADDR __MEMPOOL_START
#define RINGBUFFER_MEMPOOL_SIZE      __MEMPOOL_LENGTH
#define RINGBUFFER_SIZE_TYPE         uint32
#endif
/* A mempool is allocated at runtime - for virtual test target */
#if defined ( VIRTUAL_TARGET )
#define RINGBUFFER_MEMPOOL_STARTADDR (uint64) mempool_start
#define RINGBUFFER_MEMPOOL_SIZE      (uint64) 0x1000
#define RINGBUFFER_SIZE_TYPE         uint64
extern uint8 * mempool_start;
#endif

/************************************************************
  ENUMS AND TYPEDEFS
************************************************************/
typedef struct RingBuffer_tag
{
  uint8 * buffer;
  RINGBUFFER_SIZE_TYPE elementSize; /* size of an element in the ringbuffer */
  RINGBUFFER_SIZE_TYPE capacity; /* maximum number of items in the ringbuffer */
  RINGBUFFER_SIZE_TYPE size; /* current number of items in the ringbuffer */
  RINGBUFFER_SIZE_TYPE head; /* read pointer - "dirty" data that needs to be popped */
  RINGBUFFER_SIZE_TYPE tail; /* write pointer - "clean" data that has been processed and can be overwritten */
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
/*
  Create and Destroy are not parts of the RingBuffer object, as we are not relying on dynamic 
  allocation. We COULD put create and destory in the ringbuffer object, but it doesn't make a whole 
  lot of sense without dynamic allocation. If we split up the memory pool to multiple RingBuffers, 
  then we will want to implement dynamic allocation within the MemoryPool
*/
extern Std_ErrorCode RingBuffer_Create( RingBuffer * self, RINGBUFFER_SIZE_TYPE elementSize);
#endif /* RINGBUFFER_H */