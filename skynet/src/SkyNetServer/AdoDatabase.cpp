/**
* Copyright (c) 
*
* @author 
*/
// AdoDatabase.cpp
//
#include "stdafx.h"
#include <skynet/server/AdoDatabase.h>
#include <skynet/detail/Win32Util.h>
#include <sstream>
#include <skynet/detail/Assert.h>

namespace skynet
{

namespace
{

bool getInteger(_variant_t vt, int& value)
{
    switch (vt.vt) {
    case VT_I1:
        value = vt.cVal;
        break;
    case VT_UI1:
        value =  vt.bVal;
        break;
    case VT_I2:
        value =  vt.iVal;
        break;
    case VT_UI2:
        value =  vt.uiVal;
        break;
    case VT_I4:
        value =  vt.lVal;
        break;
    case VT_UI4:
        value =  vt.ulVal;
        break;
    case VT_INT:
        value = vt.intVal;
        break;
    case VT_UINT:
        value = vt.uintVal;
        break;
    case VT_BOOL:
        value = vt.boolVal == 0 ? 0 : 1;
        break;
    case VT_NULL:
        return false;
    default:
        value = 0;
        break;
    }
    return true;
}


bool getString(_variant_t vt, std::string& value)
{
    if (VT_BSTR == vt.vt) {
        value = static_cast<const char*>(bstr_t(vt.bstrVal));
        return true;
    }
    else if (VT_NULL == vt.vt) {
        value.clear();
        return true;
    }

    return false;
}


bool getTime(_variant_t vt, time_t& value)
{
    switch (vt.vt) {
    case VT_DATE:
        value = variantTimeToUnixTime(vt);
        break;
    case VT_EMPTY:
    case VT_NULL:
        value = 0;
        break;
    default:
        return false;
    }
    return true;
}

} // namespace

// = AdoDatabase

AdoDatabase::AdoDatabase(const std::string& connectionString) :
    connectionString_(connectionString),
    lastResult_(S_OK)
{
    (void)initAdo();
    WN_ASSERT(connection_ != 0);
    connection_->CursorLocation = ADODB::adUseClient;
}


AdoDatabase::~AdoDatabase()
{
    close();
}


bool AdoDatabase::open(int connectionTimeout /*= 4*/)
{
    close();
    try {
        connection_->PutConnectionTimeout(connectionTimeout);
        HRESULT hr = connection_->Open(connectionString_.c_str(), "", "",
            ADODB::adConnectUnspecified);
        return SUCCEEDED(hr);
    }
    catch (_com_error& e) {
        setLastError(e);
    }
    return false;
}


void AdoDatabase::close()
{
    if (isOpened())
        connection_->Close();
}


bool AdoDatabase::execute(const std::string& sql)
{
    WN_ASSERT(connection_ != 0);
    affectedRecordsCount_ = 0;
    if (sql.empty())
        return false;

    _variant_t records;
    try {
        connection_->Execute(_bstr_t(sql.c_str()), &records,
            ADODB::adExecuteNoRecords);
        affectedRecordsCount_ = records.iVal;
        return true;
    }
    catch (_com_error& e) {
        setLastError(e);
    }
    return false;
}


bool AdoDatabase::isOpened()
{
    try {
        return (connection_ != 0) &&
            connection_->GetState() != ADODB::adStateClosed;
    }
    catch (_com_error& e) {
        setLastError(e);
    }
    return false;
}


bool AdoDatabase::initAdo()
{
    HRESULT hr = connection_.CreateInstance(__uuidof(ADODB::Connection));
    return SUCCEEDED(hr);
}


void AdoDatabase::setLastError(_com_error& e)
{
    const char* errorMsg = e.ErrorMessage() != 0 ? e.ErrorMessage() : "n/a";
    _bstr_t source(e.Source());
    _bstr_t description(e.Description());

    std::ostringstream oss;
    oss << "AdoDataBase Error: Code = 0x" << std::hex << e.Error() <<
        ", Code meaning = " << errorMsg <<
        ", Source = " << static_cast<LPCSTR>(source) <<
        ", Description = " << static_cast<LPCSTR>(description);
    lastError_ = oss.str();
    lastResult_ = e.Error();
}


///////////////////////////////////////////////////////////////////////////////
// AdorecordSet

AdoRecordSet::AdoRecordSet(AdoDatabase& database) :
    database_(database)
{
}


AdoRecordSet::~AdoRecordSet()
{
    close();
    if (recordSet_ != 0)
        recordSet_.Release();
}


bool AdoRecordSet::open(const std::string& query)
{
    try {
        if (recordSet_ == 0) {
            HRESULT hr = recordSet_.CreateInstance(__uuidof(ADODB::Recordset));
            if (FAILED(hr)) {
                WN_ASSERT(false);
                return false;
            }
        }
        recordSet_->Open(query.c_str(),
            _variant_t(
                static_cast<IDispatch*>(database_.getConnection()), TRUE), 
            ADODB::adOpenForwardOnly, ADODB::adLockReadOnly, //adLockOptimistic,
            ADODB::adCmdUnknown);
        return true;
    }
    catch (_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


void AdoRecordSet::close()
{
    if (isOpened()) {
        try {
            recordSet_->Close();
        }
        catch (_com_error& e) {
            database_.setLastError(e);
        }
    }
}


bool AdoRecordSet::isOpened()
{
    try {
        return (recordSet_ != 0) &&
            (recordSet_->GetState() != ADODB::adStateClosed);
    }
    catch (_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoRecordSet::execute(AdoCommand& cmd)
{
    try {
        recordSet_ = cmd.execute();
        return recordSet_ != 0;
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoRecordSet::isEof()
{
	if (recordSet_ == 0) {
		return true;
	}
	
    try {
        return recordSet_->EndOfFile == VARIANT_TRUE;
    }
    catch (_com_error& e) {
        database_.setLastError(e);
    }
    return true;
}


bool AdoRecordSet::moveNext()
{
    WN_ASSERT(recordSet_ != 0);
    try {
        recordSet_->MoveNext();
    }
    catch(_com_error& e) {
        database_.setLastError(e);
        return false;
    }
    return true;
}


bool AdoRecordSet::nextRecordset()
{
	if (recordSet_ == 0) {
		return false;
	}

    try {
        if (recordSet_->GetState() != ADODB::adStateOpen) {
            return false;
        }

        _variant_t vtAffect = 0;
#pragma warning ( push )
#pragma warning ( disable: 4312 )
        recordSet_ = recordSet_->NextRecordset(&vtAffect);
#pragma warning ( pop )
    }
    catch(_com_error& e) {
        database_.setLastError(e);
        return false;
    }

    return true;
}


bool AdoRecordSet::getFieldValue(const std::string& field, int& value)
{
    WN_ASSERT(recordSet_ != 0);
    try
    {
        return getInteger(recordSet_->Fields->GetItem(field.c_str())->Value,
            value);
    }
    catch (_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoRecordSet::getFieldValue(const std::string& field, std::string& value)
{
    WN_ASSERT(recordSet_ != 0);
    try
    {
        return getString(recordSet_->Fields->GetItem(field.c_str())->Value,
            value);
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    value.clear();
    return false;
}


bool AdoRecordSet::getColumnValue(const std::string& column, int& value)
{
    WN_ASSERT(recordSet_ != 0);
    try
    {
        return getInteger(recordSet_->GetCollect(column.c_str()), value);
    }
    catch (_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoRecordSet::getColumnValue(const std::string& column,
    std::string& value)
{
    WN_ASSERT(recordSet_ != 0);
    try
    {
        return getString(recordSet_->GetCollect(column.c_str()), value);
    }
    catch (_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoRecordSet::getColumnValue(const std::string& column, time_t& value)
{
    WN_ASSERT(recordSet_ != 0);
    try
    {
        return getTime(recordSet_->GetCollect(column.c_str()), value);
    }
    catch (_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}

/*
size_t AdoRecordSet::getRecordCount()
{
    size_t rows = recordSet_->GetRecordCount();
    if (rows == -1) {
        rows = 0;
        if (recordSet->EndOfFile != VARIANT_TRUE)
            recordSet_->MoveFirst();

        while (recordSet_->EndOfFile != VARIANT_TRUE) {
            rows++;
            recordSet_->MoveNext();
        }
        if(rows > 0)
            recordSet_->MoveFirst();
    }
    return rows;
}
*/


///////////////////////////////////////////////////////////////////////////////
// AdoCommand

AdoCommand::AdoCommand(AdoDatabase& database, const std::string& cmd) :
    database_(database),
    cmd_(cmd)
{
    command_.CreateInstance(__uuidof(ADODB::Command));
    command_->CommandText = _bstr_t(cmd_.c_str());
    command_->CommandType = ADODB::adCmdStoredProc;
}


ADODB::_RecordsetPtr AdoCommand::execute()
{
    WN_ASSERT(command_ != 0);

    command_->ActiveConnection = database_.getConnection();
    return command_->Execute(NULL, NULL, ADODB::adCmdStoredProc);
}


bool AdoCommand::addParameter(AdoParameter& param)
{
    try {
        command_->Parameters->Append(param.getParameter());
        return true;
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////
// AdoParameter

AdoParameter::AdoParameter(AdoCommand& cmd,
    ADODB::DataTypeEnum type, size_t dataSize,
    ADODB::ParameterDirectionEnum direction,
    const std::string& name/* = ""*/) :
    database_(cmd.getDatabase())
{
    parameter_.CreateInstance(__uuidof(ADODB::Parameter));
    parameter_->Direction = direction;
    parameter_->Name = _bstr_t(name.c_str());
    parameter_->Type = type;
    parameter_->Size = static_cast<ADODB::ADO_LONGPTR>(dataSize);

    cmd.addParameter(*this);
}


bool AdoParameter::getValue(int& value)
{
    try {
        return getInteger(parameter_->Value, value);
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoParameter::getValue(std::string& value)
{
    try {
        return getString(parameter_->Value, value);
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    value.clear();
    return false;
}


bool AdoParameter::setValue(int value)
{
    _variant_t vtVal;
    vtVal.vt = VT_I4;
    vtVal.lVal = static_cast<LONG>(value);

    try {
        WN_ASSERT(parameter_->Size > 0);
        parameter_->Value = vtVal;
        return true;
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoParameter::setValue(const std::string& value)
{
    _variant_t vtVal;
    if (! value.empty())
        vtVal.vt = VT_BSTR;
    else
        vtVal.vt = VT_NULL;
    vtVal.bstrVal = _bstr_t(value.c_str()).copy();

    try {
        WN_ASSERT(parameter_->Size > 0);
        parameter_->Value = vtVal;
        return true;
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}


bool AdoParameter::setValue(time_t value)
{
    _variant_t vtVal(unixTimeToVariantTime(value));

    try {
        WN_ASSERT(parameter_->Size > 0);
        parameter_->Value = vtVal;
        return true;
    }
    catch(_com_error& e) {
        database_.setLastError(e);
    }
    return false;
}

} // namespace skynet
