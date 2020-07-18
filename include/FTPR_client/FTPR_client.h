//	FTPR_client.h
//	Version : 0.1
//	Date : Sat Jul  4 14:41:38 2020
//	Last revise : Sat Jul  18 21:17:? 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Client class header.Define a class in this file,and main file will contain this file.
//	Client also receive signals like same with server.Because client communicate with server use Stop-Wait mode,
//	so does not need to open a new thread or process to respones output.
//	Functions :
//		Class methods.Client will rewrite <command parsing> <setting read>
//		Client does not need logger because it will use interactive mode with user.
//		Client must to init itself.
//		Client must define method to shutdown itself.
//		Client must define a work function to process interaction with user.
//		Client must define command respones mode to finished work.
//
//	Header :
//		"Shared_Headers.h"	// Contain some common headers need by feature classes.
//		"FID_class.h"
//		"THREAD_class.h"
//		"TCP_SOCK_class.h"
//		"SYSTEM_SIGNAL_class.h"
//
//	Fix : 

#ifndef _FTPR_CLIENT_H_
#define _FTPR_CLIENT_H_


/*	Headers		*/

#include"FTPR.h"
#include<iostream>


/*	Headers		*/


/*	Define zone.	*/

// Configure file path.
#define CONFIG_FILE "/etc/ftpr/client/FTPR_client.conf"

// Default value.
#define Default_Retry_ToLink 3	// Default is three.
#define Default_DownloadPath "/tmp" //Default is tmp.
// Commands
#define LINK 25
#define EXIT 26
#define HELP 27

// Function error codes
#define C_TAKE_SUSS 0
#define C_TAKE_ERR 1	// Client failed to take IP:PORT. It is possible received a error format string.
#define C_TAKE_IP_ERR 2	// It is possible number greater than 255.
#define C_TAKE_PORT_TOO_LARGE 3 // The port number greater than 65535.
#define C_TAKE_INVALID_STR 4	// From inet_pton.
#define C_TAKE_INVALID_FAMILY 5 // From inet_pton.

extern bool Client_Should_Be_Stop;	// For prevent definitions more than once.

/*	Define zone.	*/

namespace FTPR_CLIENT{

	using namespace FTPR;		// Basic class.





	/*	-----------------	*/

	// Define a structure for init feature classes.
	
	using Client_Init=struct Init_Client_FC{
				const struct Shared_Setting & FSS;	// Shared settings set.
				unsigned short int RTL;		// Retry to make link.
				const char * & DP;			// Download path.
			};


	
	// Singals.

	void Sigaction_Client_Sigterm(int Signo,siginfo_t *SigInfo,void *Context);
	void Sigaction_Client_Sigint(int Signo,siginfo_t *SigInfo,void *Context);
	void Sigaction_Client_Sigalrm(int Signo,siginfo_t *SigInfo,void *Context);
	void Sigaction_Client_Sighup(int Signo,siginfo_t *SigInfo,void *Context);


	class FTPR_client_class: protected FTPR_Basic
	{

		private:
			/*	Client settings.	*/
			unsigned short int Retry_ToLink;	// For connect retry.
			char *DownloadPath;	// This pointer will allocs a part of memory for save download path.
			/*	Client settings.	*/

			
			/*	Client particular functions.	*/
			short int _Take_IP_Port_(char *TargetStr,struct sockaddr *Addr);

			/*	Client particular functions.	*/

			/*	Command respones functions.	*/
			void _LS_(const int & Comm_Sock,char * & Buffer,const size_t & Buffer_Len);
			void _CD_(const int & Comm_Sock,char * & Buffer,const size_t & Buffer_Len);
			void _GET_FILE_(const int & Comm_Sock,FGU & Env_GET);
			void _UP_FILE_(const int & Comm_Sock,FGU & Env_UP);
			bool _LINK_();
	
			// logout have not to define a function to do work.
			// exit also have not to define a function to do work.
	
			void _HELP_(void);
			/*	Command respones functions.	*/

		protected:

			// These virtual function will be rewriting.
			virtual short int _Read_Setting_(const char *Setting_File);
			virtual short int _Init_FC_(const void *SETTINGS);
			virtual void _Command_Parsing_(const char *Command);

		public:

			/*	Build and clear.	*/
			FTPR_client_class();
			~FTPR_client_class();
			/*	Build and clear.	*/


			/*	Work functions.		*/
			void _Client_(void);	// Start function.

			bool _Client_Init_(void);	// Init function.
			void _Client_WorkUp_(void);	// Work function.

			void _Client_ShutDown_(void);	// Shutdown function.

			/*	Work functions.		*/

	};


	/*	-----------------	*/



}




// Header end.
#endif

