/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// AdoDatabaseTest.cpp
#include "stdafx.h"

#pragma warning( push )
#pragma warning( disable : 4244 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )
#include <warnet/server/AdoDatabase.h>
#include <iostream>
#include <sstream>
#include <ace/Profile_Timer.h>

using namespace warnet;

/**
* @class AdoDatabaseTest
*
* ADO(ActiveX Data Objects) 테스트
*/
class AdoDatabaseTest : public CppUnit::TestFixture 
{ 
    CPPUNIT_TEST_SUITE(AdoDatabaseTest);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testInsert);
    CPPUNIT_TEST(testSelect);
    CPPUNIT_TEST(testSelectMultipleRow);
    CPPUNIT_TEST(testStoredProcedureReturnValue);
    CPPUNIT_TEST(testStoredProcedureArguments);
    CPPUNIT_TEST(testStoredProcedureCollection);
    CPPUNIT_TEST(testConnectionPerformance);
    CPPUNIT_TEST_SUITE_END(); 
public: 
    void setUp();
    void tearDown();
private:
    void testOpen();
    void testInsert();
    void testSelect();
    void testSelectMultipleRow();
    void testStoredProcedureReturnValue();
    void testStoredProcedureArguments();
    void testStoredProcedureCollection();
    void testConnectionPerformance();

    std::string getErrorString(AdoDatabase& db, const std::string& msg);
    void createTestTable();
    void dropTestTable();
    int insertRecords(int row_count);
    void createStoredProcedure();
    void dropStoredProcedure();
private:
    AdoDatabase* database_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AdoDatabaseTest);

static const char connectionString[] =
    "Provider=sqloledb;"
    "Data Source=megadev;"
    "Initial Catalog=GameTest;"
    "User Id=warnet;"
    "Password=<dnjspt>;"; 

void AdoDatabaseTest::setUp()
{
    database_ = new AdoDatabase(connectionString);
    database_->open();

    dropTestTable();
    createTestTable();

    dropStoredProcedure();
    createStoredProcedure();
}


void AdoDatabaseTest::tearDown()
{
    dropStoredProcedure();
    dropTestTable();

    database_->close();
    delete database_;
}


std::string AdoDatabaseTest::getErrorString(AdoDatabase& db,
    const std::string& msg)
{
    std::string result(msg);
    result += "(Error:";
    result += db.getLastError();
    result += ")";
    return result;
}


void AdoDatabaseTest::createTestTable()
{
    AdoDatabase db(connectionString);
    CPPUNIT_ASSERT(db.open());
    if (! db.execute(
            "CREATE TABLE test_table (a int NOT NULL, b char (10) NULL)")) {
        CPPUNIT_FAIL(getErrorString(db, "createTestTable"));
    }
}


void AdoDatabaseTest::dropTestTable()
{
    AdoDatabase db(connectionString);
    CPPUNIT_ASSERT(db.open());
    (void)db.execute("DROP TABLE test_table");
}


int AdoDatabaseTest::insertRecords(int row_count)
{
    AdoDatabase db(connectionString);
    CPPUNIT_ASSERT(db.open());
    int sum_a = 0;
    for (int i = 0; i < row_count; ++i) {
        std::ostringstream sql;
        sql << "INSERT INTO test_table VALUES (";
        sql << i << ",'" << i << "')";
        CPPUNIT_ASSERT(db.execute(sql.str()));
        sum_a += i;
    }
    return sum_a;
}


void AdoDatabaseTest::createStoredProcedure()
{
    AdoDatabase db(connectionString);
    CPPUNIT_ASSERT(db.open());
    const char sp1[] =
        "CREATE PROCEDURE sp_ReturnTest As return (10)";
    if (! db.execute(sp1)) {
        std::cout << getErrorString(db, "sp1");
    }

    const char sp2[] =
        "Create Procedure sp_OutputTest "
        "@IN1 int,"
        "@OutInt int Output,"
        "@OutChar varchar(20) Output "
        "As "
        "SELECT @OutChar = 'Hello World'"
        "SELECT @OutInt = 10 * @IN1 "
        "return (90)";
    if (! db.execute(sp2)) {
        std::cout << getErrorString(db, "sp2");
    }

    const char sp3[] =
        "CREATE PROCEDURE sp_CollectTest As SELECT * FROM test_table";
    if (! db.execute(sp3)) {
        std::cout << getErrorString(db, "create procedure");
    }
}


void AdoDatabaseTest::dropStoredProcedure()
{
    AdoDatabase db(connectionString);
    CPPUNIT_ASSERT(db.open());
    (void)db.execute("DROP PROCEDURE sp_ReturnTest");
    (void)db.execute("DROP PROCEDURE sp_OutputTest");
    (void)db.execute("DROP PROCEDURE sp_CollectTest");
}


void AdoDatabaseTest::testOpen()
{
    CPPUNIT_ASSERT_MESSAGE("opened", database_->isOpened());

    database_->close();
    CPPUNIT_ASSERT_MESSAGE("not connected", ! database_->isOpened());
}


