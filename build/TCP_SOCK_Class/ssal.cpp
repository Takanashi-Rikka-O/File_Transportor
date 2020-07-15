#include<iostream>
#include"Share_Headers.h"
#include"TCP_SOCK_class.h"
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;
using namespace SOCKET;

int main(void)
{
	pid_t FORK;
	int num(10);
	
	if ((FORK=fork()) == 0)
	{
		// Child.
		sleep(1);

		TCP_SOCK_class Child;
		char Buff[64];
		memset(Buff,'\0',64);

		struct sockaddr_in Peer={
				.sin_family=AF_INET,
				.sin_port=Child._HTONS_(4396),
			};

		Child._INET_PTON_(AF_INET,"127.0.0.2",&Peer.sin_addr);
		struct pollfd Temp={
				.fd=Child._GET_SOCKET_(MAIN_SOCKET),
				.events=POLLRDNORM,
			};
		
		Child._SET_ADDR_(MAIN_SOCKET,(struct sockaddr *)&Peer);
		Child._SET_POLLFD_(MAIN_SOCKET,Temp);

		errno=0;

		ssize_t Readed(0);
		if (Child._CONNECT_(MAIN_SOCKET) == 0)
		{
			while (num > 0)
			{
				Child._POLL_(MAIN_SOCKET);
				if (Child._Check_Main_(POLLRDNORM))
				{
					Readed=read(Child._GET_SOCKET_(MAIN_SOCKET),Buff,64);
//					Readed=Child._READ_SOCK_(Child._GET_SOCKET_(MAIN_SOCKET),Buff,64);
					cout<<"Buff : "<<Buff<<" Readed : "<<Readed<<endl;
				}
				else;

				--num;
			}

			Child._SHUTDOWN_(Child._GET_SOCKET_(MAIN_SOCKET),SHUT_RDWR);
			
			return 0;

		}
		else
		{
			cerr<<"Child failed to connect. - "<<errno<<endl;
			strerror(errno);
			return 1;
		}


	}
	else
	{
		// Parent.

		TCP_SOCK_class Parent(4396,4397,10,10);
		if (Parent._BIND_(MAIN_SOCKET) < 0)
		{
			cerr<<"Parent failed to bind."<<endl;
			return 1;
		}
		else
			if (Parent._LISTEN_(MAIN_SOCKET,1) < 0)
			{
				cerr<<"Parent failed to listen."<<endl;
				Parent._SHUTDOWN_(Parent._GET_SOCKET_(MAIN_SOCKET),SHUT_RDWR);
				return 1;
			}
			else;

		int Switch(1);
		socklen_t Len=sizeof(int);
		Parent._SETSOCKOPT_(Parent._GET_SOCKET_(MAIN_SOCKET),SOL_SOCKET,SO_REUSEPORT,&Switch,Len);
		Parent._SETSOCKOPT_(Parent._GET_SOCKET_(MAIN_SOCKET),SOL_SOCKET,SO_REUSEADDR,&Switch,Len);

		ssize_t Wrotten(0);
		int New_Chat=Parent._ACCEPT_(MAIN_SOCKET,NULL,NULL);
		if (New_Chat > 0)
			while (num > 0)
			{
				Wrotten=write(New_Chat,"XXXXXXXXX",32);
//				Wrotten=Parent._WRITE_SOCK_(New_Chat,"Message from parent : A",32);
				cout<<"Parent - writted "<<Wrotten<<endl;
				--num;
			}
		else
		{
			cerr<<"Parent failed to accept."<<endl;
			return 1;
		}


//		raise(SIGSTOP);

		Parent._SHUTDOWN_(New_Chat,SHUT_RDWR);
		Parent._SHUTDOWN_(Parent._GET_SOCKET_(MAIN_SOCKET),SHUT_RDWR);

		return 0;
	}


}
