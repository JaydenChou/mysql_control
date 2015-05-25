//²âÊÔÀý×Ó

#include "mysql_control.h"  
#include <string>
#include <iostream>

#define TEST_HOST "localhost"
#define TEST_USER "root"
#define TEST_PASSWORD "aaaaaaa"
#define TEST_DB "test"

void MysqlLogCallBack(int errid, const char *msg)
{
    std::cout << "Mysql error: " << errid << "--" << msg << std::endl;
}

void ControlLogCallback(CON_LOG_KIND kind, const char *msg, const char *sql)
{

}

int main(void)
{
    MysqlControl control;
    int row, column;

    control.SetControlLogCallback(ControlLogCallback);
    control.SetMysqlLogCallback(MysqlLogCallBack);
    bool ok = control.RealConnect(TEST_HOST, TEST_USER, TEST_PASSWORD, TEST_DB);
    if (!ok)
    {
        std::cout << "connect error" << std::endl;
        return 1;
    }

    control.QueryAndStore("show tables;");
    DoubleDArray<Field> arr;
    control.GetAllResult(arr);
    for (size_t i = 0; i < arr.GetRowCount(); i++)
        std::cout << arr.GetValue(i, 0).GetString() << std::endl;
    
    return 0;
}