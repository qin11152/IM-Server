#include "MysqlQuery.h"
#include "Log.h"
#include "../PublicFunction.hpp"

std::mutex MysqlQuery::m_mutex;
std::shared_ptr<MysqlQuery> MysqlQuery::m_ptrInstance={nullptr};

const char* kIp="127.0.0.1";
const char* kDataBase="chat_data";
const char* kUser="root";
const char* kPassword="qinbiao9704";

std::shared_ptr<MysqlQuery> MysqlQuery::Instance()
{
    if (m_ptrInstance == nullptr)
    {
        std::lock_guard<std::mutex> lck(m_mutex);
        if (m_ptrInstance == nullptr)
        {
            m_ptrInstance = std::shared_ptr<MysqlQuery>(new MysqlQuery(kIp,kDataBase,kUser,kPassword));
        }
    }
    return m_ptrInstance;
}

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
        _LOG(Logcxx::ERROR,"error occurs: %s",mysql_error(m_mysql));
    }else{
        //printf("connect successfully\n");
    }
    mysql_query(m_mysql,"set names utf-8");
}

int MysqlQuery::GetCurrentUserCount()
{
    std::string query="select count(*) from user_info";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select count(*) from user_info failed");
    }
    m_mysqlQueryResult=mysql_store_result(m_mysql);
    m_mysqlRow=mysql_fetch_row(m_mysqlQueryResult);
    int tmp=atoi(m_mysqlRow[0]);
    mysql_free_result(m_mysqlQueryResult);
    return tmp;
}

int MysqlQuery::InsertNewUser(const std::string name,const std::string&password,
                            const std::string iconUrl,const std::string& signature)
{
    //std::string exePahth=getCurrentDir();
    //刚开始时默认头像就是设定的那个图片
    std::string profileImagePath="./data/profileImage/DefaultImageBase64.txt";
    //获取当前时间转换为字符串
    std::string time=getCurrentTime();
    int id=GetCurrentUserCount()+1;
    std::string query="insert into user_info (id,password,name,signature,icon,online,image,imagetimestamp) values("
    +std::to_string(id)+",\""+password+"\",\""+name+"\",\""+signature+
    "\",\""+iconUrl+"\",false,\""+profileImagePath+"\",\""+time+"\");";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"insert into user_info failed,query is:%s",query.c_str());
        return -1;
    }
    return id;
}

bool MysqlQuery::VertifyPassword(int id,const std::string& password)
{
    std::string query="select * from user_info";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select * from user_info failed");
        return false;
    }
    m_mysqlQueryResult=mysql_store_result(m_mysql);
    m_mysqlRow=mysql_fetch_row(m_mysqlQueryResult);
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

bool MysqlQuery::AddFriend(std::string friend_1,std::string friend_2,std::string friend1name,std::string friend2name)
{
    std::string query="insert into friend_info values(\""+friend_1+"\",\""+friend_2+"\",\""+friend2name+"\")";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"insert into friend_info failed,query is:%s",query.c_str());
        return false;
    }
    query="insert into friend_info values(\""+friend_2+"\",\""+friend_1+"\",\""+friend1name+"\")";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"insert into friend_info failed,query is:%s",query.c_str());
        return false;
    } 
    return true;
}

bool MysqlQuery::queryUserIsOnline(std::string userId)
{
    bool onlineState={false};
    std::string query="select online from user_info where id="+userId;
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"insert into friend_info failed,query is:%s",query.c_str());
        return false;
    }
    MYSQL_RES* res=nullptr;
    //将查询的结果存储在res中
    res=mysql_store_result(m_mysql);
    //获取结果中的行数
    int rowCount=mysql_num_rows(res);
    //获取结果中的列数
    int colCount=mysql_num_fields(res);

    //当还有列的时候就一直获取
    //获取行
    MYSQL_ROW rowPtr=nullptr;
    //有就一直获取
    while(rowPtr=mysql_fetch_row(res))
    {
        //可以通过循环获取每一行的内容，每一行中
        //列的名称在上边已经进行了获取
        //这里只有一行一列
        int tmpi=atoi(rowPtr[0]);
        onlineState=tmpi==0?false:true;
    }
    mysql_free_result(res);
    return onlineState;
}

