#pragma comment(lib, "ws2_32.lib")
#include "msock.h"
#include "aaadata.h"

void mdebug(string str)
{
    //return;
    cout<<str<<endl;
}

aaadata adata;
bool isrun;

string sdata;
char rdata[2048];

//update

bool getAD(msock_tcp &sock)
{
	sdata=adata.mkaddate();
//mdebug(adata.nowpost);

	sock.msend(adata.nowpost);
	sock.mrecv(rdata);
//mdebug(rdata);


	sock.msend(sdata);
	sock.mrecv(rdata);

//mdebug(rdata);
	/*
	if (!adata.isad(rdata))
	{
		//return false;
	}
	*/

	return true;
}

//

bool login(msock_tcp &sock)
{
    sdata=adata.mklogindata();
//mdebug(sdata);
    sock.msend(adata.nowpost);
    sock.mrecv(rdata);

//mdebug(rdata);

    sock.msend(sdata);
	sock.mrecv(rdata);
//mdebug(rdata);

	if(!adata.islogin(rdata))
    {
        return false;
    }
//
	//¹ã¸æ
	sock.mrecv(rdata);
//	mdebug(rdata);
	sock.mrecv(rdata);
//	mdebug(rdata);
	
//
	//system("pause");
	//cout<<"~~~~~~~~~~"<<endl;


    adata.getlogintoken(rdata);
//mdebug(adata.token);
    return true;
}
bool keep(msock_tcp &sock)
{
    sdata=adata.mkkeepdata();
//mdebug(sdata);
    sock.msend(adata.nowpost);
    sock.mrecv(rdata);
//mdebug(rdata);
//



//
    sock.msend(sdata);
    sock.mrecv(rdata);
//mdebug(rdata);    


	if(!adata.iskeep(rdata))
    {
        return false;
    }
	
//mdebug(rdata);
	
    return true;
}

int main()
{
    if(networkinit())
    {
        puts("network init error");
        return -1;
    }
    char *name=new char[128];
	gethostname(name,128);
	hostent*pHost = gethostbyname(name);
	int k=0;
	while(pHost->h_addr_list[k]!=NULL)
    {
       name = inet_ntoa(*((in_addr *)pHost->h_addr_list[k]));
       adata.localip=name;
       if(adata.localip[1]=='0')
       {
           break;
       }
       k++;
    }

    msock_tcp sock;
    sock.setip("100.0.0.10");
    sock.setport(80);
    puts("connect start");
    if(SOCKET_ERROR==sock.mconnect())
    {
        puts("connect error");
        return -1;
    }
    puts("connect secc");

    int cnt=4;
	getAD(sock);
	Sleep(5000);
    while(!login(sock)&&cnt)
    {
        puts("login fail, recry..");
        Sleep(1000);
        cnt--;
    }
    isrun=cnt;
    if(isrun)
    {
        puts("login secc");
    }
    cnt=0;
    while(isrun)
    {
        Sleep(10000);
        isrun=keep(sock);
        printf("keep secc: %05d\n",cnt++);
        if(cnt>100000)
        {
            cnt=0;
        }
    }


    puts("rrr end");
    system("pause");
	networkclose();

    //cout<<getxmlvalue(adata.loginxml,"UserPW");

}
