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

#ifndef PUBLIC_MYSQL_CONTROL_H_
#define PUBLIC_MYSQL_CONTROL_H_  

#include <mysql.h>  

#include <iostream>  
#include <string>  
#include <vector>  
#include <cassert>  
#include <cstdio>  
#include "double_d_array.h"
#include "field.h"
using namespace std;  

class MysqlControl  
{  
private:  
    MYSQL *m_Connection;                //与MYSQL的连接句柄  
    MYSQL_RES *m_Result;                //上一次查询的结果集  
    MYSQL_FIELD *m_Field;
    bool m_DebugLog;                    //是否输出调试日志
    bool m_Connected;                    //程序是否已经和MYSQL连接了  
    int m_RowCount;                        //上一次查询的结果集的行数  
    int m_ColumnCount;                    //上一次查询的结果集的列数  
    string m_sql;                       //sql语句的缓冲区

    //发生错误时输出错误信息  
    void OutputError(const string &str); 

    //
    string GetNodeContent(MYSQL_ROW row, int index);

    //将格式化字符串赋给string，参数和printf的参数一样，返回格式化后的string类  
    string StringFormat(const char *format, ...);

private:
    MysqlControl(const MysqlControl &other) { }
    MysqlControl & operator = (const MysqlControl &) { }

public:  

    MysqlControl();  
    ~MysqlControl();  

    MYSQL *GetMysql() const { return m_Connection; }  
    MYSQL_RES *GetMysqlRes() const { return m_Result; }  

    //与MYSQL建立连接  
    bool RealConnect(const string &host, const string &user,  
        const string &password, const string &db=string(),  
        unsigned int port=0, const string &unix_socket=string(),  
        unsigned long client_flag=0, bool reconnect=true);  

    //改变当前的用户   
    bool ChangeUser(const string &host, const string &user,   
        const string &password, const string &db=string());  

    //清空结果集，并将结果集的指针置为空  
    void ClearResult();  

    //执行一条SQL语句  
    //返回值是该表受到影响的行数  
    int Query(const char *order, ...);
    int Query(const string &order)
    { return Query(order.c_str()); }

    //执行一条SQL语句，并把返回的结果放置到结果集中  
    //返回值是结果集的行数  
    int QueryAndStore(const char *order, ...);
    int QueryAndStore(const string &order)
    { return QueryAndStore(order.c_str()); }

    //返回结果集的行数或列数  
    int GetRowCount() { return m_RowCount; }  
    int GetColumnCount() { return m_ColumnCount; }  

    //返回由以前的INSERT或UPDATE语句为AUTO_INCREMENT列生成的值
    int GetInsertID()
    { return mysql_insert_id(m_Connection); }

    void SetDebugLog(bool debug) { m_DebugLog = debug; }

    //获取某个单元格的结果，将其作为返回值  
    Field GetOneNode(int row, int column);  

    //将上一次查询的结果集存入指定的容器中  
    //获取全部结果，将其存入DoubleDArray 
    void GetAllResult(DoubleDArray<Field> &data);  
    //获取一行结果，将其存入DoubleDArray  
    void GetOneResult(DoubleDArray<Field> &data, int row);

    //获取结果集中的表头的名字  
    void GetHeaderName(vector<string> &vec); 

    //返回SQL缓冲区的指针
    string GetSQLString() { return m_sql; }
};  


//提供了一个MysqlControl的单例类
class SingleMysqlControl
{
private:
    MysqlControl m_control;
    SingleMysqlControl() { }
    ~SingleMysqlControl() { }

public:
    MysqlControl *GetDate() { return &m_control; }
    static SingleMysqlControl *GetInstance()
    {
        static SingleMysqlControl instance;
        return &instance;
    }
};

#define GET_MYSQL_CONTROL SingleMysqlControl::GetInstance()->GetDate

#endif  