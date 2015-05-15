# MysqlControl��ӿ�˵��

/*
* ����MySQL���������־�Ļص�����
* @param fun �ص�����
*/
void SetMysqlLogCallback(MYSQL_LOG_FUN fun)

/*
* ����MysqlControl�Ĵ�����־�Ļص�����
* @param fun �ص�����
*/
void SetControlLogCallback(CONTROL_LOG_FUN fun)

/*
* ��MYSQL�������� 
* @param host ��mysql_real_connect������ͬ������һ��
* @param user ��mysql_real_connect������ͬ������һ��
* @param password ��mysql_real_connect������ͬ������һ��
* @param db ��mysql_real_connect������ͬ������һ��
* @param port ��mysql_real_connect������ͬ������һ��
* @param client_flag ��mysql_real_connect������ͬ������һ��
* @param reconnect �����ӶϿ�ʱ���Ƿ��Զ�����
* @return �ɹ�����true�����򷵻�false
*/
bool RealConnect(const char *host, const char *user,  const char *password, const char *db = NULL, unsigned int port = 0, const char *unix_socket = NULL,  unsigned long client_flag = 0, bool reconnect = true)

/*
* �ı䵱ǰ���û�  
* @param host ��mysql_change_user������ͬ������һ��
* @param user ��mysql_change_user������ͬ������һ��
* @param password ��mysql_change_user������ͬ������һ��
* @param db ��mysql_change_user������ͬ������һ��
* @return �ɹ�����true�����򷵻�false
*/
bool ChangeUser(const char *host, const char *user, const char *password, const char *db = NULL)

/*
* ��ս�����������������ָ����Ϊ��  
*/
void ClearResult()

/*
* ִ��һ��SQL���
* @param SQL��䣬������printf�������÷�һ��  
* @return ���ظñ��ܵ�Ӱ�������  
*/
int Query(const char *order, ...)

/*
* ִ��һ��SQL���
* @param SQL���
* @return ���ظñ��ܵ�Ӱ�������  
*/
int Query(const string &order)

/*
* ִ��һ��SQL��䣬������ѯ�Ľ������������
* @param SQL��䣬������printf�������÷�һ��  
* @return ���ؽ����������  
*/
int QueryAndStore(const char *order, ...)

/*
* ִ��һ��SQL��䣬������ѯ�Ľ������������
* @param SQL���
* @return ���ؽ���������� 
*/
int QueryAndStore(const string &order)

/*
* ��ȡ�����������
* @return ���ؽ���������� 
*/  
int GetRowCount()

/*
* ���ؽ����������
* @return ���ؽ���������� 
*/  
int GetColumnCount()


/*
* ��ȡĳ����Ԫ��Ľ����������Ϊ����ֵ  
* @param row Ŀ����
* @param column Ŀ����
* @return Ŀ��ĵ�Ԫ������
*/
Field GetOneNode(int row, int column)

/*
* ��ȡȫ��������������DoubleDArray 
*/
void GetAllResult(DoubleDArray<Field> &data)

/*
* ��ȡһ�н�����������DoubleDArray  
* @param Ŀ����
*/
void GetOneResult(DoubleDArray<Field> &data, int row)

/*
* ��ȡ������еı�ͷ������  
/*
void GetHeaderName(vector<string> &vec)