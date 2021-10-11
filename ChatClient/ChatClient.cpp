#include "ChatClient.h"

ChatClient::ChatClient(tcp::socket sock)
:m_clientSocket(std::move(sock))
{}

void ChatClient::Start()
{
    //do read after start
    DoRead();
}

void ChatClient::DoRead()
{
    auto self=shared_from_this();
    //error symbol
    boost::system::error_code ec;
    //clear the first recevied buffer
    memset(m_oneBuffer,0,1024);
    m_clientSocket.async_read_some
    (
        boost::asio::buffer(m_oneBuffer,1024),
        [this,self](std::error_code ec,size_t length)
        {

            //length indicate the lenth of recevied message
            if(!ec)
            {
                //copy the message from first buffer to a big buffer;
                //need improve
                memcpy(m_cBuffer+m_endPosOfBuffer,m_oneBuffer,length);
                //updte the position of buffer end
                m_endPosOfBuffer+=length;
                //if the message length bigger than the sizeof lengthinfo struct
                //we can get the length of package
                if(m_endPosOfBuffer>=sizeof(LengthInfo))
                {
                    //read the lengthinfo struct
                    char headBuffer[sizeof(LengthInfo)]{0};
                    memcpy(headBuffer,m_cBuffer,sizeof(LengthInfo));
                    //cast type and get length of package
                    int lengthOfMessage=reinterpret_cast<LengthInfo*>(&headBuffer)->length;
                    //printf("message length: %d\n",lengthOfMessage);
                    //if buffer length bigger than package length,read it
                    if(m_endPosOfBuffer>=sizeof(LengthInfo)+lengthOfMessage)
                    {
                        //copy it from buffer
                        char message[lengthOfMessage+1]{0};
                        memcpy(message,m_cBuffer+sizeof(LengthInfo),lengthOfMessage);
                        std::string test(message);
                        printf("%s\n",test.c_str());
                        //cover the message already read from buffer
                        memcpy(m_cBuffer,m_cBuffer+sizeof(LengthInfo)+lengthOfMessage,BUFFERLENGTH-(sizeof(LengthInfo)+lengthOfMessage));
                        //update end of buffer
                        m_endPosOfBuffer-=(sizeof(LengthInfo)+lengthOfMessage);
                        //TODO prase json string and do corresponding operation

                        //the message type is json
                        //construct a ptree object,used for prase json
                        ptree pt;
                        std::stringstream ss(test);
                        read_json(ss,pt);

                        //after prase, we can use pt to get message

                        int imessageType=pt.get<int>("type");
                        printf("type is::%d\n",imessageType);
                        //according to the message type,we can do corresponding operation

                        switch (imessageType)
                        {
                        case static_cast<message_type_base_type>(MESSAGE_TYPE::ADD_FRIEND):
                            break;
                        
                        default:
                            break;
                        }

                        DoRead();
                        return;
                    }
                    //if the length of buffer is smaller than length of package
                    //continue doread();
                    else
                    {
                        DoRead();
                        return;
                    }
                }
                //if the length of buffer is smaller than length of lengthinfo struct
                //continue doread
                else
                {
                    DoRead();
                    return;
                }
            }
            //read has error
            //seem as disconnect temperary//TODO
            else
            {
                //TODO read error and do something
                printf("connection disconnected\n");
                return;
            }
        }
    );
}

bool praseJsonString(std::string& message,ptree& pt)
{
    //transfer to stream
    std::stringstream ss(message);

    //read json type
    read_json(ss,pt);
}