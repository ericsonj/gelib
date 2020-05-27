/*
 * econfig.h
 *
 *  Created on: May 25, 2020
 *      Author: Ericson Joseph
 */

#ifndef APP_ECONFIG_H_
#define APP_ECONFIG_H_

#include "main.h"

#define ESTR_MALLOC(_bsize_) 			App_staticMalloc(_bsize_)
#define ESTR_FREE(_mblock_)				App_staticFree(_mblock_)

#endif /* APP_ECONFIG_H_ */
