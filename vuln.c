//
//  vuln.c
//  Mach_Messages
//
//  Created by A2nkF on 12/18/18.
//  Copyright © 2018 A2nkF. All rights reserved.

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "tfp.h"
#include "vuln.h"


// This code is from the heap1 challenge of https://exploit-exercises.com/protostar
int magic(char *msg1, char *msg2)
{
  struct internet *i1, *i2, *i3;

  i1 = malloc(sizeof(struct internet));
  i1->priority = 1;
  i1->name = malloc(8);

  i2 = malloc(sizeof(struct internet));
  i2->priority = 2;
  i2->name = malloc(8);

  strcpy(i1->name, msg1);
  strcpy(i2->name, msg2);

  printf("and that's a wrap folks!\n");
}
