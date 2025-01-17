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

/* write tests */
extern void test_RingBuffer_Write_ReturnsOk(void); 
extern void test_RingBuffer_Write_ReturnsNotOk_WrongSize(void); 
extern void test_RingBuffer_Write_ReturnsNotOk_NullPointerToData(void); 
extern void test_RingBuffer_Write_ReturnsNotOk_NotEnoughSpace_Full(void);

/* read tests */
extern void test_RingBuffer_Read_ReturnsOk(void); 
extern void test_RingBuffer_Read_CorrectDataReturned(void); 
extern void test_RingBuffer_Read_ReturnsNotOk_SizeBiggerThanElement(void); 
extern void test_RingBuffer_Read_ReturnsNotOk_SizeSmallerThanElement(void);
extern void test_RingBuffer_Read_ReturnsNotOk_NullPointerToData(void); 
extern void test_RingBuffer_Read_ReturnsNotOk_NoDataToRead(void);
/* TODO: Consider a "readPartial" so which would not move the head pointer */

/* Big test */
extern void test_RingBuffer_ReadAndWriteLotsOfData(void);
extern void test_RingBuffer_FillAndReadAndWriteAgain_OK(void);
extern void test_RingBuffer_FillAndReadAndWriteAgain_CorrectData(void);

extern void printRB(RingBuffer rb);