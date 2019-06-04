#include "minunit.h"
#include <assert.h>
#include <lcthw/ringbuff.h>

static RingBuffer *buffer = NULL;
static unsigned int BUFF_SIZE = 1 << 8;
#define TEST_DATA_SIZE 256
static char TEST_DATA[TEST_DATA_SIZE] = {0};
static char BACK_DATA[TEST_DATA_SIZE] = {0};

#define NUM_TESTS 3

char *test_create()
{
    buffer = RingBuffer_create(BUFF_SIZE);
    mu_assert(buffer != NULL, "Failed to create ringbuffer.");
    
    return NULL;
}

char *test_destroy()
{
    mu_assert(buffer != NULL, "Failed to destroy ringbuffer.");
    RingBuffer_destroy(buffer);

    return NULL;
}

void init_test_data()
{
    int i = 0;
    for(i = 0; i < TEST_DATA_SIZE; i++)
    {
        // max value for an unsigned char
        TEST_DATA[i] = i % 256 + 1;
    }
}

char *test_read_write()
{
    init_test_data();
    int i = 0;
    char *src_data = TEST_DATA;
    char *des_data = BACK_DATA;
    for(i = 0; (1 << (i+1)) < TEST_DATA_SIZE; i++)
    {
        int len = 1 << i;
        int rc = 0;
        debug("current len: %d", len);
        rc = RingBuffer_write(buffer, src_data, len);
        mu_assert(rc == len, "FAILED to write");
        rc = RingBuffer_read(buffer, des_data, len);
        mu_assert(rc == len, "FAILED to read");        
        while(len)
        {
            mu_assert(*(src_data++) == *(des_data++), "Failed read and write");
            len--;
        }
    }
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_read_write);    
    mu_run_test(test_destroy);
    
    return NULL;
}

RUN_TESTS(all_tests);
