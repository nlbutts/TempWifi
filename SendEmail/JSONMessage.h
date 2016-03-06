#include <stdint.h>
#include <string>

namespace Messages {

class JSONMessage
{
public:
    JSONMessage();
    JSONMessage(std::string msg);
	~JSONMessage();

    void insertMeasurement(std::string measurementTime, std::string measurement);
    std::string getJSON();

private:
    std::string _msg;
    uint32_t    _initLen;

};
}
