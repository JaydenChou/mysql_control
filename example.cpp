//²âÊÔÀý×Ó

#include "mysql_control.h"  
#include <string>
#include <iostream>
using namespace std;

#define HOST "localhost"
#define USER "root"
#define PASSWORD "aaaaaaa"
#define DB "test"

void MysqlLogCallBack(int errid, const char *msg)
{
    cout << "Mysql error: " << errid << "--" << msg << endl;
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
    bool ok = control.RealConnect(HOST, USER, PASSWORD, DB);
    if (!ok)
    {
        cout << "connect error" << endl;
        return 1;
    }

    control.QueryAndStore("show tables;");
    DoubleDArray<Field> arr;
    control.GetAllResult(arr);
    for (size_t i = 0; i < arr.GetRowCount(); i++)
        cout << arr.GetValue(i, 0).GetString() << endl;
    
    return 0;
}