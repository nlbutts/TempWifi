/*
 * Test.h
 *
 *  Created on: Feb 28, 2016
 *      Author: Nick Butts
 */

#include <stdint.h>

#ifndef TEST_H_
#define TEST_H_

class Test {
public:
	Test();
	virtual ~Test();

	uint32_t getTime(uint32_t foo);
};

#endif /* TEST_H_ */
