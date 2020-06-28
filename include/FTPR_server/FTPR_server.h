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

//	End to define,T->1,F->0.

// _Read_Setting_
#define S_RS_GET_MEM_ERR 0
#define S_RS_SHARE_SET_ERR 1
#define S_RS_OPEN_CONFIG_ERR 2
#define S_RS_SET_OVER 3
#define S_RS_SET_ERR 4
// _Read_Setting_ end

// _Init_FC_
#define S_INIT_FC_SUSS 5
#define S_INIT_FID_ERR 6
#define S_INIT_CRE_FID_ERR 7
#define S_INIT_THREAD_ERR 8
#define S_INIT_CRE_THREAD_ERR 9
#define S_INIT_SOCK_ERR 10
#define S_INIT_CRE_SOCK_ERR 11
#define S_INIT_SIGNAL_ERR 12
#define S_INIT_CRE_SIGNAL_ERR 13
// _Init_FC_ end

// Signal
#define S_SIGALRM_ERR 14
#define S_SIGHUP_ERR 15
#define S_SIGINT_ERR 16
#define S_SIGTERM_ERR 17


// _WorkUp_

#define S_BIND_ERR 18
#define S_LISTEN_ERR 19

// GET FILE
#define S_FILE_NOTEX 20
#define S_OPEN_FILE_F 21
#define S_CRE_PT_F 22
#define S_CMD_GET 23

// UP FILE
#define S_FILE_HADEX 24
#define S_GETF_INFO_F 25
#define S_SETF_LEN_F 26
#define S_REVF_INFO_F 27
#define S_GETF_LOCK_F 28
#define S_LNK_FILE_F 29
#define S_CMD_UP 30

// CD
#define S_CD_T 31
#define S_CD_F 32
#define S_CMD_CD 33

// LS
#define S_LS_T 34
#define S_CMD_LS 35

#define S_FAIL_MEM 36

// _WorkUp_ end


/*	End of error code.	*/

#define CONFIG_FILE "FTPR_server.conf"	// Configure file.

/*	The default server setting.	*/

#define Default_CWAITS 30
#define Default_CPORT 4396
#define Default_DUPORT 33242
#define Default_RPATH "/run/ftprs"	// ftprsd must check this file if had existed.In the case of not existed,server will try to mkdir.

/*	End of default setting.	*/

// The server logger only respones server logs,FTPR logger respones FTPR logs.

extern bool Server_Should_Not_To_Stop;

namespace FTPR_SERVER{

	using namespace FTPR;

//	bool Server_Should_Not_To_Stop(true); // Server will check this variable before next work starting.


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
			char Root_Path[128];		// This array will apply memory with 128B to save root path.
			unsigned short int DownUp_Port;	// Option DUPORT,for file transport.
			unsigned short int Comm_Wait_Seconds;	// Option CWAITS,for connect timing in thread.
			unsigned short int Comm_Port;	// Option CPORT,for communication with client.

			// Define optionals in class is not a great method,because it make matter in the case of update.
			// But at the time,use this method.In the later will resive this method.

			/*	End of setting.	*/

			
		
			/*	Internal function zone.	*/
			
			// Define 'ls' function.

			void _LS_(const int Peer_Sock,char *LS_Buffer);


			// Define 'cd' function.

			void _CD_(const int Peer_Sock,const char *New_Dir);

			// Define 'GET File' function.

			bool _GET_FILE_(FGU *T_Resource,int & Client_Socket);		// It only need parameters,a FGU object and client socket.

			// Define 'UP File' function.			
			
			bool _UP_FILE_(FGU *T_Resource,int & Client_Socket);		// Like _GET_FILE_.

			/*	They are can accessing all resources of this class.	*/

			// Define logger.

			void _Server_Logger_(const short int Which);	// Record server log.

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

			FTPR_Server_class():DownUp_Port(65535),Comm_Wait_Seconds(3),Comm_Port(65534)
			{
				memset(Root_Path,'\0',128);
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

			void _Server_()
			{
				// Start server.

				if (_Server_Init_())
				{
					_Server_WorkUp_();
				}
				else;
			}

	};






}



#endif
