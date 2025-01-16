/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */


/* #include "Timer_RP2040.h" */
#include "RingBuffer_Test.h"
#include "unity.h"

/* Global variables for testing */
uint8 * mempool_start;

#pragma ab

void setUp(void)
{
}

/* 
  These are run after each test, so we want to make sure the global variable is 'UNINIT' 
  so that each test can behave as if its its own test. 
*/
void tearDown(void)
{

}

/* HELPER FUNCTIONS */

/* TESTS */
/* Calls ...Create() and returns Ok */
void test_RingBuffer_Create_ReturnsOK(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 1);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_RingBuffer_Create_AllocatedSpaceIsEmpty(void)
{
  uint64 i;
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  mempool_start[3] = 0xFF;

  retVal = RingBuffer_Create(&rb, 1);
  (void) retVal; /* We do not care for the value of retVal. That is not the purpose of this test. */

  for(i = 0; i < RINGBUFFER_MEMPOOL_SIZE; i++)
  {
    TEST_ASSERT_EQUAL(0, rb.buffer[i]);
  }
}

void test_RingBuffer_Create_CorrectCapacityForElemSizeOne(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 1);
  TEST_ASSERT_EQUAL(RINGBUFFER_MEMPOOL_SIZE, rb.capacity);
  (void) retVal; /* We do not care for the value of retVal. That is not the purpose of this test. */

}

void test_RingBuffer_Create_CorrectCapacityForElemSizeFour(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 4);
  TEST_ASSERT_EQUAL(RINGBUFFER_MEMPOOL_SIZE/4, rb.capacity);
  (void) retVal; /* We do not care for the value of retVal. That is not the purpose of this test. */

}

void test_RingBuffer_Create_ZeroElemSizeShouldFail(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 0);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Create_ThreeElemSizeShouldFail(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 3);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_RingBuffer_Create_HugeElemSizeShouldFail(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, RINGBUFFER_MEMPOOL_SIZE/2)+1;
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}
