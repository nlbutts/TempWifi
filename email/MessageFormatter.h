#include <stdint.h>

void MeasurementInit();
void InsertMeasurement(char * measurementTime, char * measurement);
void GetJSON(char * json, uint32_t maxSize);
