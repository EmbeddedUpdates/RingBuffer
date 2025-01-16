#include "RingBuffer.h"

/************************************************************
  LOCAL VARIABLES
************************************************************/


/************************************************************
  LOCAL FUNCTIONS
************************************************************/
extern void test_RingBuffer_Create_ReturnsOK(void);
extern void test_RingBuffer_Create_AllocatedSpaceIsEmpty(void);
extern void test_RingBuffer_Create_CorrectCapacityForElemSizeOne(void);
extern void test_RingBuffer_Create_CorrectCapacityForElemSizeFour(void);
extern void test_RingBuffer_Create_ZeroElemSizeShouldFail(void); 
extern void test_RingBuffer_Create_ThreeElemSizeShouldFail(void); 
extern void test_RingBuffer_Create_HugeElemSizeShouldFail(void); 
