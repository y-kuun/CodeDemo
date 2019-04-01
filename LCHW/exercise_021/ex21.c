#include <stdio.h>
#include <stdint.h>

#include "dbg.h"

int main(int argc, char* argv[])
{
    log_info("INT8  RANGE: %20d ~ %20d",  INT8_MIN,  INT8_MAX);
    log_info("INT16 RANGE: %20d ~ %20d", INT16_MIN, INT16_MAX);
    log_info("INT32 RANGE: %20d ~ %20d", INT32_MIN, INT32_MAX);
    log_info("INT64 RANGE: %20ld ~ %20ld", INT64_MIN, INT64_MAX);

    log_info("UINT8  RANGE: 0 ~ %20d", UINT8_MAX);
    log_info("UINT16 RANGE: 0 ~ %20d", UINT16_MAX);
    log_info("UINT32 RANGE: 0 ~ %20d", UINT32_MAX);
    log_info("UINT64 RANGE: 0 ~ %20lu", UINT64_MAX);

    return 0;
}
