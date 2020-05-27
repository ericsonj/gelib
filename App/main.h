/*
 * main.h
 *
 *  Created on: May 27, 2020
 *      Author: Ericson Joseph
 */

#ifndef APP_MAIN_H_
#define APP_MAIN_H_

#include "stdlib.h"

void* App_staticMalloc(size_t size);

void App_staticFree(void* ptr);

#endif /* APP_MAIN_H_ */
