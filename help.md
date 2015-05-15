# MysqlControl类接口说明

/*
* 设置MySQL自身错误日志的回调函数
* @param fun 回调函数
*/
void SetMysqlLogCallback(MYSQL_LOG_FUN fun)

/*
* 设置MysqlControl的错误日志的回调函数
* @param fun 回调函数
*/
void SetControlLogCallback(CONTROL_LOG_FUN fun)

/*
* 与MYSQL建立连接 
* @param host 与mysql_real_connect函数的同名参数一致
* @param user 与mysql_real_connect函数的同名参数一致
* @param password 与mysql_real_connect函数的同名参数一致
* @param db 与mysql_real_connect函数的同名参数一致
* @param port 与mysql_real_connect函数的同名参数一致
* @param client_flag 与mysql_real_connect函数的同名参数一致
* @param reconnect 当连接断开时，是否自动重连
* @return 成功返回true，否则返回false
*/
bool RealConnect(const char *host, const char *user,  const char *password, const char *db = NULL, unsigned int port = 0, const char *unix_socket = NULL,  unsigned long client_flag = 0, bool reconnect = true)

/*
* 改变当前的用户  
* @param host 与mysql_change_user函数的同名参数一致
* @param user 与mysql_change_user函数的同名参数一致
* @param password 与mysql_change_user函数的同名参数一致
* @param db 与mysql_change_user函数的同名参数一致
* @return 成功返回true，否则返回false
*/
bool ChangeUser(const char *host, const char *user, const char *password, const char *db = NULL)

/*
* 清空结果集，并将结果集的指针置为空  
*/
void ClearResult()

/*
* 执行一条SQL语句
* @param SQL语句，参数与printf参数的用法一致  
* @return 返回该表受到影响的行数  
*/
int Query(const char *order, ...)

/*
* 执行一条SQL语句
* @param SQL语句
* @return 返回该表受到影响的行数  
*/
int Query(const string &order)

/*
* 执行一条SQL语句，并将查询的结果存入结果集中
* @param SQL语句，参数与printf参数的用法一致  
* @return 返回结果集的行数  
*/
int QueryAndStore(const char *order, ...)

/*
* 执行一条SQL语句，并将查询的结果存入结果集中
* @param SQL语句
* @return 返回结果集的行数 
*/
int QueryAndStore(const string &order)

/*
* 获取结果集的行数
* @return 返回结果集的行数 
*/  
int GetRowCount()

/*
* 返回结果集的列数
* @return 返回结果集列行数 
*/  
int GetColumnCount()


/*
* 获取某个单元格的结果，将其作为返回值  
* @param row 目标行
* @param column 目标列
* @return 目标的单元格数据
*/
Field GetOneNode(int row, int column)

/*
* 获取全部结果，将其存入DoubleDArray 
*/
void GetAllResult(DoubleDArray<Field> &data)

/*
* 获取一行结果，将其存入DoubleDArray  
* @param 目标行
*/
void GetOneResult(DoubleDArray<Field> &data, int row)

/*
* 获取结果集中的表头的名字  
/*
void GetHeaderName(vector<string> &vec)