void AdoDatabaseTest::testInsert()
{
    CPPUNIT_ASSERT_MESSAGE("insert",
        database_->execute("INSERT INTO test_table VALUES (1, '1')"));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("affected records count",
        1, database_->getAffectedRecordsCount());
}


void AdoDatabaseTest::testSelect()
{
    CPPUNIT_ASSERT(
        database_->execute("INSERT INTO test_table VALUES (2, '2')"));

    const std::string query("SELECT * FROM test_table WHERE a=2");
    int a;
    std::string b;
    AdoRecordSet rs(*database_);
    if (rs.open(query)) {
        if (rs.getFieldValue("a", a))
            rs.getFieldValue("b", b);
        std::string msg = getErrorString(*database_, query);
        CPPUNIT_ASSERT_MESSAGE(msg, 2 == a);
        CPPUNIT_ASSERT_MESSAGE(msg, b[0] == '2');
    }
    else {
        std::string msg = getErrorString(*database_, query);
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }
}


void AdoDatabaseTest::testSelectMultipleRow()
{
    const int row_count = 10;
    int record_count = 0;
    int total_a = 0;
    int sum_a = 0;

    sum_a = insertRecords(row_count);

    const std::string query("SELECT * FROM test_table");
    AdoRecordSet rs(*database_);
    if (rs.open(query)) {
        while (! rs.isEof()) {
            int a;
            std::string b;
            if (rs.getFieldValue("a", a))
                rs.getFieldValue("b", b);
            total_a += a;
            ++record_count;
            if (! rs.moveNext())
                break;
        }
    }
    else {
        std::string msg = getErrorString(*database_, query);
        CPPUNIT_ASSERT_MESSAGE(msg, false);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("multiple records",
        row_count, record_count);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sum(a)",
        sum_a, total_a);
}


void AdoDatabaseTest::testStoredProcedureReturnValue()
{
    int retVal = 0;

    AdoCommand cmd(*database_, "sp_ReturnTest");
    AdoParameter paramRetVal(cmd,
        ADODB::adInteger, sizeof(retVal), ADODB::adParamReturnValue);

    AdoRecordSet rs(*database_);
    CPPUNIT_ASSERT(rs.execute(cmd));

    CPPUNIT_ASSERT(paramRetVal.getValue(retVal));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sp_ReturnTest return value",
        10, retVal);
}


void AdoDatabaseTest::testStoredProcedureArguments()
{
    int outInt = 0;
    std::string outChar;
    int retVal = 0;

    AdoCommand cmd(*database_, "sp_OutputTest");
    AdoParameter paramRetVal(cmd,
        ADODB::adInteger, sizeof(int), ADODB::adParamReturnValue);
    AdoParameter paramIn(cmd,
        ADODB::adInteger, sizeof(int), ADODB::adParamInput);
    paramIn.setValue(2);
    AdoParameter paramOutInt(cmd,
        ADODB::adInteger, sizeof(int), ADODB::adParamOutput);
    AdoParameter paramOutChar(cmd,
        ADODB::adChar, sizeof(char) * 20, ADODB::adParamOutput);

    AdoRecordSet rs(*database_);
    CPPUNIT_ASSERT(rs.execute(cmd));

    CPPUNIT_ASSERT(paramRetVal.getValue(retVal));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sp_OutputTest return value",
        90, retVal);
    CPPUNIT_ASSERT(paramOutInt.getValue(outInt));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sp_OutputTest outInt value",
        2 * 10, outInt);
    CPPUNIT_ASSERT(paramOutChar.getValue(outChar));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sp_OutputTest outChar value",
        std::string("Hello World         "), outChar);
}


void AdoDatabaseTest::testStoredProcedureCollection()
{
    const int row_count = 10;
    int sum_a = insertRecords(row_count);

    AdoCommand cmd(*database_, "sp_CollectTest");
    AdoRecordSet rs(*database_);
    CPPUNIT_ASSERT(rs.execute(cmd));

    int record_count = 0;
    int total_a = 0;
    while (! rs.isEof()) {
        int a;
        std::string b;
        CPPUNIT_ASSERT(rs.getColumnValue("a", a));
        CPPUNIT_ASSERT(rs.getColumnValue("b", b));
        total_a += a;
        ++record_count;
        if (! rs.moveNext())
            break;
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("multiple records",
        row_count, record_count);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("sum(a)",
        sum_a, total_a);
}


void AdoDatabaseTest::testConnectionPerformance()
{
    const int count = 100;

    ACE_Profile_Timer profiler;
    profiler.start();

    AdoDatabase database(connectionString);
    for (int i = 0; i < count; ++i) {
        CPPUNIT_ASSERT(database.open());
        database.close();
    }

    profiler.stop();
    ACE_Profile_Timer::ACE_Elapsed_Time et;
    profiler.elapsed_time(et);
    std::ostringstream oss;
    oss << "real time = " << et.real_time << " secs, user time = " <<
        et.user_time << " secs, system time = " << et.system_time <<
        " secs\n";
    CPPUNIT_ASSERT_MESSAGE(oss.str(), et.real_time < 1.);
}
