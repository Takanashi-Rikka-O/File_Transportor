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

		PTC=NULL,FIDC=NULL,TSC=NULL,SYSS=NULL;		// Null.

		syslog(LOG(LOG_INFO),"The Basic class created.");

	}

	// Clear method.
	FTPR_Basic::~FTPR_Basic()
	{
		if (State_Of_Initialization_FTPR)
		{
			if (NULL != PTC)	// Posix thread class pointer.
				delete PTC;
			else;

			if (NULL != FIDC)
				delete FIDC;	// File and Directory io class pointer.
			else;
	
			if (NULL != TSC)	// TCP socket class pointer.
				delete TSC;
			else;
			
			if (NULL != SYSS)	// System signal class pointer.
				delete SYSS;
			else;
		
			syslog(LOG(LOG_NOTICE),"Basic class had deleted.");

		}
		else
			syslog(LOG(LOG_ERR),"Basic class does not be succeed to created.");

	}
	

	// Virtual method read setting.
	short int FTPR_Basic::_Read_Setting_(const char *Setting_File)
	{
		syslog(LOG(LOG_ERR),"Do not call this function,this function must be overwire in other class.");
	}



}
