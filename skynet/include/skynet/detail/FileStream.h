#if !defined(INCLUDE_FILESTREAM)
#define INCLUDE_FILESTREAM
#include <string>

namespace skynet
{
namespace filemode
{
    enum
    {
        READ      = 0x01,
        WRITE     = 0x02,
        READWRITE = 0x03,
        CREATE    = 0x10,
        APPEND    = 0x20
    };

}

namespace sharemode
{
    enum
    {
        DEFAULT = 0x00,        
        SHARE_READ  = 0x01,
        SHARE_WIRTE  = 0x02,
        SHARE_READWRITE = 0x03,
        SHARE_DELETE  = 0x05,
    };
}


#define HasFlag(v, f) (((v) & (f)) != 0)

class WARNET_API FileStream
{
public:
    FileStream(){hFile_ = INVALID_HANDLE_VALUE;}
    ~FileStream(){}
public:
    bool create(const std::string& fileName, int mode, int shareMode);
    unsigned int read(void* buffer, unsigned int readSize);
    unsigned int write(const void* buffer, unsigned int writeSize);
    unsigned int getFileSize() const;
    std::string getFileName() {
        return fileName_;
    }
    bool isOpen() const;
    bool isEndofFile() const;
    unsigned int seekFromCurrent(int offset);
    void close();
    void flush();
private:
    unsigned int fileSize_;
    HANDLE hFile_;
    std::string fileName_;
};
}
#endif