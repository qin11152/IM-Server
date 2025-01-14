/*
 * @Author: qin11152 1052080761@qq.com
 * @Date: 2022-06-18 13:57:30
 * @LastEditors: qin11152 1052080761@qq.com
 * @LastEditTime: 2023-04-02 15:04:07
 * @FilePath: /src/IM-Server/protocol/GetFriendListReplyData.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "GetFriendListReplyData.h"

namespace protocol
{
    GetFriendListReplyData::GetFriendListReplyData(const std::string& message)
    {
        parse(message);
    }

    void GetFriendListReplyData::parse(const std::string& message)
    {
        if (message.empty())
        {
            return;
        }
        ptree m_ptree;
        ptree m_friendIdList;
        ptree m_friendNamelist;
        std::stringstream sstream(message);
        read_json(sstream, m_ptree);
        //json中有两个数组，一个数组存放id，一个数组存放name，两者顺序对应，保证属于同一个friend
        m_friendIdList=m_ptree.get_child("friendIdList");
        m_friendNamelist=m_ptree.get_child("friendNameList");
        for(auto iter=m_friendIdList.begin();iter!=m_friendIdList.end();++iter)
        {
            //遍历数组得到对象，获取对象的id
            auto item=iter->second;
            //构建friendinfo对象
            FriendInfo tmp;
            tmp.m_strFriendId=item.get<std::string>("");
            //存储在vec中
            m_vecFriendList.push_back(tmp);
        }
        int i=0;//i为当前迭代器对应的vec中的位置
        for(auto iter=m_friendNamelist.begin();iter!=m_friendNamelist.end();++iter,++i)
        {
            //把对应的名字也解析了
            auto item=iter->second;
            m_vecFriendList[i].m_strFriendName=item.get<std::string>("");
        }

        return;
    }

    std::string GetFriendListReplyData::generateJson()
    {
        ptree m_ptree,m_friendIdList,m_friendNameList;
        m_ptree.put("type",static_cast<int>(m_strType));

        int cnt=0;
        ptree ListNode;
        for(auto &item:m_vecFriendList)
        {
            cnt++;
    #if 0
            ptree child1;
            child1.put("",item.m_strFriendId);
            m_friendIdList.push_back(std::make_pair("",child1));
            ptree child2;
            child2.put("",item.m_strFriendName);
            m_friendNameList.push_back(std::make_pair("",child2));
    #endif
            ptree tmpNode;
            tmpNode.put("id",item.m_strFriendId);
            tmpNode.put("name",item.m_strFriendName);
            tmpNode.put("imageTimeStamp",item.m_strImageTimeStamp);
            ListNode.push_back(std::make_pair("",tmpNode));
        }

        m_ptree.add_child("friendInfoList",ListNode);

    #if 0
        m_ptree.add_child("FriendIdList",m_friendIdList);
        m_ptree.add_child("FriendNameList",m_friendNameList);
    #endif

        std::stringstream sstream;
        write_json(sstream,m_ptree);
        return sstream.str();
    }
} // namespace protocol