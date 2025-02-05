/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */


/* #include "Timer_RP2040.h" */
#include "RingBuffer_Test.h"
#include "unity.h"

#pragma ab

/* Global variables for testing */
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
uint8 * mempool_start;
extern MemPool memPools[1];
MemPool zeroMp = {0};
extern uint8 memPoolsIdx;
#endif


void setUp(void)
{
}

/* 
  These are run after each test, so we want to make sure the global variable is 'UNINIT' 
  so that each test can behave as if its its own test. 
*/
void tearDown(void)
{
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_MEMPOOL )
  memPools[0].poolStartAddr = 0;
  memPools[0].poolSize = 0;
  memPools[0].blockSize = 0;
  memPools[0].numTotalBlocks = 0;
  memPools[0].numFreeBlocks = 0;
  memPoolsIdx = 0xFF;
#endif
}

/* HELPER FUNCTIONS */
void testHelper_PrintCharArray(uint8 * arr, uint8 numElements)
{
  printf("array: \n");
  uint8 i;
  for(i = 0; i < numElements; i++)
  {
    printf(" 0x%02X |", arr[i]);
  }
  printf("\n");
}

/* TESTS */
/* Calls ...Create() and returns Ok */
void test_RingBuffer_Create_ReturnsOK(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 1, RINGBUFFER_SIZE);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_RingBuffer_Create_AllocatedSpaceIsEmpty(void)
{
  uint64 i;
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
#if( RINGBUFFER_USECASE == RINGBUFFER_USECASE_INDEPENDENT )
  ringbuffer_start[3] = 0xAB;
#endif
  retVal = RingBuffer_Create(&rb, 1, RINGBUFFER_SIZE);
  printf("rb.buffer: %p\n", (rb.buffer));
  (void) retVal; /* We do not care for the value of retVal. That is not the purpose of this test. */
  for(i = 0; i < RINGBUFFER_SIZE; i++)
  {
    TEST_ASSERT_EQUAL(0, rb.buffer[i]);
  }
}

void test_RingBuffer_Create_CorrectCapacityForElemSizeOne(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 1, RINGBUFFER_SIZE);
  TEST_ASSERT_EQUAL(RINGBUFFER_SIZE, rb.capacity);
  (void) retVal; /* We do not care for the value of retVal. That is not the purpose of this test. */

}

void test_RingBuffer_Create_CorrectCapacityForElemSizeFour(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE);
  TEST_ASSERT_EQUAL(RINGBUFFER_SIZE/4, rb.capacity);
  (void) retVal; /* We do not care for the value of retVal. That is not the purpose of this test. */

}

void test_RingBuffer_Create_ZeroElemSizeShouldFail(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 0, RINGBUFFER_SIZE);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Create_ThreeElemSizeShouldFail(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 3, RINGBUFFER_SIZE);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Create_HugeElemSizeShouldFail(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, RINGBUFFER_SIZE/2+1, RINGBUFFER_SIZE);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

/* write tests */
void test_RingBuffer_Write_ReturnsOk(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  retVal = rb.write(&rb, &arr0[0], 4);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_RingBuffer_Write_ReturnsNotOk_WrongSize(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 2, RINGBUFFER_SIZE/2);
  printf("rb.buffer %p\n", rb.buffer);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  retVal = rb.write(&rb, arr0, 4);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Write_ReturnsNotOk_NullPointerToData(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  uint8 * arr0 = NULL;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  retVal = rb.write(&rb, arr0, 4);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Write_ReturnsNotOk_NotEnoughSpace_Full(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, RINGBUFFER_SIZE/2, 2);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  uint8 arr1[4] = {0xDE, 0xAD, 0xBE, 0xEF};
  uint8 arr2[4] = {0x00, 0x11, 0x22, 0x33};
  retVal = rb.write(&rb, arr0, 4);
  retVal = rb.write(&rb, arr1, 4);
  TEST_ASSERT_EQUAL(E_OK, retVal); /* should not be full */
  retVal = rb.write(&rb, arr2, 4);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal); /* should be full */

}

