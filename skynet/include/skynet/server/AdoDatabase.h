/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// AdoDatabase.h
#if !defined(INCLUDED_ADODATABASE)
#define INCLUDED_ADODATABASE

#include <boost/utility.hpp>
#include <string>

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "EndOfFile")

namespace skynet
{

class AdoCommand;
class AdoParameter;

/**
* @class AdoDatabase
*
* ADO(ActiveX Data Objects) wrapper.
*/
class WARNET_API AdoDatabase : public boost::noncopyable
{
public:
    /**
     * ADO Connection Strings
     * (http://www.codeproject.com/database/connectionstrings.asp) 참고
     */
    AdoDatabase(const std::string& connectionString);
    ~AdoDatabase();

    bool open(int connectionTimeout = 4);
    void close();

    bool execute(const std::string& sql);

    bool isOpened();

    int getAffectedRecordsCount() const;

    std::string getLastError() const;
    long getLastResult() const;
private:
    bool initAdo();

    friend class AdoRecordSet;
    friend class AdoCommand;
    friend class AdoParameter;
    void setLastError(_com_error& e);
    ADODB::_ConnectionPtr getConnection() const;

    // not impl.
    AdoDatabase(const AdoDatabase&);
    AdoDatabase& operator=(const AdoDatabase&);
private:
    std::string connectionString_;
    long lastResult_;
    std::string lastError_;

    ADODB::_ConnectionPtr connection_;

    int affectedRecordsCount_;
};


/**
 * @class AdoRecordSet
 *
 */
class WARNET_API AdoRecordSet : public boost::noncopyable
{
public:
    explicit AdoRecordSet(AdoDatabase& db);
    ~AdoRecordSet();

    bool open(const std::string& query);
    void close();

    bool execute(AdoCommand& cmd);

    bool isEof();
    bool moveNext();

    /// compound query recordSet에서 다음 recordset으로 이동한다
    bool nextRecordset();

    bool isOpened();

    bool getFieldValue(const std::string& field, int& value);
    bool getFieldValue(const std::string& field, std::string& value);

    bool getColumnValue(const std::string& column, int& value);
    bool getColumnValue(const std::string& column, std::string& value);
    bool getColumnValue(const std::string& column, time_t& value);
private:
    // not impl.
    AdoRecordSet(const AdoRecordSet&);
    AdoRecordSet& operator=(const AdoRecordSet&);
private:
    AdoDatabase& database_;
    ADODB::_RecordsetPtr recordSet_;
};


/**
 * @class AdoCommand
 *
 */
class WARNET_API AdoCommand : public boost::noncopyable
{
public:
    AdoCommand(AdoDatabase& database, const std::string& cmd);

    ADODB::_RecordsetPtr execute();

    bool addParameter(AdoParameter& param);

    const std::string& getCommandName() const {
        return cmd_;
    }
private:
    friend class AdoParameter;
    AdoDatabase& getDatabase() const;
private:
    AdoDatabase& database_;
    ADODB::_CommandPtr command_;
    std::string cmd_;
};


/**
 * @class AdoParameter
 *
 */
class WARNET_API AdoParameter : public boost::noncopyable
{
public:
    AdoParameter(AdoCommand& cmd,
        ADODB::DataTypeEnum type, size_t dataSize,
        ADODB::ParameterDirectionEnum direction,
        const std::string& name = "");

    bool getValue(int& value);
    bool getValue(std::string& value);

    bool setValue(int value);
    bool setValue(const std::string& value);
    bool setValue(time_t value);
private:
    friend class AdoCommand;
    ADODB::_ParameterPtr getParameter() const;
private:
    AdoDatabase& database_;
    ADODB::_ParameterPtr parameter_;
};

///////////////////////////////////////////////////////////////////////////////

// = INLINE FUNCTIONS

inline ADODB::_ConnectionPtr AdoDatabase::getConnection() const
{
    return connection_;
}


inline int AdoDatabase::getAffectedRecordsCount() const
{
    return affectedRecordsCount_;
}


inline std::string AdoDatabase::getLastError() const
{
    return lastError_;
}


inline long AdoDatabase::getLastResult() const
{
    return lastResult_;
}


inline AdoDatabase& AdoCommand::getDatabase() const
{
    return database_;
}


inline ADODB::_ParameterPtr AdoParameter::getParameter() const
{
    return parameter_;
}

} // namespace skynet

#endif // !defined(INCLUDED_ADODATABASE)
