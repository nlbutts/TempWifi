#include <string>
#include <sstream>
#include <fs.h>
#include "FileStorage.h"
#include <rom_map.h>
#include <utils.h>
#include <common.h>
#include <uart_if.h>

namespace File {

FileStorage::FileStorage(std::string fileName, uint32_t maxSize, std::string dataToWrite)
{
    handleError(sl_FsOpen((unsigned char *)fileName.c_str(),
                             FS_MODE_OPEN_CREATE(maxSize , _FS_FILE_OPEN_FLAG_NO_SIGNATURE_TEST | _FS_FILE_OPEN_FLAG_COMMIT ),
                             NULL, &_fd), "Failed to create file");

    handleError(sl_FsWrite(_fd, 0, (uint8_t*)dataToWrite.c_str(), dataToWrite.size()), "Failed to write data to file");
    handleError(sl_FsClose(_fd, NULL, NULL, 0), "Failed to close file");
    _fd = -1;
}

FileStorage::FileStorage(std::string fileName)
{
    handleError(sl_FsOpen((unsigned char *)fileName.c_str(),
                FS_MODE_OPEN_READ,
                NULL, &_fd),
                "Failed to open existing file.");
}


FileStorage::~FileStorage()
{
    if (_fd >= 0)
    {
        handleError(sl_FsClose(_fd, NULL, NULL, 0), "Failed to close file");
        _fd = -1;
    }
}

void FileStorage::handleError(long errorCode, char * str)
{
    if (errorCode < 0)
    {
        UART_PRINT(str);
        MAP_UtilsDelay(600000);
    }
}

std::string FileStorage::read(uint32_t offset, uint32_t bytesToRead)
{
    uint32_t maxBytesToRead = std::max(bytesToRead, sizeof(_tempBuffer));
    handleError(sl_FsRead(_fd, offset, (uint8_t*)_tempBuffer, maxBytesToRead), "Failed to read data from file.");
    return std::string((char*)_tempBuffer, maxBytesToRead);
}

