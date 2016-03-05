#include <string.h>
#include "MessageFormatter.h"

static const char   _Message[1000];
static uint32_t     _InitializedLength;

void MeasurementInit()
{
    strcpy(_Message, "{\"samples\": [{");
    _InitializedLength = strlen(_Message);
}

void InsertMeasurement(char * measurementTime, char * measurement)
{
    if (strlen(Message) > _InitializedLength)
    {
        strcat(Message, ",");
    }
    strcat(Message, "\"time\":");
    strcat(Message, measurementTime);
    strcat(Message, ",\"temp\":");
    strcat(Message, measurement);
    strcat(Message, "}");
}

void GetJSON(char * json, uint32_t maxSize)
{
    strncpy(json, Message, maxSize);
}

