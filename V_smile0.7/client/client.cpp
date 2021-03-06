#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

using namespace std;
using namespace Json;

#define PORT 40000
#define IP  "192.168.181.128"

int main(void)
{
	int client;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if(client == -1)
	{
		cout<<"create client error"<<endl;
		return 0;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);

	connect(client,(struct sockaddr*)&addr, sizeof(struct sockaddr));

	Value value,value1;
	FastWriter writer;
	value["typecode"] = Value(0);
	value["name"] = Value("hubian");
	value["paswd"] = Value("who");

	//value1["typecode"] = 
	string strdata = writer.write(value);

	cout<<"send data:"+strdata<<endl;

	int datalen = strdata.length();
	char* data = (char*)malloc(datalen);
	strdata.copy(data,datalen,0);
	
	send(client,data,datalen,0);
	sleep(1);
/*
	value1["typecode"] = Value(60);
	value1["sender"] = Value("zhaokai");
	value1["friend"] = Value("finder");
	strdata = writer.write(value1);
	datalen = strdata.length();
	strdata.copy(data,datalen,0);
	send(sock,data,datalen,0);
	//finish testing for login
	//send(sock,data,16,0);		//send content of data to serverr
	char dat [50];		//create array data as buffer
	memset(dat,'\0',50);		//set data to '\0'
	recv(sock,dat,50,0);		//receive content form server and store in data
	cout<<dat<<endl;		//cout data 
*/
	free(data);
	close(client);		//close socket
}

