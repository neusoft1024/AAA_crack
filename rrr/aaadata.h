#include<string>
#include "md5.h"

static int findpos(string str,string flag)
{
    for(int i=0;i<str.length();i++)
    {
        for(int j=0;j<flag.length();j++)
        {
            if(str[i+j]!=flag[j])
            {
                break;
            }
            if(j==flag.length()-1)
            {
                return i;
            }
        }
    }
    return -1;
}
static string getxmlvalue(string xmlstr,string flag)
{
    int pos=findpos(xmlstr,flag);
    int ed=findpos(xmlstr.c_str()+pos+1,flag);
//    cout<<xmlstr<<" "<<flag<<endl;
//    cout<<pos<<" "<<ed<<endl;
    if(pos==-1||ed==-1)
    {
        return "";
    }
    while(xmlstr[pos]!='>')
    {
        pos++;
        ed--;
    }
    pos++;
    ed-=2;
    string result="";
    for(int i=0;i<ed;i++)
    {
        result+=xmlstr[pos+i];
    }
    return result;
}
static void mreplace(string &str,const string flag,const string &re)
{
    int pos=findpos(str,flag);
    str.replace(pos,flag.length(),re);
}


class aaadata
{
public:
    string vcs;
    string errorinfo;
    string cname;
    string mac;
    string vos;
    string uid;
    string upass;
    string localip;
    string loginxml;
    string keepsession;
    string httppost;
    string token;//!!

	//update
	string adpost;
	string adxml;
	//

    aaadata()
    {
        readinfo("aaadata/Info.txt");

        readall("aaadata/Login.xml",loginxml);
        readall("aaadata/KeepSession.xml",keepsession);

        readall("aaadata/HttpPost.txt",httppost);
        /*httppost="";
        httppost+="POST /NSUAAAWS/Default.asmx HTTP/1.1\r\n";
        httppost+="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; MS Web Services Client Protocol 4.0.30319.18052)\r\n";
        httppost+="Content-Type: text/xml; charset=utf-8\r\n";
        httppost+="SOAPAction: \"http://tempuri.org/%s\"\r\n";
        httppost+="Host: 100.0.0.10\r\n";
        httppost+="Content-Length: %d\r\n";
        httppost+="Expect: 100-continue\r\n\r\n";*/

		//update
		adpost=httppost+="Connection: Keep-Alive\r\n\r\n";
		readall("aaadata/AD.xml",adxml);
		//
    }

    void readinfo(string fadd)
    {
        char buf[256];
        int len=0;
        int rlen=0;
        FILE *f;
        f=fopen(fadd.c_str(),"r");

        fscanf(f,"%s",buf);
        vcs=buf;
        fscanf(f,"%s",buf);
        errorinfo=buf;
        fscanf(f,"%s",buf);
        cname=buf;
        fscanf(f,"%s",buf);
        mac=buf;
        fscanf(f,"%s",buf);
        vos=buf;
        fscanf(f,"%s",buf);
        uid=buf;
        fscanf(f,"%s",buf);
        upass=buf;

        fclose(f);

    }

    void readall(string fadd,string &objxml)
    {
        char buf[20480];
        int len=0;
        int rlen=0;
        FILE *f;
        f=fopen(fadd.c_str(),"r");
        while(!feof(f))
        {
            rlen=fread(buf+len,1,1,f);
            len+=rlen;
        }
        fclose(f);
        buf[len]='\0';
        objxml=buf;
    }


    string nowpost;
	//update
	string mkaddate()
	{
		string str=adxml;
		char s[512];
		sprintf(s,httppost.c_str(),"GetAD",str.length());
		nowpost=s;
		return str;
	}
	//
    string mklogindata()
    {
        string str=loginxml;
        mreplace(str,"$ErrInfo",errorinfo);
        mreplace(str,"$UserID",uid);
        mreplace(str,"$UserPW",upass);
        mreplace(str,"$UserIP",localip);
        mreplace(str,"$ComputerName",cname);
        mreplace(str,"$MAC",mac);
        mreplace(str,"$IsAutoLogin","false");
        mreplace(str,"$ClientVersion",vcs);
        mreplace(str,"$OSVersion",vos);
        char s[512];
        sprintf(s,httppost.c_str(),"Login",str.length());
        nowpost=s;
		return str;
    }
    string mkkeepdata()
    {
        string str=keepsession;
        mreplace(str,"$UserID",uid);
        mreplace(str,"$UserIP",localip);
        mreplace(str,"$Token",token);
        //mreplace(str,"$IsHaveNewMessage","");
        comptoken();
        char s[512];
        sprintf(s,httppost.c_str(),"KeepSession",str.length());
        nowpost=s;
        return str;
    }
	//update

	bool isad(string str)
	{
		/*
		string str;
		if (str==getxmlvalue(str,"GetADResult"))
		{
			cout<<str<<endl;
			return true;
		}
		return false;
		*/
	}

	//
    bool islogin(string str)
    {
        if("true"==getxmlvalue(str,"IsLogin"))
        {
			cout<<"~~~~~~~IsLogin"<<endl;
            return true;
        }
        return false;
    }
    bool iskeep(string str)
    {
        if("true"==getxmlvalue(str,"KeepSessionResult"))
        {
            return true;
        }
        return false;
    }

    void getlogintoken(string str)
    {
        token=getxmlvalue(str,"Token");
        comptoken();
        comptoken();
    }
    void comptoken()
    {
        token=md5(token);
        for(int i=0;i<token.length();i++)
        {
            if(token[i]>='a'&&token[i]<='z')
            {
                token[i]-=32;
            }
        }
    }
};
