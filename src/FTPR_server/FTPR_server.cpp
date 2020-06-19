//	FTPR_server.cpp
//	Version : 0.1
//	Date : Fri Jun 12 10:31:41 2020
//	Last revise : Fri Jun 12 10:31:41 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Class definition of class FTPR_Server_class.The class design detail contain in the Proposal.
//
//	Functions :
//		Some function definition of the class.
//
//	Header :
//		"FTPR_server.h"


#include"FTPR_server.h"

namespace FTPR_SERVER{


	// C++ standard io.
	using namespace std;

	// Virtual Functions in protected.
	
	void FTPR_Server_class::_Read_Setting_(const char *Setting_File)
	{
		// This is a virtual function.
		// Overwrite in there.
		
		// Define a file object.
		ifstream Read;

		if (NULL != Setting_File)
		{
			if (FTPR_Basic::_Read_Shared_Setting_(Setting_File))
				;
			else
			{
				syslog(LOG(LOG_ERR),"Reading config in shared was fault.");
				return;
			}
			// Try to open file.
			try{
				
				Read.open(Setting_File,ios::in);
				if (Read.is_open())
					syslog(LOG(LOG_INFO),"Server opened setting file %s.",Setting_File);
				else
					throw -1;

			}
			catch (const short int Err)
			{
				// Does not opened.
				// To check this file if existed.
				struct stat Temporary_Stat=FTPR_Basic::SYSS->_LSTAT_(Setting_File);

				if (0 == Temporary_Stat.st_size)
				{
					syslog(LOG(LOG_ERR),"This config file %s had not existed - %d.",Setting_File,Err);
					State_Of_Initialization=false;
					return;		
				}
				else
				{
					// Retry open file.
					Read.open(Setting_File,ios::in);
					if (Read.is_open())
						;
					else
					{
						syslog(LOG(LOG_ERR),"Retry failed,server can not read config file.");
						return;
					}
				}
				


			}
		
		}
		else
		{
			syslog(LOG(LOG_ERR),"NULL Path of config file");
			State_Of_Initialization_FTPR_Server=false;
			return;
		
		}

		// Server config use '#' at comment symbol.		
		// Before reading,must get memory in heap.

		char *Temp_Read_Buffer=new char[32];
		// Does not need more memory than 32B.
		if (Temp_Read_Buffer)
			while (Read.getlien(Temp_Read_Buffer,32) && Read.fail())
			{
				if (*Temp_Read_Buffer == '#')
					continue;
				else
				{
					// Not comment.
					// Match string.

					// The function _Cut_Setting_Parameter_ will return the argument number.					
	
				}
			}
		else
		{
			State_Of_Initialization_FTPR_Server=false;
			syslog(LOG(LOG_ERR),"Can not get memory for buffer.");		
		}


	}







}
