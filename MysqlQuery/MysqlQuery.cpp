#include "MysqlQuery.h"

MysqlQuery::MysqlQuery(const char* ip,const char* database,const char* user,const char*password):
    m_destinationIp(ip),
    m_destinationDatabase(database),
    m_destinationUser(user),
    m_destinationPassword(password)
{
    m_mysql=mysql_init(NULL);
    initMysql();
}

void MysqlQuery::initMysql(){
    mysql_real_connect(m_mysql,m_destinationIp.c_str(),m_destinationUser.c_str(),
    m_destinationPassword.c_str(),m_destinationDatabase.c_str(),0,NULL,0);
    if(m_mysql==NULL){
        printf("error occurs: %s\n",mysql_error(m_mysql));
    }else{
        printf("connect successfully\n");
    }
    mysql_query(m_mysql,"set names utf-8");
}

int MysqlQuery::GetCurrentUserCount()
{
    std::string query="select count(*) from user_info";
    if(mysql_query(m_mysql,query.c_str()))
    {
        std::cout<<"query failed\n";
    }
    m_mysqlQueryResult=mysql_store_result(m_mysql);
    m_mysqlRow=mysql_fetch_row(m_mysqlQueryResult);
    int tmp=atoi(m_mysqlRow[0]);
    mysql_free_result(m_mysqlQueryResult);
    return tmp;
}

bool MysqlQuery::InsertNewUser(const std::string name,const std::string&password,
                            const std::string iconUrl,const std::string& signature)
{
    int id=GetCurrentUserCount()+1;
    std::string query="insert into user_info (id,password,name,signature,icon) values("
    +std::to_string(id)+",\""+name+"\",\""+password+"\",\""+signature+
    "\",\""+iconUrl+"\");";
    //std::cout<<query<<std::endl;
    if(mysql_query(m_mysql,query.c_str()))
    {
        std::cout<<"insert new failed\n";
        return false;
    }
    return true;
}

bool MysqlQuery::VertifyPassword(int id,const std::string& password)
{
    std::string query="select * from user_info";
    if(mysql_query(m_mysql,query.c_str()))
    {
        printf("query failed\n");
        return false;
    }
    m_mysqlQueryResult=mysql_store_result(m_mysql);
    m_mysqlRow=mysql_fetch_row(m_mysqlQueryResult);
    printf("%s \n",m_mysqlRow[2]);
    if(m_mysqlRow[1]==password)
    {
        mysql_free_result(m_mysqlQueryResult);
        return true;
    }
    else
    {
        mysql_free_result(m_mysqlQueryResult);
        return false;
    }
    
}

bool MysqlQuery::AddFriend(int friend_1,int friend_2)
{
    std::string query="insert into friend_info values("+std::to_string(friend_1)+","+
    std::to_string(friend_2)+")";
    if(mysql_query(m_mysql,query.c_str()))
    {
        printf("insert into friend_info failed\n");
        return false;
    }
    query="insert into friend_info values("+std::to_string(friend_2)+","+
    std::to_string(friend_1)+")";
    if(mysql_query(m_mysql,query.c_str()))
    {
        printf("insert into friend_info failed\n");
        return false;
    } 
    return true;
}

MysqlQuery::~MysqlQuery(){
    mysql_close(m_mysql);
}
