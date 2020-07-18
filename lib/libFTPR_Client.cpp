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
			if (! Reader.is_open())	// Check open status.
			{
				cerr<<"FTPR_Client: Failed to open configure file."<<endl;
				return -1;
			}
			else;
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
				/*	The DownloadPath must be alloced by _Client_Init_,this function not checking it whether is not NULL.	*/
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
		if (Reader.eof() || 0 == Client_Options)
		{
			cerr<<"FTPR_Client: Succeed to read configure file."<<endl;
			(void)Reader.close();
			return 0;
		}
		else
		{
			cerr<<"FTPR_Client: Detected an error in reading configure file."<<endl;
			(void)Reader.close();
			return -1;
		}

		//	Return
	}


	/*	Public zone.	*/


	void FTPR_client_class::_Client_WorkUp_(void)
	{
		/*	Client work function.	*/
		/*	This function respones initiactive with user and invoke command interface.	*/

		char *CommBuff(NULL);	// Buffer for message and command.
		int Comm_Socket=TSC->_GET_SOCKET_(MAIN_SOCKET);	// Communication socket.
		struct pollfd Comm_Target={
					.fd=Comm_Socket,
					.events=POLLRDNORM,
				};
		
		// These structure for 'get file' and 'up file' commant to be ready.
		NETIOINFO Network_Parameter={
			.Network_File_IO_Buffer=Share_Set.File_Buff_Size,
			.What_Behavior=0,	// This member will be resetting.
			.Retry_Number=Share_Set.Retry_Net_IO,
			.Retry_Link=Retry_ToLink,
			};	// Network io parameter.

		FGU DownUp_Env={
			.Tcp_sock=TSC,
			.Fid=FIDC,
			.Thread=PTC,
			.Timer_Timeout=0,	// This member have not to be used by client.
			.Net_io=&Network_Parameter,
			.Work_id=CLIENT_ID,
			};

		
		//	Env

		/*	Get memory for buffer.	*/
		CommBuff=new char[Share_Set.Message_Buff_Size];
		if (NULL == CommBuff)	// Check
		{
			cerr<<"FTPR_Client: Can not apply to memory for messages buffer."<<endl;
			return;
		}
		else;

		/*	Entry initiactive cycle.	*/
		/*	The outer layout is 'link' command.	*/
		/*	The inner layout is others command.	*/

		_HELP_();	// View help.
		
		cout<<endl<<"FTPR_Client: processing---"<<endl;

		while (true && ! Client_Should_Be_Stop)
		{

			cout<<"FTPR_Client: "<<endl
				<<"\tPlease use 'link' command to make new connection with server in the front! (extra - 'exit' 'help')"
				<<endl<<"> ";
			cin.getline(CommBuff,Share_Set.Message_Buff_Size);
			
			// Prevent cin class set fail bit,so must check the bit switch.
			if (cin.fail())
				cin.clear();
			else;

			// Check input.

			if (0 == strncmp(CommBuff,"link",4))
				if (_LINK_())
				{
					cout<<"FTPR_Client: Succeed to make connection."<<endl;
					Comm_Socket=TSC->_GET_SOCKET_(MAIN_SOCKET);
					Comm_Target.fd=Comm_Socket;
					Comm_Target.events=POLLRDNORM;
					TSC->_SET_POLLFD_(MAIN_SOCKET,Comm_Target);

					#ifdef DEBUG
						cerr<<"FTPR_Client: Socket - "<<Comm_Socket<<endl;
					#endif

				}
				else
				{
					cerr<<"FTPR_Client: Link failed."<<endl;
					continue;
				}
			else if (0 == strncmp(CommBuff,"help",4))
			{
				_HELP_();
				continue;
			}
			else if (0 == strncmp(CommBuff,"exit",4))
				break;
			else
			{
				cerr<<"FTPR_Client: Must make link at first."<<endl;
				continue;
			}

			/*	Inner cycle,must be make a new connection at the front.	*/
			do
			{
				cout<<"> ";	// Prompt character.
				cin.getline(CommBuff,Share_Set.Message_Buff_Size);
				// Check fail bit.
				if (cin.fail())
					cin.clear();
				else;

				// Check command.

				_Command_Parsing_(CommBuff);	// Commands parasing.	

				switch (The_CMD_Hit.CMDN)
				{
					case LS:				
						_LS_(Comm_Socket,CommBuff,Share_Set.Message_Buff_Size);
						break;

					case CD:
						_CD_(Comm_Socket,CommBuff,Share_Set.Message_Buff_Size);
						break;

					case GET:
						Network_Parameter.What_Behavior=GET_FILE;
						_GET_FILE_(Comm_Socket,DownUp_Env);
						break;

					case UP:
						Network_Parameter.What_Behavior=UP_FILE;
						_UP_FILE_(Comm_Socket,DownUp_Env);
						break;

					case LOGOUT:
						goto _LOGOUT_;

					case EXIT:
						Client_Should_Be_Stop=true;
						goto _LOGOUT_;

					case HELP:
						_HELP_();
						break;

					default:
						cerr<<"FTPR_Client: Command had not definecd."<<endl;
				}


				memset(CommBuff,'\0',Share_Set.Message_Buff_Size);	// Clear memory.


			} while (true && ! Client_Should_Be_Stop);

			_LOGOUT_:

				if (TSC->_RESET_SOCKETS_(MAIN_SOCKET|THREAD_SOCKET))	// Close all socket.
					cerr<<"FTPR_Client: Succeed to close link."<<endl;
				else
					cerr<<"FTPR_Client: Failed to reset socket."<<endl;
		}


		//	Primary

		delete[] CommBuff;
		CommBuff=NULL;

		//	Return
	}







}


