//	libFTPR_Client.cpp
//	Version : 0.1
//	Date : Sat Jul 11 11:27:24 2020
//	Last revise : Sat Jul 11 11:27:24 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		All functions in this file which will be making as a shared library for client.It is possible down the procees speed for program,buf for 
//	upgarde,the benebit is more than malpractice.
//
//		As lib :
//			# For upgrade.
//			<client workup>
//			<read setting>
//			<init client>
//
//	Functions :
//		void _Client_WorkUp_(void);
//		virtual short int _Read_Setting_(const char *Setting_File);
//		bool _Client_Init_(void);
//		
//
//	Header :
//		"FTPR_client.h"
//
//	Fix : 


#include"FTPR_client.h"

namespace FTPR_CLIENT{


	/* C++ standard class.	*/
	using std::cin;
	using std::cout;
	using std::cerr;
	using std::endl;
	using std::ios_base;
	using std::ifstream;

	/*	Protected zone.	*/
	
	short int FTPR_client_class::_Read_Setting_(const char *Setting_File)	/*	0 on success,-1 on fault.	*/
	{

		ifstream Reader;	// C++ File reading object.
		char *Temp_Buff=new char[1024+1]; // This array will be using by optional reading.
		if (NULL == Temp_Buff)
		{
			cerr<<"FTPR_Client: Failed to alloc memory."<<endl;
			return -1;
		}
		else;
		int Invoke_Result(-1);	// This variable will save the number from setting optional.
		char *Temp_Pointer(NULL);	// This pointer will point to the position '=' on it.
		unsigned short int Client_Options(2);	// For front to break cycle.
		
		//	Env

		/*	At first must to read shared setting in the front.	*/
		if (! _Read_Shared_Setting_(Setting_File))
		{
			cerr<<"FTPR_Client: Failed to read shared setting."<<endl;
			return -1;	
		}
		else
		{
			Reader.open(Setting_File,ios_base::in);	// Try to open configure file with read only.
			if (! Reader.is_open())	// Succeed to open.
				cerr<<"FTPR_Client: Failed to open configure file."<<endl;
			else;
	
			return -1;
		}

		// Cycle to reading setting options.

		while (true && ! Client_Should_Be_Stop && Client_Options > 0)
		{
			Reader.getline(Temp_Buff,1025);	// Read 1024Bytes in once.
			if (Reader.fail())
				if (Reader.eof())
					break;
				else
					Reader.clear();
			else;

			if (0 == strncmp(Temp_Buff,"DPATH",5))
			{
				if ((Temp_Pointer=strstr(Temp_Buff,"=")) && *(Temp_Pointer+1) != '\0')
					_Copy_String_(DownloadPath,Temp_Pointer+1,strlen(Temp_Pointer+1));
				else
					_Copy_String_(DownloadPath,Default_DownloadPath,strlen(Default_DownloadPath));
				--Client_Options;	
			}
			else if (0 == strncmp(Temp_Buff,"RLINK",5))
			{
				Retry_ToLink=((Invoke_Result=_Get_Number_Of_Optional_(Temp_Buff)) > -1)?Invoke_Result:Default_Retry_ToLink;
				--Client_Options;
			}
			else;
		}


		//	Primary

		// Recycle memory.
		delete[] Temp_Buff;
		Temp_Buff=NULL;

		/*	Check state.	*/
		// Whether is EOF to broken.
		if (Reader.eof())
		{
			cerr<<"FTPR_Client: Succeed to read configure file."<<endl;
			return 0;
		}
		else
		{
			cerr<<"FTPR_Client: Detected an error in reading configure file."<<endl;
			return -1;
		}

		//	Return
	}


	/*	Public zone.	*/

	bool FTPR_client_class::_Client_Init_(void)
	{
		return false;
	}


	void FTPR_client_class::_Client_WorkUp_(void)
	{


	}







}


