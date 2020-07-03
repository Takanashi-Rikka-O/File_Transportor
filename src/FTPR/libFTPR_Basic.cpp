//	TheBasic_Lib.cpp
//	Version : 0.1.1
//	Date : Sat Jun 13 10:25:49 2020
//	Last revise : Sat Jun 13 10:25:49 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This file have some function definitions for make a shared library.Why make shared lib for this program,is so that upgrad.
//	Functions :
//		Some Class function defintions.
//	Header :
//		"FTPR.h"
//
//	Fix :
//		> Adding a optional 'FBUFF',this option will be using in download/upload thread to apply memory.


#include"FTPR.h"

namespace FTPR{


/*	We can use tcp socket to send a structure segment to peer,peer could use same struct to save them.	*/
/*	So do not need to make cmd string for file information.	*/
/*	Client will overwirte '_RECEIVE_FILE_INFO_' virtual method for get file info.	*/


	using std::ifstream;
	using std::ios;

	int FTPR_Basic::_Get_Number_Of_Optional_(const char *Option)
	{
		// At first search '='.If the number is '0xxxxx' will suppose it is a number with eight base.

		const char *The_Equal=strstr(Option,"=");		// Search '='.	Name=Value.
		if (NULL == The_Equal)	// None '='.
			return -1;
		else;

		// Because these shared setting all are positive interger,so must scan value whether is a error format.

		const char *The_End=&Option[strlen(Option)]-1;		// End.

		// Scan 'The_Equal -- The_End'

		int Result(0);	// This variable will be return.
		
		bool Suppose(true);	// Suppose it's right format.
		for (const char *Temp_Posi(The_Equal+1); Temp_Posi != The_End+1 && Suppose; ++Temp_Posi)
			Suppose=(*Temp_Posi >= '0' && *Temp_Posi <= '9')?true:false;

		if (Suppose)	// Check what it is.
			;
		else
			return -1;	// -1 will make process use default value.	

		
		if (*(The_Equal+1) == '0')	// 8.	mode_t.
			for (int Base(1); The_End != The_Equal; --The_End,Base*=8)
				Result+=(*The_End-'0')*Base;
		else
			for (int Base(1); The_End != The_Equal; --The_End,Base*=10)
				Result+=(*The_End-'0')*Base;

		#ifdef DEBUG
			syslog(LOG(LOG_NOTICE),"FTPR: Option : %s Get Number : %d .",Option,Result);
		#endif

		return Result;	// Return number.
	}	

	// Define a function to read shared setting.
	
	bool FTPR_Basic::_Read_Shared_Setting_(const char *Setting_File)
	{
		// Don't check this pointer because server and client will check it in the order at front.



	
		ifstream Read;	// Declare a file io object.

		Read.open(Setting_File,ios::in);	// ASCII open.

		int Temp_Num(0);	// To check return.
		
		if (Read.is_open())
		{
			// Succeed to open.
			
			char *In_Buffer=new char[64+1];	// Apply for memory.

			if (In_Buffer)
			{
				while (true)	// Cycle to read setting,if detected error,using default.
				{
					Read.getline(In_Buffer,65);	// Because reader does not use one character guess,so it is possible cause error.

					if (Read.eof())
						break;
					else if (Read.fail())
					{
						Read.clear();
						continue;
					}
					else;

					if (*In_Buffer == '#' || *In_Buffer == '\0')
						continue;
					else if (0 == strncmp(In_Buffer,"NIOTIMEOUT",10))	// Network IO timeout in thread.
						Share_Set.Network_IO_Timeout=((Temp_Num=_Get_Number_Of_Optional_(In_Buffer)) != -1)?Temp_Num:Default_NIOTIMEOUT;
					else if (0 == strncmp(In_Buffer,"MBUFF",5))	// Message buffer size.
						Share_Set.Message_Buff_Size=((Temp_Num=_Get_Number_Of_Optional_(In_Buffer)) != -1)?Temp_Num:Default_MBUFF;
					else if (0 == strncmp(In_Buffer,"FBUFF",5))	// File buffer size.
						Share_Set.File_Buff_Size=((Temp_Num=_Get_Number_Of_Optional_(In_Buffer)) != -1)?Temp_Num:Default_FBUFF;
					else if (0 == strncmp(In_Buffer,"FMASK",5))	// File umask.
						Share_Set.Make_File_Mask=((Temp_Num=_Get_Number_Of_Optional_(In_Buffer)) != -1)?Temp_Num:Default_FMASK;
					else if (0 == strncmp(In_Buffer,"RSIO",4))	// Retry network IO number.
						Share_Set.Retry_Net_IO=((Temp_Num=_Get_Number_Of_Optional_(In_Buffer)) != -1)?Temp_Num:Default_RSIO;
					else;

				}

				delete[] In_Buffer;	// Recycle memory.
	
				if (Read.eof())
				{
						
				#ifdef DEBUG
					syslog(LOG(LOG_NOTICE),"FTPR: EOF.");
				#endif
					Read.clear();
					Read.close();
					return true;
				}
				else
				{
					syslog(LOG(LOG_NOTICE),"FTPR: FAIL Config.");
					_FTPR_Logger_(FTPR_RS_F);
					return false;
				}


			}
			else
			{
				_FTPR_Logger_(FTPR_RS_MEM_F);
				return false;
			}
			

		}
		else
		{
			_FTPR_Logger_(FTPR_RS_OPENF_F);
			return false; // Failed to open file.
		}

	}

}