/* read tests */
void test_RingBuffer_Read_ReturnsOk(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  uint8 arr1[4] = {0};
  retVal = rb.write(&rb, arr0, 4);

  retVal = E_NOT_OK;
  retVal = rb.read(&rb, arr1, 4);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_RingBuffer_Read_CorrectDataReturned(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  uint8 arr1[4] = {0};
  retVal = rb.write(&rb, arr0, 4);

  retVal = E_NOT_OK;
  retVal = rb.read(&rb, arr1, 4);
  (void)retVal; /* void for Compiler Warning unused variable */
  TEST_ASSERT_EQUAL_CHAR_ARRAY(arr0, arr1, 4);
}

void test_RingBuffer_Read_ReturnsNotOk_SizeBiggerThanElement(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  uint8 arr1[4] = {0, 0, 0, 0};
  retVal = rb.write(&rb, arr0, 4);

  retVal = E_NOT_OK;
  retVal = rb.read(&rb, arr1, 6);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Read_ReturnsNotOk_SizeSmallerThanElement(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  uint8 arr1[4] = {0, 0, 0, 0};
  retVal = rb.write(&rb, arr0, 4);

  retVal = E_NOT_OK;
  retVal = rb.read(&rb, arr1, 2);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
} 

void test_RingBuffer_Read_ReturnsNotOk_NullPointerToData(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0xFA, 0xCE, 0xBE, 0xEF};
  uint8 *arr1 = NULL;
  retVal = rb.write(&rb, arr0, 4);
  
  retVal = E_NOT_OK;
  retVal = rb.read(&rb, arr1, 4);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Read_ReturnsNotOk_NoDataToRead(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr1[4] = {0};

  retVal = E_NOT_OK;
  retVal = rb.read(&rb, arr1, 4);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_ReadAndWriteLotsOfData(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;
  uint32 i = 0;

  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0};
  uint8 arr1[4] = {0};

  for(i = 0; i < 0x3000; i++)
  {
    arr0[0] = i;
    arr0[1] = i+1;
    arr0[2] = i+2;
    arr0[3] = i+3;
    retVal = rb.write(&rb, arr0, 4);
    TEST_ASSERT_EQUAL(E_OK, retVal);
    retVal = rb.read(&rb, arr1, 4);
    TEST_ASSERT_EQUAL(E_OK, retVal);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(arr0, arr1, 4);
  }
}

void test_RingBuffer_FillAndReadAndWriteAgain_OK(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;

  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0, 1, 2, 3};
  uint8 arr1[4] = {4, 5, 6, 7};

  retVal = rb.write(&rb, arr0, 4);
  retVal = rb.write(&rb, arr1, 4);
  retVal = rb.read(&rb, arr1, 4); /* should now have the content of arr0 */
  retVal = rb.write(&rb, arr0, 4);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_RingBuffer_FillAndReadAndWriteAgain_CorrectData(void)
{
  RingBuffer rb; /* rb is placed on the stack, will be automatically deleted after execution of this function */
  Std_ErrorCode retVal = E_NOT_OK;

  retVal = RingBuffer_Create(&rb, 4, RINGBUFFER_SIZE/4);
  uint8 arr0[4] = {0, 1, 2, 3};
  uint8 arr1[4] = {4, 5, 6, 7};

  retVal = rb.write(&rb, arr0, 4);
  retVal = rb.write(&rb, arr1, 4);
  retVal = rb.read(&rb, arr1, 4); /* should now have the content of arr0 */
  retVal = rb.write(&rb, arr0, 4);

  (void) retVal;
  TEST_ASSERT_EQUAL_CHAR_ARRAY(arr0, arr1, 4);
}


/* utility functions */
void printRB(RingBuffer rb)
{
  RINGBUFFER_SIZE_TYPE i = 0;
  for(i = 0; i < rb.capacity; i++)
  {
    printf("%d, %d%d%d%d\n", i, rb.buffer[i*rb.elementSize + 0], rb.buffer[i*rb.elementSize + 1], rb.buffer[i*rb.elementSize + 2], rb.buffer[i*rb.elementSize + 3]);
  }
}