bool MysqlQuery::updateUserOnlineState(std::string userId,bool onlineState)
{
    std::string strOnlineState=onlineState?"1":"0";
    std::string query="update user_info set online="+strOnlineState+" where id="+userId;
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"update user_info set online failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}

bool MysqlQuery::queryUserIsExist(std::string userId)
{
    bool isExist=false;
    std::string query="select count(*) from user_info where id="+userId;
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select count(*) from user_info failed,query is:%s",query.c_str());
        return false;
    }
    MYSQL_RES* res=nullptr;
    //将查询的结果存储在res中
    res=mysql_store_result(m_mysql);
    //获取结果中的行数
    int rowCount=mysql_num_rows(res);

    //获取行
    MYSQL_ROW rowPtr=nullptr;
    //有就一直获取
    while(rowPtr=mysql_fetch_row(res))
    {
        //可以通过循环获取每一行的内容，每一行中
        //列的名称在上边已经进行了获取
        //这里只有一行一列
        isExist=rowPtr[0]==0?false:true;
    }
    mysql_free_result(res);
    return isExist;
}

bool MysqlQuery::insertAddFriendCache(const std::string& requestId,const std::string& destinationId,const std::string& verifyMsg)
{
    std::string query="insert into add_friend values(\""+requestId+"\",\""+destinationId+"\",\""+verifyMsg+"\")";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"insert into add_friend failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}

bool MysqlQuery::queryCachedAddFriendInfo(std::vector<MyAddFriendInfo>& vecFriednInfo,std::string& id)
{
    std::string query="select * from add_friend where myId=\""+id+"\"";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select * from add_friend failed,query is:%s",query.c_str());
        return false;
    }

    MYSQL_RES* res=nullptr;
    //将查询的结果存储在res中
    res=mysql_store_result(m_mysql);
    //获取结果中的行数
    int rowCount=mysql_num_rows(res);
    //获取结果中的列数
    int colCount=mysql_num_fields(res);
    //存储列
    MYSQL_FIELD* pField=nullptr;
    //当还有列的时候就一直获取
    //我们这里不需要列的信息
    //while(pField=mysql_fetch_field(res))
    //{
         //可以存储列的名称
         //操作是pField->name;
    //}
    //获取行
    MYSQL_ROW rowPtr=nullptr;
    //有就一直获取
    while(rowPtr=mysql_fetch_row(res))
    {
        //可以通过循环获取每一行的内容，每一行中
        //这里一行两列
        //类对象存储好友信息
        MyAddFriendInfo tmp;
        tmp.m_strFriendId=rowPtr[0];
        tmp.m_strMyId=rowPtr[1];
        tmp.m_strVerifyMsg=rowPtr[2];
        vecFriednInfo.push_back(tmp);
    }
    mysql_free_result(res);
    return true;
}

