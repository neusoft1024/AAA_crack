#define WIN32
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>

#ifdef WIN32
#include<winsock2.h>
#else
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#endif // WIN32

using namespace std;

static int networkinit()
{
#ifdef WIN32
	WSADATA wsadata;
	return WSAStartup(MAKEWORD(1,0),&wsadata);
#endif // WIN32
	return 0;
}
static int networkclose()
{
#ifdef WIN32
    return WSACleanup();
#endif // WIN32
	return 0;
}

class msock_tcp
{
public:
    SOCKET sock;
    sockaddr_in addr;
    msock_tcp()
    {
        sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(sock==INVALID_SOCKET)
        {
            puts("socket build error");
            exit(-1);
        }

        addr.sin_family=AF_INET;
    }

    void setip(string ip)
    {
        addr.sin_addr.S_un.S_addr=inet_addr(ip.c_str());
    }
    void setport(int port)
    {
        addr.sin_port=htons(port);
    }
    int mconnect()
    {
        return connect(sock,(sockaddr *)&addr,sizeof(addr));
    }
    int msend(const char *data,int len)
    {
        return send(sock,data,len,0);
    }
    int msend(const string data)
    {
        return msend(data.c_str(),data.length());
    }
    int mrecv(char *data,int len)
    {
        return recv(sock,data,len,0);
    }
    int mrecv(char *data)
    {
        return recv(sock,data,2047,0);
    }
    int close()
    {
        #ifdef WIN32
            closesocket(sock);
        #else
            close(sock);
        #endif
    }

};
