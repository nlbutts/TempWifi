#include <stdio.h>
#include <string>
#include "JSONMessage.h"

using Messages::JSONMessage;

/*
 * hello.c
 */
int main(void)
{
	JSONMessage msg;

	msg.insertMeasurement("March 5th, 2016", "Temp=15 IR=25");
	msg.insertMeasurement("March 5th, 2016", "Temp=20 IR=30");

	std::string str = msg.getJSON();

	printf("Hello World\n");
	printf("%s\n", str.c_str());
	printf("Goodbye cruel world\n");
	
	return 0;
}
