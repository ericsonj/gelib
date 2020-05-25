/*
 * eopt.h
 *
 *  Created on: May 24, 2020
 *      Author: ericson
 */

#ifndef ESTRING_EOPT_H_
#define ESTRING_EOPT_H_

#include "econfig.h"

#ifndef ESTR_MALLOC
#define ESTR_MALLOC(_bsize_) 			malloc(_bsize_)
#endif

#ifndef ESTR_FREE
#define ESTR_FREE(_mblock_)				free(_mblock_)
#endif

#endif /* ESTRING_EOPT_H_ */
