#include <string>
#include <sstream>
#include "JSONMessage.h"

namespace Messages {

JSONMessage::JSONMessage()
: _msg("{\"samples\": [")
, _initLen(_msg.size())
{
}

JSONMessage::JSONMessage(std::string msg)
: _msg(msg)
{
}

JSONMessage::~JSONMessage()
{

}

void JSONMessage::insertMeasurement(struct tm * ntpTime, float irTemp, float ambTemp)
{
    std::string ntpTimeStr = asctime(ntpTime);
    if (!ntpTimeStr.empty() && ntpTimeStr[ntpTimeStr.size() - 1] == '\n')
        ntpTimeStr.erase(ntpTimeStr.size() - 1);
    if (!ntpTimeStr.empty() && ntpTimeStr[ntpTimeStr.size() - 1] == '\r')
        ntpTimeStr.erase(ntpTimeStr.size() - 1);

    std::ostringstream fmtMsg;

   if (_msg.size() > _initLen)
    {
        _msg += ",";
    }
    fmtMsg << "{\"time\":\"";
    fmtMsg <<  ntpTimeStr;
    fmtMsg <<  "\",\"IRTemp\":\"";
    fmtMsg <<  irTemp;
    fmtMsg <<  "\",\"AMBTemp\":\"";
    fmtMsg <<  ambTemp;
    fmtMsg <<  "\"}";
    _msg += fmtMsg.str();
}

std::string JSONMessage::getJSON()
{
    return (_msg + "]}");
}

}
