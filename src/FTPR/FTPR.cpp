//	FTPR.cpp
//	Version : 0.1
//	Date : Sat May 23 15:13:05 2020
//	Last revise : Sat May 23 15:13:05 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Build definition of class FTPR_Basic.
//
//	Functions :
//		Class method,prototype in the header.
//		
//	Header :
//		"FTPR_class.h"

#include"FTPR.h"

namespace FTPR{

	// Build method.
	FTPR_Basic::FTPR_Basic()
	{
		State_Of_Initialization_FTPR=true;	// True.
		
		// Set memory to zero.
		memset(&Share_Set,'0',sizeof(struct Shared_Setting));
		memset(&File_Info,'0',sizeof(GET_UP));

		The_CMD_Hit.CMDN=0;			// The macro definition of cmds had been defined.Zero is a error value.
		The_CMD_Hit.CMDP=NULL;			// This pointer will point to a command string.

		PTC=NULL,FIDC=NULL,TSC=NULL,SYSS=NULL;		// Null.Them will be create by inherted class.

		syslog(LOG(LOG_INFO),"FTPR: The Basic class created.");

	}

	// Clear method.
	FTPR_Basic::~FTPR_Basic()
	{
			syslog(LOG(LOG_NOTICE),"FTPR: Basic class had destroied.");
	}
	

	// Virtual method read setting.
	short int FTPR_Basic::_Read_Setting_(const char *Setting_File)
	{
		syslog(LOG(LOG_ERR),"FTPR: Do not call this function,this function must be overwire in other class.");
		return 0;
	}


	void FTPR_Basic::_FTPR_Logger_(const short int & Value)
	{
		// Log message array.
		char LogMsg[64+1];
		memset(LogMsg,'\0',64+1);	// Clear memory.

		// Selecet log.
		switch (Value)	
		{
			/*	Read shared setting.	*/

			case FTPR_RS_F:
				_Copy_String_(LogMsg,"FTPR: Failed to complete shared settings.",41);
				break;

			case FTPR_RS_MEM_F:
				_Copy_String_(LogMsg,"FTPR: Failed to apply for buffer mem in RS.",43);
				break;

			case FTPR_RS_OPENF_F:	
				_Copy_String_(LogMsg,"FTPR: Failed to open config file in RS.",39);
				break;


			/*	FF_Thread.	*/
			case FF_SET_CANCEL_STATE_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to set thread cancel state.",40);
				break;

			case FF_SET_CANCEL_TYPE_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to set thread cancel type.",39);
				break;

			case FF_BIND_SOCKET_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to bind socket in ff.",34);
				break;

			case FF_OPEN_LISTEN_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to set listen in ff.",33);
				break;

			case FF_OPEN_TIMER_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to open timer in ff.",33);
				break;

			case FF_GET_IO_BUFF_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to apply for buffer mem in ff.",43);
				break;

			case FF_CANCEL_TIMER_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to cancel timer in ff.",35);
				break;

			case FF_GET_SUSS:
				_Copy_String_(LogMsg,"FTPR: Successfully to sent file in ff.",38);
				break;

			case FF_GET_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to send file in ff.",32);
				break;

			case FF_UP_SUSS:
				_Copy_String_(LogMsg,"FTPR: Successfully to received file in ff.",42);
				break;

			case FF_UP_ERR:
				_Copy_String_(LogMsg,"FTPR: Failed to receive file in ff.",35);
				break;

			case FF_BEHAVIOR_ERR:
				_Copy_String_(LogMsg,"FTPR: Behavior had not defined in ff.",37);
				break;

			case FF_ACCEPT_BAD:
				_Copy_String_(LogMsg,"FTPR: Failed to accept link in ff.",34);
				break;

			deafault:
				_Copy_String_(LogMsg,"FTPR: Log value had not defined.",32);

		}

		syslog(LOG(LOG_ERR),LogMsg);	// Record.
	}

}
