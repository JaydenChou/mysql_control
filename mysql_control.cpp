/*
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mysql_control.h" 
#include <cstring>
#include <cstdarg>
 

MysqlControl::MysqlControl()  
{  
    //为MYSQL连接分配空间，并对其初始化  
    m_Connection = new MYSQL;  
    mysql_init(m_Connection);  

    m_Result = NULL;  
    m_Field = NULL;
    m_Connected = false;  
    m_RowCount = m_ColumnCount = 0;  
}  

MysqlControl::~MysqlControl()  
{  
    //清空结果集，关闭连接，释放空间  
    ClearResult();  
    mysql_close(m_Connection);  
    delete m_Connection;  
}  

void MysqlControl::OutputError(const string &str)  
{  
    if( mysql_errno(m_Connection) == 0 )  
    {  
        fprintf(stderr, "%s\n", str.c_str());  
        return;  
    }  
    fprintf(stderr, "%s : %d: %s\n", str.c_str(),  
        mysql_errno(m_Connection), mysql_error(m_Connection));  
}  

bool MysqlControl::RealConnect(const string &host,  
                                 const string &user,  
                                 const string &password,   
                                 const string &db,  
                                 unsigned int port,  
                                 const string &unix_socket,  
                                 unsigned long client_flag,
                                 bool reconnect)
{  
    if( m_Connected )
        return true;

    //因为unix_socket=NULL时，是有特殊意义的，所以要特别处理一下  
    char *un_sock;  
    if( unix_socket.empty() )  
        un_sock = NULL;  
    else  
        un_sock = (char*)unix_socket.c_str();  

    //连接数据库，设置标志(connected)  
    m_Connection = mysql_real_connect(m_Connection, host.c_str(),   
        user.c_str(), password.c_str(), db.c_str(), port,   
        un_sock, client_flag);  

    if( m_Connection == NULL )  
    {  
        OutputError("real connect fail");  
        return false;  
    }  

    //设置自动重连
    if( reconnect )
    {
        char value = 1;
        mysql_options(m_Connection, MYSQL_OPT_RECONNECT, (char *)&value);
    }
    m_Connected = true;  
    return true;  
}  

bool MysqlControl::ChangeUser(const string &host, const string &user,   
                                const string &password, const string &db/* = */)  
{  
    assert(m_Connected);  
    int res = mysql_change_user(m_Connection, user.c_str(),   
        password.c_str(), db.c_str());  
    if( res != 0 )  
    {  
        OutputError("change user fail");  
        return false;  
    }  
    return true;  
}  

void MysqlControl::ClearResult()  
{  
    if( m_Result == NULL )  
        return;  

    //清空结果集，并将结果集的指针置为空  
    mysql_free_result(m_Result);  
    m_Result = NULL;  
    m_RowCount = 0;  
    m_ColumnCount = 0;  
}  

int MysqlControl::Query(const char *order, ...)
{  
    assert(m_Connected);

    va_list list;    
    va_start(list, order); 
    m_sql = StringFormat(order, list);
    va_end(list);

    //执行SQL语句  
    int res = mysql_query(m_Connection, m_sql.c_str());  
    if( res != 0 )  
    {  
        OutputError("sql error");  
        return -1;  
    }  

    //将影响的行数作为返回值  
    res = mysql_affected_rows(m_Connection);  
    return res;  
}  

int MysqlControl::QueryAndStore(const char *order, ...)  
{  
    assert(m_Connected);  
    //先将结果集清空  
    ClearResult();  

    va_list list;    
    va_start(list, order); 
    m_sql = StringFormat(order, list);
    va_end(list);

    //执行SQL语句  
    int res = mysql_query(m_Connection, m_sql.c_str()); 
    if( res != 0 )  
    {  
        OutputError("sql error");  
        return -1;  
    }  

    //将my_res设为当前的结果集和行列数  
    m_Result = mysql_store_result(m_Connection);
    m_Field = mysql_fetch_field(m_Result);
    if( NULL == m_Result )  
        return -1;  
    m_RowCount = mysql_num_rows(m_Result);  
    m_ColumnCount = mysql_num_fields(m_Result);  

    return m_RowCount;  
}  

Field MysqlControl::GetOneNode(int row, int column)  
{  
    Field result;
    if (NULL == m_Result)
        return result;

    if( row >= m_RowCount ||  
        column >= m_ColumnCount )  
        return result;  

    mysql_data_seek(m_Result, row);  
    MYSQL_ROW my_row = mysql_fetch_row(m_Result);  
    result.SetStructuredValue(my_row[column], m_Field[column].type);
    return result;  
}  


//////////////////////////////////////////////////////////////////////////

void MysqlControl::GetAllResult(DoubleDArray<Field> &data)
{  
    data.ClearArray();

    if( NULL == m_Result )
        return;

    MYSQL_ROW my_row;
    data.InitArray(m_RowCount, m_ColumnCount);

    //将光标设为第一行  
    mysql_data_seek(m_Result, 0);  

    for(int i=0; i<m_RowCount; i++)  
    {  
        //获取光标所在行的那一行数据（的指针）  
        my_row = mysql_fetch_row(m_Result);  
        for(int j=0; j<m_ColumnCount; j++)  
        {  
            Field field;
            field.SetStructuredValue(my_row[j], m_Field[j].type);
            data.SetValue(i, j, field);
        }  
    }  
}  

void MysqlControl::GetOneResult(DoubleDArray<Field> &data, int row)
{  
    data.ClearArray();

    if( NULL == m_Result )
        return;
 
    if( row >= m_RowCount )  
        return;  

    data.InitArray(1, m_ColumnCount); 

    //将光标放置到目标行  
    mysql_data_seek(m_Result, row);  
    MYSQL_ROW my_row = mysql_fetch_row(m_Result);  

    //将结果存入vector容器  
    for(int i=0; i<m_ColumnCount; i++)  
    {  
        Field field;
        field.SetStructuredValue(my_row[i], m_Field[i].type);
        data.SetValue(0, i, field);
    }  
}  

void MysqlControl::GetHeaderName(vector<string> &vec)  
{  
    if( NULL == m_Result )
        return;

    vec.resize(m_ColumnCount);  
    MYSQL_FIELD *field;  

    //获取表头名字，存入vector容器  
    field = mysql_fetch_fields(m_Result);  
    for(int i=0; i<m_ColumnCount; i++)  
        vec[i] = field[i].name;  
}  

string MysqlControl::StringFormat(const char *format, ...)
{
    string result;
    //将字符串的长度初始化为1024  
    int tlen = 1024;
    int len = 0;
    result.resize(tlen);

    //为string赋值  
    va_list list;
    va_start(list, format);
    len = vsnprintf((char*)result.c_str(), tlen, format, list);

    //如果结果字符串长度比初始长度长，就重新设置字符串长度，然后再赋值  
    if (len >= tlen)
    {
        tlen = len + 1;
        result.resize(tlen);
        len = vsnprintf((char*)result.c_str(), tlen, format, list);
    }
    va_end(list);

    //删除字符串尾部的0字符  
    result.erase(len);
    return result;
}