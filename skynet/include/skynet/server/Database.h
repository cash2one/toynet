/**
* Copyright (c) 
*
* @author 
*/
// Database.h
#if !defined(INCLUDED_DATABASE)
#define INCLUDED_DATABASE

#include <boost/utility.hpp>
#include <string>

namespace skynet
{

/**
* @class Database
*
* Database Abstract Class.
*/
class WARNET_API Database : public boost::noncopyable
{
public:
    virtual ~Database() {}

    virtual bool open() = 0;

    virtual void close() = 0;

    virtual bool isOpened() = 0;

    virtual std::string getLastError() const = 0;
    virtual long getLastResult() const = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_DATABASE)
