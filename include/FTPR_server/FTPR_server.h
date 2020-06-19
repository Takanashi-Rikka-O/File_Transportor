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
#include"SYSTEN_SIGNAL_class.h"

namespace FTPR_SERVER{



//	In server'named space,must define some structures only owns to server.
//	Server will use them to save setting or make other features.




	// Server class is-a FPTR_Basic,and FTPR_Basic has-a TCP/FID/SIG/THREAD.
	class FTPR_Server_class : protected FTPR_Basic
	{

		private :
			/*	Private zone.	*/

			/*	There define some setting variables them monapolize to server.	*/
			char *Server_Root_Path;		// This pointer will apply memory with 128B to save root path.
		

			/*	There define some inter function,them would be using by class methods.	*/

			int _Cut_Setting_Parameter_(const char *Set_Option);
		
		protected :
			/*	Protected zone.	*/

			// Read setting.
			virtual void _Read_Setting_(const char *Setting_File);

			// Init feature classes.
			virtual short int _Init_FC_(const void *SETTINGS);

			/* Command parsing.	*/
			virtual WCH * _Command_Parsing_(const char *Command);	
		
		public :
			/*	Public zone.	*/

			bool State_Of_Initialization_FTPR_Server;	// Status record.	
			FTPR_Server_class();
			~FTRP_Server_class();

			short int _Server_Init_(void);
			
			void _Server_WorkUp_(void);

			void _Server_Shutdown_(void);	

	};






}



#endif
