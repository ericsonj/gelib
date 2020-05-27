/*
 * eprintf.h
 *
 *  Created on: May 27, 2020
 *      Author: Ericson Joseph
 */

#ifndef ESTRING_EPRINTF_H_
#define ESTRING_EPRINTF_H_

#include <stdarg.h>
#include "etypes.h"

eint e_vasprintf(echar**      string,
                 echar const* format,
                 va_list      args);

#endif /* ESTRING_EPRINTF_H_ */
