#include <string>
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

void JSONMessage::insertMeasurement(std::string measurementTime, std::string measurement)
{
   if (_msg.size() > _initLen)
    {
        _msg += ",";
    }
    _msg += "{\"time\":\"";
    _msg += measurementTime;
    _msg += "\",\"temp\":\"";
    _msg += measurement;
    _msg += "\"}";
}

std::string JSONMessage::getJSON()
{
    return (_msg + "]}");
}

}