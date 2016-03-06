#include <stdint.h>
#include <string>
#include <ctime>

namespace Messages {

class JSONMessage
{
public:
    JSONMessage();
    JSONMessage(std::string msg);
	~JSONMessage();

    void insertMeasurement(struct tm * ntpTime, float irTemp, float ambTemp);
    std::string getJSON();

private:
    std::string _msg;
    uint32_t    _initLen;

};
}
