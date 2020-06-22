//	FTPR_server.h
//	Version : 0.1
//	Date : Thu Jun 11 20:05:35 2020
//	Last revise : Thu Jun 11 20:05:35 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Server process header.It does not need 0,1,2.And only services one a user at same time.The design detail in the Proposal file.And this server
//	have not ensure portability.So it is possible only using on the linux platform in normaly.
//
//	Functions :
//		Details in the Proposal document.
//
//	Header :
//		"FTPR.h"
//		"FID_class.h"
//		"THREAD_class.h"
//		"TCP_SOCK_class.h"
//		"SYSTEM_SIGNAL_class.h"

#ifndef _FTPR_SERVER_H_
#define _FTPR_SERVER_H_

#include"FTPR.h"
#include"FID_class.h"
#include"THREAD_class.h"
#include"TCP_SOCK_class.h"
#include"SYSTEM_SIGNAL_class.h"

/*	Define error code zone.	*/

// _Read_Setting_
#define R_GET_MEM_ERR 1
#define R_SHARE_SET_ERR 2
#define R_OPEN_CONFIG_ERR 3
#define R_SET_OVER 4
#define R_SET_ERR 5
// _Read_Setting_ end

// _Init_FC_
#define Init_FC_SUSS 0
#define Init_FID_ERR 1
#define Init_CRE_FID_ERR 2
#define Init_THREAD_ERR 3
#define Init_CRE_THREAD_ERR 4
#define Init_SOCK_ERR 5
#define Init_CRE_SOCK_ERR 6
#define Init_SIGNAL_ERR 7
#define Init_CRE_SIGNAL_ERR 8
// _Init_FC_ end


/*	End of error code.	*/

#define CONFIG_FILE "FTPR_server.conf"	// Configure file.

/*	The default server setting.	*/

#define Default_CWAITS 30
#define Default_CPORT 4396
#define Default_DUPORT 33242
#define Default_RPATH "/run/ftprs"

/*	End of default setting.	*/



namespace FTPR_SERVER{

	using namespace FTPR;

	bool Server_Should_Not_To_Stop(true); // Server will check this variable before next work starting.


/*	Signal action function defining zone.	*/

// Server captures signal SIGTERM,SIGINT,SIGHUP,SIGALRM.
// The action for SIGINT will send signal SIGTERM to itself.
// The action for SIGHUP will record a log to message but do not stop.
// The action for SIGALRM will ignores.

	void  _Server_Signal_Sigterm_(int Sig,siginfo_t *Receive_Info,void *Content);
	void  _Server_Signal_Sigint_(int Sig,siginfo_t *Receive_Info,void *Content);
	void  _Server_Signal_Sighup_(int Sig,siginfo_t *Receive_Info,void *Content);
	void  _Server_Signal_Sigalrm_(int Sig,siginfo_t *Receive_Info,void *Content);

// The content will not use in them.
// They will record a log when them receive a signal.
// Record : "Server: Signal [number] [name] received from [pid] - [action]"

/*	End of signal action.	*/

//	In server'named space,must define some structures only owns to server.
//	Server will use them to save setting or make other features.


// Define server setting structure,the structure will contains a Shared_Setting pointer in FTPR namepsace.

using Server_Set=struct Server_Set_Structure{

			struct Shared_Setting *SHS;	// This pointer point to a object of FTPR_Basic.
			
			unsigned short int DUP;	// Down/Up port.
			unsigned short int CPT;	// Comm port.
			const char *Root_Path;	// RPATH.

			};



/*	Server inherits from FTPR_Basic with protected,it could accessing all members of the basic class.	*/

	// Server class is-a FPTR_Basic,and FTPR_Basic has-a TCP/FID/SIG/THREAD.
	class FTPR_Server_class : protected FTPR_Basic
	{

		private :
			/*	Private zone.	*/

			/*	There define some setting variables them monapolize to server.	*/
			char RPATH[128];		// This array will apply memory with 128B to save root path.
			unsigned short int DUPORT;	// Option DUPORT,for file transport.
			unsigned short int CWAITS;	// Option CWAITS,for connect timing in thread.
			unsigned short int CPORT;	// Option CPORT,for communication with client.

			// Define optionals in class is not a great method,because it make matter in the case of update.
			// But at the time,use this method.In the later will resive this method.

			/*	End of setting.	*/

			
		
			/*	Internal function zone.	*/
			
			// Define 'ls' function.

			void _LS_(const int Peer_Sock);


			// Define 'cd' function.

			void _CD_(const char *New_Dir);

			// Define 'GET File' function.

			

			// Define 'UP File' function.



			/*	End of internal function zone.	*/
			
		
		protected :
			/*	Protected zone.	*/

			// Read setting.
			virtual short int _Read_Setting_(const char *Setting_File);

			// Init feature classes.
			virtual short int _Init_FC_(const void *SETTINGS);

			/* Command parsing.	*/
			virtual void _Command_Parsing_(const char *Command);	
		
		public :
			/*	Public zone.	*/

			FTPR_Server_class():DUPORT(65535),CWAITS(3),CPORT(65534)
			{
				memset(RPATH,'\0',128);
			}
			~FTPR_Server_class()
			{
				_Server_Shutdown_();
			}

			bool _Server_Init_(void);
		/* Description for _Server_Init_:
		 *	This function respones :
		 *		read setting,initialize variables
		 *		set root tree,change work home,
		 *		set signal behavior,init feature classes.
		 * 
		*/ 
			
			void _Server_WorkUp_(void);
		/* Description for _Server_Work_Up_:
		 * 	This function respones :
		 *		create socket,set socket option,bind socket,
		 *		open listen,call accpet(In a cycle),
		 *		call command responer
		*/
			void _Server_Shutdown_(void);	
		/* Description for _Server_Shutdown_:
		 * 	This function respones :
		 *		stop server(after get a signal),destroy class,
		 *		record log,exit.
		 *
		*/ 

	};






}



#endif
