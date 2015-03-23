#include "mysql_control.h"  
#include "double_d_array.h"
#include "field.h"
#include <string>
#include <iostream>
using namespace std;

int main(void)
{
    MysqlControl control;
    int row, column;
    int tmp;
    bool ok = control.RealConnect("localhost", "root", "aaaaaaa", "auth");
    if (!ok)
    {
        cout << "connect error" << endl;
        return 1;
    }
    control.QueryAndStore("");
    DoubleDArray<Field> arr;
    control.GetAllResult(arr);
    for (size_t i = 0; i < arr.GetRowCount(); i++)
        cout << arr.GetValue(i, 0).GetString() << endl;
    
    return 0;
}