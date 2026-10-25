/*
 * Copyright (c) 2024-2025, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "McuRTOS.h"
#include <stdio.h>
#if PL_CONFIG_USE_GCOV
  #include "McuCoverage.h"
#endif

int main(void) {
  gcov_test(1); /* exercise coverage test: Calc(1,2)->j=3, Test2 sets j=0, "j is zero!" */
  gcov_test(0); /* exercise coverage test: Calc(0,0)->j=1, Test2 leaves it, "j is not zero!" */
  gcov_test(-1); /* i<0: covers Calc's i>0-false path (else-if not taken) */
  gcov_test(6); /* i=6, j=12: covers Calc's i>0-true but j<10-false path */
  PL_Init();
  vTaskStartScheduler();
#if PL_CONFIG_USE_GCOV
  McuCoverage_WriteFiles(); /* write coverage data files */
#endif /* PL_CONFIG_USE_GCOV */

  for(;;) {
    /* do not return from main() */
  }
  return 0;
}
