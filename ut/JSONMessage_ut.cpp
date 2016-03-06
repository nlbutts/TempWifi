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

    time_t timeNow = time(NULL);
    struct tm * tmNow = localtime(&timeNow);
    char * dateTime = asctime(tmNow);
    std::string dateTimeStr(dateTime);
    if (!dateTimeStr.empty() && dateTimeStr[dateTimeStr.size() - 1] == '\n')
        dateTimeStr.erase(dateTimeStr.size() - 1);
    if (!dateTimeStr.empty() && dateTimeStr[dateTimeStr.size() - 1] == '\r')
        dateTimeStr.erase(dateTimeStr.size() - 1);

    std::string str;

    msg.insertMeasurement(dateTimeStr, "Temp=52.6, IR=12.6");
    str = msg.getJSON();
    std::cout << str << std::endl;

    msg.insertMeasurement(dateTimeStr, "Temp=62.6, IR=32.6");
    str = msg.getJSON();
    std::cout << str << std::endl;

    EXPECT_EQ(str.size(), 142);
}