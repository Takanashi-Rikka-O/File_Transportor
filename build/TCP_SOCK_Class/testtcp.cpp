//	testtcp.cpp
//	Version : 0.1
//	Date : Sat Jun 13 12:40:48 2020
//	Last revise : Sat Jun 13 12:40:48 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This file for test TCP_SOCK_class.
//	Functions :
//	Header :

#include"TCP_SOCK_class.h"
#include<iostream>
#include<errno.h>
#include<cstring>

using namespace std;
using namespace SOCKET;

int main(void)
{
	TCP_SOCK_class TempObject;	// Declare a temp object.
	
	if (TempObject.State_Of_Initialization_SOCK)
		;
	else
	{
		cerr<<"The object had noe created!!\n";
		return 1;
	}
	
	unsigned short int (*P)[2]=new unsigned short int[1][2];	// Memory get.
	**P=4396;	// Parent listen.
	*(*P+1)=32317;	// Not using.

	int len(16),len2(12);
	char MB[128];
	memset(MB,'\0',128);

	if (P)
	{
		pid_t PID;	// Plan to fork a child process.
		PID=fork();

		if (0 == PID)	// It is child process.
		{
			// Child make link.

			TempObject._Set_Ports_(MAIN_SOCKET,**P);	
			sleep(5);
			if (TempObject._CONNECT_(MAIN_SOCKET) == 0)
			{
				cout<<"Child writting\n";

				cout<<"Child written : "<<TempObject._WRITE_SOCK_(TempObject._Get_Socket_(THREAD_SOCKET),"From child.\0",12)<<endl;
			
				ssize_t R=TempObject._READ_SOCK_(TempObject._Get_Socket_(THREAD_SOCKET),MB,12);
	
				cout<<"Readed\n";

				if (R > 0)
					cout<<"Received "<<MB<<" - child\n";
				else
					cerr<<"Bad reading.\n";
			}
			else
			{
				cerr<<"Connect failed - child.\n";
				return 1;
			}
			

		}	
		else		// It is parent process.
		{
			// Parent listen.
			TempObject._Set_Ports_(THREAD_SOCKET,*(*P+1));
			if (TempObject._BIND_(MAIN_SOCKET) == 0)
			{
				TempObject._LISTEN_(MAIN_SOCKET,1);
				int New=TempObject._ACCEPT_(MAIN_SOCKET,NULL,NULL);
				
				ssize_t R=TempObject._READ_SOCK_(New,MB,12);
				if (R > 0)
					cout<<"Received "<<MB<<" - parent\n";
				else
					cerr<<"Bad reading\n";

				cout<<"Wrotten : "<<TempObject._WRITE_SOCK_(New,"From parent.\0",12)<<endl;

				TempObject._SHUTDOWN_(New,SHUT_RDWR);

				return 0;
				
			}
			else
			{
				perror("NULL");	
				cerr<<"Bind failed - parent.\n";
				return 1;
			}
		}

	}
	else
	{
		cerr<<"Can not get memory for ports.!\n";
		return 1;
	}
	
}
