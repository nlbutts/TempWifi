#include <gmock/gmock.h>
#include <string>
#include <iostream>
#include "JSONMessage.h"

using Messages::JSONMessage;

TEST(MessageFormatter_Test, Constructor1)
{
    JSONMessage msg;
    std::string str = msg.getJSON();
    EXPECT_EQ(str.size(), 15);
}

TEST(MessageFormatter_Test, Constructor2)
{
    JSONMessage msg("{\"samples\": [{abcdef");
    std::string str = msg.getJSON();
    EXPECT_EQ(str.size(), 22);
}

TEST(MessageFormatter_Test, InsertData)
{
    JSONMessage msg;
    std::string str;

    time_t timeNow = time(NULL);
    struct tm * tmNow = localtime(&timeNow);

    msg.insertMeasurement(tmNow, 12.234, 56.789);
    str = msg.getJSON();
    std::cout << str << std::endl;

    msg.insertMeasurement(tmNow, 98.654, 32.165);
    str = msg.getJSON();
    std::cout << str << std::endl;
    EXPECT_TRUE(true);
}