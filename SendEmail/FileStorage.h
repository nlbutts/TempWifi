#include <stdint.h>
#include <string>

namespace File {

class FileStorage
{
public:
    FileStorage(std::string fileName, uint32_t maxSize, std::string dataToWrite);
    FileStorage(std::string fileName);
	~FileStorage();

    std::string read(uint32_t offset, uint32_t bytesToRead);

private:
    void handleError(long errorCode, char * str);


    uint8_t     _tempBuffer[200];
    long        _fd;
};
}
