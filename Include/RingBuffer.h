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

#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
  #define RINGBUFFER_STARTADDR (uint64) ringbuffer_start
  #define RINGBUFFER_SIZE      (uint64) 0x1000
  #define RINGBUFFER_SIZE_TYPE         uint64
  extern uint8 * ringbuffer_start;
#elif( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
  #define RINGBUFFER_SIZE_TYPE         uint64

  #define RINGBUFFER_SIZE 0x1
  #define RINGBUFFER_STARTADDR ringbuffer_start
  extern uint8 * ringbuffer_start;
#else
#endif /* RINGBUFFER_USECASE */

#endif /* VIRTUAL_TARGET */

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
/*
  Create and Destroy are not parts of the RingBuffer object, as we are not relying on dynamic 
  allocation. We COULD put create and destory in the ringbuffer object, but it doesn't make a whole 
  lot of sense without dynamic allocation. If we split up the memory pool to multiple RingBuffers, 
  then we will want to implement dynamic allocation within the MemoryPool
*/
extern Std_ErrorCode RingBuffer_Create( RingBuffer * self, RINGBUFFER_SIZE_TYPE elementSize, RINGBUFFER_SIZE_TYPE numBufferSlots);
#endif /* RINGBUFFER_H */