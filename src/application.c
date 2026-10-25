/*
 * Copyright (c) 2023-2024, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "application.h"
#include "McuRTOS.h"
#include "McuLog.h"
#include "McuUtility.h"
#include "McuLED.h"
#include "leds.h"
#include "gcov_test.h"
#include <stdio.h>

static void AppTask(void *pv) {
  int count = 0;
  gcov_test(1); /* exercise coverage test: Calc(1,2)->j=3, Test2 sets j=0, "j is zero!" */
  gcov_test(0); /* exercise coverage test: Calc(0,0)->j=1, Test2 leaves it, "j is not zero!" */
  gcov_test(-1); /* i<0: covers Calc's i>0-false path (else-if not taken) */
  gcov_test(6); /* i=6, j=12: covers Calc's i>0-true but j<10-false path */
#if PL_CONFIG_USE_PICO_W && PL_CONFIG_USE_LEDS
  Leds_InitFromTask(); /* needed for the on-board Pico-W LED */
#endif
  for(;;) {
    count++;
    #if PL_CONFIG_USE_LEDS
    Leds_Neg(LEDS_ONBOARD);
    #endif
    vTaskDelay(pdMS_TO_TICKS(1000));
    #if PL_CONFIG_USE_GCOV
    if (count>5) {
      vTaskEndScheduler();
    }
    #endif
  }
}

void App_Init(void) {
  if (xTaskCreate(
      AppTask,  /* pointer to the task */
      "App", /* task name for kernel awareness debugging */
      1500/sizeof(StackType_t), /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY+2,  /* initial priority */
      (TaskHandle_t*)NULL /* optional task handle to create */
    ) != pdPASS)
  {
    McuLog_fatal("Failed creating task");
    for(;;){} /* error! probably out of memory */
  }
}
