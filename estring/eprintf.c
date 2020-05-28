/*
 * eprintf.c
 *
 *  Created on: May 27, 2020
 *      Author: Ericson Joseph
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "eprintf.h"
#include "emacros.h"
#include "emem.h"


#define G_VA_COPY(ap1, ap2)	  (*(ap1) = *(ap2))

eint e_vasprintf(echar**      string,
                 echar const* format,
                 va_list      args) {
  eint len;

  va_list args2;

  G_VA_COPY(args2, args);

  len = vsnprintf(NULL, 0, format, args2);

  if (len > 0) {
	  (*string) = e_malloc(len + 1);
	  vsnprintf((*string), len + 1, format, args);
  }

  va_end(args2);

  return len;
}