bool MysqlQuery::deleteCachedAddFriendInfo(std::string& id)
{
    std::string query="delete from add_friend where myId=\""+id+"\"";
    if(!mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"delete from add_friend failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}

bool MysqlQuery::queryCachedChatMsg(std::vector<MyChatMessageInfo>& vecFriednInfo,std::string& id)
{
    std::string query="select * from chat_message_cache where toid=\""+id+"\"";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select * from chat_message_cache failed,query is:%s",query.c_str());
        return false;
    }

    MYSQL_RES* res=nullptr;
    //将查询的结果存储在res中
    res=mysql_store_result(m_mysql);
    //获取结果中的行数
    int rowCount=mysql_num_rows(res);
    //获取结果中的列数
    int colCount=mysql_num_fields(res);
    //存储列
    MYSQL_FIELD* pField=nullptr;
    //当还有列的时候就一直获取
    //我们这里不需要列的信息
    //while(pField=mysql_fetch_field(res))
    //{
         //可以存储列的名称
         //操作是pField->name;
    //}
    //获取行
    MYSQL_ROW rowPtr=nullptr;
    //有就一直获取
    while(rowPtr=mysql_fetch_row(res))
    {
        //可以通过循环获取每一行的内容，每一行中
        //这里一行两列
        //类对象存储好友信息
        MyChatMessageInfo tmp;
        tmp.m_strFromId=rowPtr[0];
        tmp.m_strToId=rowPtr[1];
        tmp.m_strChatMsg=rowPtr[2];
        tmp.m_strSendName=rowPtr[3];
        vecFriednInfo.push_back(tmp);
    }
    mysql_free_result(res);
    return true;
}

bool MysqlQuery::insertCachedChatMsg(std::string& fromId,std::string& toId,std::string& msg,std::string& sendName,std::string& time)
{
    std::string query="insert into chat_message_cache values(\""+fromId+"\",\""+toId+"\",\""+msg+"\",\""+sendName+"\",\""+time+"\")";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"insert into chat_message_cache failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}

bool MysqlQuery::deleteCachedChatMsg(std::string& id)
{
    std::string query="delete from chat_message_cache where toId=\""+id+"\"";
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"delete from chat_message_cache failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}

void MysqlQuery::queryUserFrinedList(std::vector<FriendInfo>& vecFriendList,std::string& strUserId)
{
    //printf("friend list user id:%s\n",strUserId.c_str());
    //std::string query="select id_friend,name from friend_info where id_my="+strUserId;
    std::string query="select id_friend, name from friend_info where id_my=\""+strUserId+"\" order by CONVERT( name USING gbk) COLLATE gbk_chinese_ci ASC";
    //printf("%s\n",query.c_str());
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select id_friend, name from friend_info failed,query is:%s",query.c_str());
        return;
    }
    MYSQL_RES* res=nullptr;
    //将查询的结果存储在res中
    res=mysql_store_result(m_mysql);
    //获取结果中的行数
    int rowCount=mysql_num_rows(res);
    //获取结果中的列数
    int colCount=mysql_num_fields(res);
    //存储列
    MYSQL_FIELD* pField=nullptr;
    //当还有列的时候就一直获取
    //我们这里不需要列的信息
    //while(pField=mysql_fetch_field(res))
    //{
         //可以存储列的名称
         //操作是pField->name;
    //}
    //获取行
    MYSQL_ROW rowPtr=nullptr;
    //有就一直获取
    while(rowPtr=mysql_fetch_row(res))
    {
        //可以通过循环获取每一行的内容，每一行中
        //这里一行两列
        //类对象存储好友信息
        FriendInfo tmp;
        tmp.m_strFriendId=rowPtr[0];
        tmp.m_strFriendName=rowPtr[1];
        vecFriendList.push_back(tmp);
    }
    mysql_free_result(res);
    return;
}

std::string MysqlQuery::queryUserNameAcordId(const std::string& id)
{
    std::string query="select name from user_info where id="+id;
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select name from user_info failed,query is:%s",query.c_str());
        return "";
    }
    std::string name="";
    MYSQL_RES* res=nullptr;
    //将查询的结果存储在res中
    res=mysql_store_result(m_mysql);
    //获取结果中的行数
    int rowCount=mysql_num_rows(res);

    //获取行
    MYSQL_ROW rowPtr=nullptr;
    //有就一直获取
    while(rowPtr=mysql_fetch_row(res))
    {
        //可以通过循环获取每一行的内容，每一行中
        //列的名称在上边已经进行了获取
        //这里只有一行一列
        name=rowPtr[0];
    }
    mysql_free_result(res);
    return name;
}

bool MysqlQuery::updateImagePathAcordId(const std::string& id,const std::string strIamgePath)
{
    std::string curTime=getCurrentTime();
   std::string query="update user_info set image=\'"+strIamgePath+"\', imagetimestamp=\'"+curTime+"\' where id="+id; 
   if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"update user image failed,query is:%s",query.c_str());
        return false;
    }
    return true;
}

std::string MysqlQuery::queryImagePathAcordId(const std::string& id)
{
    std::string query="select image from user_info where id="+id;
    if(mysql_query(m_mysql,query.c_str()))
    {
        _LOG(Logcxx::ERROR,"select image from user_info failed,query is:%s",query.c_str());
        return "";
    }
    std::string image="";
    MYSQL_RES* res=nullptr;
    //将查询的结果存储在res中
    res=mysql_store_result(m_mysql);
    //获取结果中的行数
    int rowCount=mysql_num_rows(res);

    //获取行
    MYSQL_ROW rowPtr=nullptr;
    //有就一直获取
    while(rowPtr=mysql_fetch_row(res))
    {
        //可以通过循环获取每一行的内容，每一行中
        //列的名称在上边已经进行了获取
        //这里只有一行一列
        image=rowPtr[0];
    }
    mysql_free_result(res);
    return image;
}

MysqlQuery::~MysqlQuery(){
    mysql_close(m_mysql);
}
