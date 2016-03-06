#include <string.h>
#include "MessageFormatter.h"

static char   _Message[1000];
static uint32_t     _InitializedLength;

void MeasurementInit()
{
    strcpy(_Message, "{\"samples\": [{");
    _InitializedLength = strlen(_Message);
}

void InsertMeasurement(char * measurementTime, char * measurement)
{
    if (strlen(_Message) > _InitializedLength)
    {
        strcat(_Message, ",");
    }
    strcat(_Message, "\"time\":");
    strcat(_Message, measurementTime);
    strcat(_Message, ",\"temp\":");
    strcat(_Message, measurement);
    strcat(_Message, "}");
}

void GetJSON(char * json, uint32_t maxSize)
{
    strncpy(json, _Message, maxSize);
}

