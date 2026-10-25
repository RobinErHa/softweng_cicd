/*
 * Copyright (c) 2023-2024, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gcov_test.h"
#include <stdio.h>
#pragma GCC optimize("O3,inline")

static int Calc(int i, int j) {
  int res = 0; /* initialize: value is undefined for i<0 or (i>0, j>=10) */
  if (i==0) {
    res = i+1;
  } else if (i>0 && j<10) {
    res = i+j;
  }
  return res;
}

static int Value(int i) {
  if (i==3) {
    return 5;
  }
  return 1;
}

static void Test2(int *p) {
  if (*p!=0) {
    if (Value(*p)==5) {
      printf("value is 5\n");
      *p = 0;
    }
  }
}

void gcov_test(int i) {
  int j = Calc(i, i*2);
  Test2(&j);
  if (j==0) {
    printf("j is zero!\n");
  } else {
    printf("j is not zero!\n");
  }
  #if 1
   /* cover Calc: i==0, i>0&&j<10, and fall-through paths */
   Calc(0, 0);
   Calc(1, 2);
   Calc(-1, -2);
   Calc(1, 20);

   /* cover Value: i==3 and i!=3 paths (i!=3 is unreachable via Test2) */
   Value(3);
   Value(7);

   /* cover Test2: *p==0, Value(*p)==5, Value(*p)!=5 paths */
   int v0 = 0, v3 = 3, v7 = 7;
   Test2(&v0);
   Test2(&v3);
   Test2(&v7);
   /* \TODO: add more code to have better coverage */
  #endif
}
