//	FTPR_client_main.cpp
//	Version : 0.1
//	Date : Sat Jul  4 14:42:10 2020
//	Last revise : Sat Jul  4 14:42:10 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Start main function.This function only create class object and process _Client_
//
//	Functions :
//
//	Header :
//		"FTPR_client.h"
//
//	Fix : 

#include"FTPR_client.h"

bool Client_Should_Be_Stop(false);

using namespace FTPR_CLIENT;
using namespace std;

int main(void)
{
	FTPR_client_class *Client=new FTPR_client_class;	// Make object.
	if (NULL == Client)
		cerr<<"ftprc: Failed to open client."<<endl;
	else
	{
		cout<<"ftprc: Start Client up!"<<endl;
		Client->_Client_();

		cout<<"ftprc: Client shut down!"<<endl;
		delete Client;
	}

	_exit(0);
}

