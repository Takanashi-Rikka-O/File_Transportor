//	FTPR_client.cpp
//	Version : 0.1
//	Date : Sat Jul  4 14:42:06 2020
//	Last revise : Sat Jul  4 14:42:06 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This file contains all definitions of class member.But some functions will archive to a shared library for upgrade after.
//		Hard coding :
//			<command mode>	// Because all command had been determined.
//			<command parsing> // Also had been determined.
//			<Init features class>
//			<Shutdown client>
//			<Take ip:port>
//			<signals>
//			<init client>
//
//		As lib :
//			# For upgrade.
//			<client workup>
//			<read setting>
//
//
//	Functions :
//		void _LS_(const int & Comm_Sock,char * & Buffer);
//		void _CD_(const int & Comm_Sock,const char *New_Path);
//		void _GET_FILE_(const int & Comm_Sock,FGU & Env_GET);
//		void _UP_FILE_(const int & Comm_Sock,FGU & Env_UP);
//		void _HELP_(void);
//		short int _Take_IP_Port_(const char *TargetStr,struct sockaddr *Addr);
//
//		short int _Init_FC_(const void *SETTINGS);
//		void _Command_Parsing_(const char *Command);
//
//		void _Client_(void);
//		void _Client_ShutDown_(void);
//		
//		// Build and clear methods.
//
//	Header :
//		"FTPR_client_class.h"
//
//	Fix : 

#include"FTPR_client.h"


namespace FTPR_CLIENT{

	// C++ standard classes.
	using std::cout;
	using std::endl;
	using std::cin;
	using std::ios_base;
	using std::cerr;
	using std::hex;
	using std::dec;
	

	/*	Signal captures.	*/
	
	/*	If have syscall had interrupted by signal,it would not be restarted by system.	*/

	void Sigaction_Client_Sigterm(int Signo,siginfo_t *SigInfo,void *Context)
	{
		cerr<<"FTPR_Client: Received signal "<<Signo<<" - SIGTERM - sent by "<<SigInfo->si_pid<<" user : "<<SigInfo->si_uid
			<<" action - stop."<<endl;
	}
	void Sigaction_Client_Sigint(int Signo,siginfo_t *SigInfo,void *Context)
	{
		union sigval Temp={.sival_int=0};
		cerr<<"FTPR_Client: Received signal "<<Signo<<" - SIGINT - sent by "<<SigInfo->si_pid<<" user : "<<SigInfo->si_uid
			<<" action - send SIGTERM."<<endl;
		sigqueue(getpid(),SIGTERM,Temp);
	}
	void Sigaction_Client_Sigalrm(int Signo,siginfo_t *SigInfo,void *Context)
	{
		cerr<<"FTPR_Client: Received signal "<<Signo<<" - SIGALRM - sent by "<<SigInfo->si_pid<<" user : "<<SigInfo->si_uid
			<<" action - ignores."<<endl;
	}
	void Sigaction_Client_Sighup(int Signo,siginfo_t *SigInfo,void *Context)
	{
		cerr<<"FTPR_Client: Received signal "<<Signo<<" - SIGHUP - sent by "<<SigInfo->si_pid<<" user : "<<SigInfo->si_uid
			<<" action -ignores."<<endl;
	}



	/*	Class functions.	*/

	/*	Private.		*/

	short int FTPR_client_class::_Take_IP_Port_(char *TargetStr,struct sockaddr *Addr)
	{	
	
		// Function does not check whether is null pointer,this work would be doing by caller.

		// At first,scan string to check wheter is error format

		const size_t StrLength(strlen(TargetStr));	// Get length.
		short int State(-1);		// State for this function work.
		bool Right_Format(true);	// Suppose it is right.
		unsigned short int Splite_Char(0);
		short int Found_point(0);	// This variable after scan must is equal to 3.
		short int Found_splite(0);	// This variable after scan must is equal to 1.
		unsigned short int Port_Number(0);	// Port number.

		// TCP Socket interface pointer woul be checking at front invoke by caller.



		// IP:PORT - 255.255.255.255:65535	Max.

		// Cycle to check,and at this action will record ':'.

		for (unsigned short int index(0); index < StrLength && Right_Format; ++index)
		{
			if (TargetStr[index] <= 9 && TargetStr[index] >= 0)
				continue;
			else if ('.' == TargetStr[index])
				++Found_point;
			else if (':' == TargetStr[index])
				Splite_Char=index,++Found_splite;
			else
				Right_Format=false;	// If it is not the cases in over,it must be a error format.
		}

		// Check result.

		if (Right_Format && 3 == Found_point && 1 == Found_splite)
		{
			// Try to convert IP.
			TargetStr[Splite_Char]='\0';	// ':' -> '\0'.
			int Convert_Result(TSC->_INET_PTON_(Addr->sa_family,TargetStr,&Addr));
			// 0 success,1 valid string,-1 valid family.

			switch (Convert_Result)
			{
				case 1:
					State=C_TAKE_INVALID_STR;
					break;
				case -1:
					State=C_TAKE_INVALID_FAMILY;
					break;
				default:
					// Try to get port.
					//	Left	<-	Right.
					for (unsigned short int i(Splite_Char),j(StrLength-1),base(1); j != i; --j,base*=10)
						Port_Number+=(TargetStr[j]-'\0')*base;

					// Check result.
					if (Port_Number > 65535)
						State=C_TAKE_PORT_TOO_LARGE;
					else
					{
						((struct sockaddr_in &)Addr).sin_port=TSC->_HTONS_(Port_Number);
						State=C_TAKE_SUSS;
					}
			}
		
		}
		else
			State=C_TAKE_ERR;


		return State;

	}

	void FTPR_client_class::_LS_(const int & Comm_Sock,char * & Buffer,const size_t & Buffer_Len)
	{
		// Server will send '#EOF#' as end of messages.	
		// And _LS_ default set retry to read socket limit for number is 2,and time limit is 3s.
		
		const unsigned short int OldTime_Limit(TSC->_GET_WAIT_TIME_(MAIN_SOCKET));	// Backup.
		unsigned short int Retry(2);	// Retry limit.
		ssize_t ReallyRead(0);	// Really read.

		// Set new time limit.
		TSC->_SET_WAIT_TIME_(MAIN_SOCKET,3);	// 3s.
		
		/*	Send command to server.	*/
		TSC->_WRITE_SOCK_(Comm_Sock,"ls",2);

		// Poll set by caller.

		while (true && ! Client_Should_Be_Stop)
		{
			if (Retry != 0)	// Retry check
				;
			else
			{
				cerr<<"FTPR_Client: _LS_ retry limit."<<endl;
				break;
			}	

			if (TSC->_POLL_(MAIN_SOCKET) != -1)
			{
				if (TSC->_Check_Main_(POLLRDNORM))
				{
					ReallyRead=TSC->_READ_SOCK_(Comm_Sock,Buffer,Buffer_Len);	// Socket read.
					if (ReallyRead > 0u)
					{
						if (strncmp(Buffer,"#EOF#\0",6) == 0)	// Check if it is end.
							break;
						else
							Buffer[ReallyRead]='\0';	// Append end of string.

						cout<<Buffer<<endl;	// View item name.
						Retry=2;	// Reset retry.
					}
					else
					{
						cerr<<"FTPR_Client: ls failed."<<endl;	// Read return zero or a negative number.
						break;
					}
				}
				else
					--Retry; // Timeout.
			}
			else
				syslog(LOG(LOG_NOTICE),"FTPR_Server: _LS_ poll had detected an error.");
		}

		
		// At end,must restore socket env.
		TSC->_SET_WAIT_TIME_(MAIN_SOCKET,OldTime_Limit);	// Reset timer.

	}

	// Memory Buffer saved the command string,it will be send to server,and the result also use there to saving.
	void FTPR_client_class::_CD_(const int & Comm_Sock,char * & Buffer,const size_t & Buffer_Len)
	{
		// Poll set by caller at front.
		// This function will set new time limit 3s.

		const unsigned short int OldTime_Limit(TSC->_GET_WAIT_TIME_(MAIN_SOCKET));	// Old env.
		ssize_t ReallyRead(0);	// Readed data length.
		
		TSC->_SET_WAIT_TIME_(MAIN_SOCKET,3);	// New time limit.
		
		/*	Send command to server.	*/
		TSC->_WRITE_SOCK_(Comm_Sock,Buffer,strlen(Buffer));

		if (TSC->_POLL_(MAIN_SOCKET) != -1)
			if (TSC->_Check_Main_(POLLRDNORM))
			{
				ReallyRead=TSC->_READ_SOCK_(Comm_Sock,Buffer,Buffer_Len);	// Read socket.
				if (ReallyRead > 0u)
				{
					Buffer[ReallyRead]='\0';
					cout<<"FTPR_Client: cd - Message from server - "<<Buffer<<endl;
				}
				else
					cerr<<"FTPR_Client: cd - read failed."<<endl;
			}
			else
				cerr<<"FTPR_Client: cd - time out."<<endl;
		else
			cerr<<"FTPR_Client: cd - wait data failed."<<endl;
	
		

		// Reset time limit.

		TSC->_SET_WAIT_TIME_(MAIN_SOCKET,OldTime_Limit);	// Restore.

	}



	void FTPR_client_class::_GET_FILE_(const int & Comm_Sock,FGU & Env_GET)
	{
		// Get file from server.
		// Client would create a tempfile to save file data,if finished work as normaly,then link this file.
		// Caller must set socket interface before call this funcion.
		// Client will use thread_socket to make connection with server in posix thread.
		// Client must get file lock for temp file.
		
		/*	Env_GET fill by caller.	*/
				
		char TempFileName[]="ftprc_tmpf.XXXXXX";	// Temp file name.
		int TmpFile(-1);	// For tempfile descriptor.
		memset(&File_Info,'\0',sizeof(GET_UP));	// Clear memory.
		short int *DownState(NULL);	// For thread return.
		struct stat For_Target_File;	// For target file.
		struct flock TempFile_Lock={
				.l_type=F_WRLCK,
				.l_whence=SEEK_SET,
				.l_start=0,
				.l_len=0,
			};	// File lock,lock up all file with read only.
		bool If_Had_Lock(false);	// For check if lock succeed.

		pthread_t Downloader;		// Thread id object.
		


		/*	Get message buffer memory.	*/
		/* Because the message is could to guess so need not allocs large memory piece.	*/
		char *Msg_Buff=new char[128];	// 128Bytes.
		if (NULL == Msg_Buff)
		{
			cerr<<"FTPR_Client: None of space to receiving messages.\n";
			return;
		}
		else;
		//	Env
		
		if ((TmpFile=FIDC->_MKSTEMP_(TempFileName)) < 0)		// TempFile.
		{
			cerr<<"FTPR_Client: Failed to create tempfile."<<endl;
			goto _GET_FILE_Return;
		}
		else;

		/*	Make command string and send to server.	*/
		snprintf(Msg_Buff,128,"get %s",(const char *)The_CMD_Hit.CMDP);
		TSC->_WRITE_SOCK_(Comm_Sock,Msg_Buff,strlen(Msg_Buff));

		/*	After received '#OK#' then ready to read data for a structure.	*/
		TSC->_POLL_(MAIN_SOCKET);	// Wait data.
		if (TSC->_Check_Main_(POLLRDNORM))	// Check event.
		{
			memset(Msg_Buff,'\0',128);	// Clear invalid data.
			if (TSC->_READ_SOCK_(Comm_Sock,Msg_Buff,128) <= 0)	// Read message.
			{
				cerr<<"FTPR_Client: Failed to read message."<<endl;
				goto _GET_FILE_Return;
			}
			else
				if (0 == strncmp(Msg_Buff,"#OK#",4))
					;
				else
				{
					cout<<"FTPR_Client: Message from server - "<<Msg_Buff<<endl;
					goto _GET_FILE_Return;
				}
		}
		else
		{
			cerr<<"FTPR_Client: Server no responation."<<endl;
			goto _GET_FILE_Return;
		}


		if (TSC->_READ_SOCK_(Comm_Sock,&File_Info,sizeof(GET_UP)) != sizeof(GET_UP))
		{
			cerr<<"FTPR_Client: File info incomplete."<<endl;
			goto _GET_FILE_Return;
		}
		else
		{
			// Reset tempfile size and get read locker for it.
			if (FIDC->_FTRUNCATE_(TmpFile,File_Info.FileSize) < 0)
			{
				cerr<<"FTPR_Client: Can not reset file length."<<endl;
				goto _GET_FILE_Return;
			}
			else
				if (FIDC->_LOCK_FILE_(TmpFile,F_SETLK,&TempFile_Lock) < 0)	// Set file lock.
				{
					cerr<<"FTPR_Client: Can not lock up file."<<endl;
					goto _GET_FILE_Return;
				}
				else
					If_Had_Lock=true;


			TSC->_SET_PORT_(THREAD_SOCKET,File_Info.UsePort);	// Thread will connect to this port.

			/*	Create new pthread.	*/
			PTC->_LOCK_MUTEX_();
			if (PTC->_CREATE_THREAD_(&Downloader,NULL,Thread_FF,&Env_GET) != 0)
			{
				cerr<<"FTPR_Client: Failed to start downloader."<<endl;
				PTC->_UNLOCK_MUTEX_();
				goto _GET_FILE_Return;
			}
			else
			{
				PTC->_UNLOCK_MUTEX_();	// Release lock.
				cout<<"FTPR_Client: Downloader - "<<hex<<Downloader<<dec<<" start."<<endl;
			}
			sleep(1);
			PTC->_LOCK_MUTEX_();	// Get lock.
			PTC->_JOIN_THREAD_(Downloader,(void **)&DownState);

			// Check status.
			if (*DownState == FF_GET_SUSS)
			{
				cout<<"FTPR_Client: Succeed to receive file."<<endl;
				if (fstat(TmpFile,&For_Target_File) < 0)	// Get tempfile info
					cerr<<"FTPR_Client: Failed to get tmp file info."<<endl;
				else
					if (For_Target_File.st_size != File_Info.FileSize)	// Check file size.
						cerr<<"FTPR_Client: File size not match."<<endl;
					else
						FIDC->_LINK_(TempFileName,(char *)The_CMD_Hit.CMDP);
			}
			else
			{
				cerr<<"FTPR_Client: Failed to receive file."<<endl;
				_FTPR_Logger_(*DownState);
			}

			PTC->_UNLOCK_MUTEX_();	// Release lock.

		}
		

		//	Primary


		_GET_FILE_Return:

			delete[] Msg_Buff;
			Msg_Buff=NULL;
			if (TmpFile > 0)
			{
				// Check if had placed a lock on it.

				if (If_Had_Lock)
				{
					TempFile_Lock.l_type=F_UNLCK;	// Unlock
					(void)FIDC->_LOCK_FILE_(TmpFile,F_SETLK,&TempFile_Lock);
				}
				else;

				FIDC->_CLOSE_(TmpFile);	// Close temp file.
			}
			else;

		//	Return
	}


	void FTPR_client_class::_UP_FILE_(const int & Comm_Sock,FGU & Env_UP)
	{
		// For upload file,client must check front at first.
		
		struct stat File_Stat;	// To save target file stat info.
		int Target_FileDes(-1);	// Target file descriptor.
		pthread_t Uploader;	// Uploader thread id.
		short int *UpState(NULL);	// Save thread result.
		memset(&File_Info,'\0',sizeof(GET_UP));	// Clear invalid data.
		const char *Target_Pointer((const char *)The_CMD_Hit.CMDP);	// For useful.
		char *Msg_Buff=new char[128];	// To get message buffer.
		if (NULL == Msg_Buff)
		{
			cerr<<"FTPR_Client: Can not open buff space."<<endl;
			goto _UP_FILE_Return;
		}

		//	Env

		/*	Check file whether had existed.	*/
		File_Stat=FIDC->_LSTAT_((const char *)The_CMD_Hit.CMDP);
		if (File_Stat.st_size == 0)
		{
			cerr<<"FTPR_Client: Target file have not existed."<<endl;
			goto _UP_FILE_Return;
		}
		else
		{
			_Copy_String_(File_Info.FILENAME,Target_Pointer,strlen(Target_Pointer));	// Set file info obj.
			File_Info.FileSize=File_Stat.st_size;	// File size.
		}

		/*	Open target file.	*/
		if ((Target_FileDes=FIDC->_OPEN_(Target_Pointer,O_RDONLY)) < 0)
		{
			cerr<<"FTPR_Client: Failed to open target file."<<endl;
			goto _UP_FILE_Return;
		}
		else
		{
			/*	Make command string and send to server.	*/
			snprintf(Msg_Buff,128,"up %s",Target_Pointer);
			TSC->_WRITE_SOCK_(Comm_Sock,Msg_Buff,strlen(Msg_Buff));	// Send command.
		}


		// Wait server'message.
		TSC->_POLL_(MAIN_SOCKET);
		if (TSC->_Check_Main_(POLLRDNORM))
		{
			memset(Msg_Buff,'\0',128);	// Clear invalid data.
			TSC->_READ_SOCK_(Comm_Sock,Msg_Buff,127);
			if (0 == strncmp(Msg_Buff,"#UPINFO#",8))
				TSC->_WRITE_SOCK_(Comm_Sock,&File_Info,sizeof(GET_UP));	// Send target file info.
			else
			{
				cerr<<"FTPR_Client: Message from server - "<<Msg_Buff<<endl;
				goto _UP_FILE_Return;	
			}
		}
		else
		{
			cerr<<"FTPR_Client: Server no responation."<<endl;
			goto _UP_FILE_Return;
		}

		// Wait '#OK#'.
		TSC->_POLL_(MAIN_SOCKET);
		if (TSC->_Check_Main_(POLLRDNORM))
		{
			memset(Msg_Buff,'\0',128);	// Clear invalid data.
			TSC->_READ_SOCK_(Comm_Sock,Msg_Buff,127);
			if (0 == strncmp(Msg_Buff,"#OK#",4))
				if (TSC->_READ_SOCK_(Comm_Sock,&File_Info,sizeof(GET_UP)) != sizeof(GET_UP))	// Server had fill port field.
					cerr<<"FTPR_Client: Receive file info had error."<<endl;
				else
				{
					// Set port and then open thread.
					TSC->_SET_PORT_(THREAD_SOCKET,File_Info.UsePort);
			
					PTC->_LOCK_MUTEX_();
					if (PTC->_CREATE_THREAD_(&Uploader,NULL,Thread_FF,&Env_UP) != 0)
					{
						cerr<<"FTPR_Client: Open uploader failed."<<endl;
						PTC->_UNLOCK_MUTEX_();
						goto _UP_FILE_Return;
					}
					else
					{
						PTC->_UNLOCK_MUTEX_();
						cout<<"FTPR_Client: Uploader - "<<hex<<Uploader<<dec<<" start."<<endl;
					}
			
					sleep(1);
					PTC->_LOCK_MUTEX_();	// Lock up again.
					PTC->_JOIN_THREAD_(Uploader,(void **)&UpState);	// Wait thread.
				
					if (*UpState == FF_UP_SUSS)
						cout<<"FTPR_Client: Finished upload file,please use 'ls' to check status"<<endl;
					else
						cerr<<"FTPR_Client: Failed to upload file."<<endl;

					_FTPR_Logger_(*UpState);
	
		
					PTC->_UNLOCK_MUTEX_();	// Release lock.
				}
			else
				cerr<<"FTPR_Client: Message from server - "<<Msg_Buff<<endl;
		}
		else
				cerr<<"FTPR_Client: Server no responation."<<endl;

		//	Primary

		_UP_FILE_Return:

			delete[] Msg_Buff;
			Msg_Buff=NULL;
			if (Target_FileDes > 0)
				FIDC->_CLOSE_(Target_FileDes);
			else;

		//	Return
	}

	bool FTPR_client_class::_LINK_(const int & Comm_Sock)	// 'true' on success,'false' on error.
	{
		bool Function_State(false);	// Suppose it's false.

		char *IP_Addr=new char[32];	// This array for read address:port.
		if (NULL == IP_Addr)	// Check.
			return Function_State;
		else;
		short int Invoke_Result(-1);	// For function calling.
		struct sockaddr_in *Server_Addr=new struct sockaddr_in;
		if (NULL == Server_Addr)
			goto _LINK_Return;
		else
			Server_Addr->sin_family=AF_INET;	// Default use IPv4.
						
		//	Env

		cout<<"Please text IP:Port - ";
		cin.getline(IP_Addr,32);	// Get ip and port.	
		
		Invoke_Result=_Take_IP_Port_(IP_Addr,(struct sockaddr *)Server_Addr);

		switch (Invoke_Result)
		{
			case C_TAKE_ERR:
				cerr<<"FTPR_Client: Incorrect ip:port string."<<endl;
				goto _LINK_Return;
			case C_TAKE_IP_ERR:
				cerr<<"FTPR_Client: IP Address error."<<endl;
			case C_TAKE_PORT_TOO_LARGE:
				cerr<<"FTPR_Client: The port number is too large."<<endl;
				goto _LINK_Return;
			case C_TAKE_INVALID_STR:
				cerr<<"FTPR_Client: Invalid ip address string."<<endl;
				goto _LINK_Return; 
			case C_TAKE_INVALID_FAMILY:
				cerr<<"FTPR_Client: Invalid network family."<<endl;
			default:
				;
		}

		TSC->_SET_ADDR_(MAIN_SOCKET,(struct sockaddr *)Server_Addr);	// Set main address structure.
		TSC->_SET_ADDR_(THREAD_SOCKET,(struct sockaddr *)Server_Addr);	// Because in same host.

		for (unsigned short int i(Retry_ToLink); i > 0; --i)
			if (TSC->_CONNECT_(MAIN_SOCKET) < 0)	// This is to connect communication socket.
				continue;
			else
			{
				Function_State=true;	// Succeed.
				break;
			}

		//	Primaray

	
	_LINK_Return:
		
		// Use delete to null pointer is secury operation.

		delete Server_Addr;
		Server_Addr=NULL;

		delete[] IP_Addr;
		IP_Addr=NULL;	

		return Function_State;


		//	Return

	}

	



	void FTPR_client_class::_HELP_(void)
	{
		cerr<<"File-Transport client :"<<endl
			<<"Commands :"<<endl
			<<"\tls - List files in the remote server."<<endl
			<<"\tcd - Change directory for remote server."<<endl
			<<"\tget file - Download file from remote server."<<endl
			<<"\tup file - Upload file to remote server."<<endl
			<<"\tlogout - Disconnection with a server which had connected."<<endl
			<<"\texit - Disconnection and quit program."<<endl
			<<"\thelp - View help messages."<<endl;
	}



	/*	Protected.		*/

	void FTPR_client_class::_Command_Parsing_(const char *Command)
	{
		// Check command and set WCH structure which is inherit from basic class.
		// Client command : ls,cd,get file,up file,link,logout,exit,help

		// Use strncmp to check command and also use shortly checking.

		The_CMD_Hit.CMDN=255;	// 255 have not be defined.
		The_CMD_Hit.CMDP=NULL;

		//	Env
	
		if (Command)
			if (0 == strncmp(Command,"ls",2))
				The_CMD_Hit.CMDN=LS;
			else if (0 == strncmp(Command,"cd",2))
				The_CMD_Hit.CMDN=CD,The_CMD_Hit.CMDP=Command+3;
			else if (0 == strncmp(Command,"ge",2))
				The_CMD_Hit.CMDN=GET,The_CMD_Hit.CMDP=Command+4;
			else if (0 == strncmp(Command,"up",2))
				The_CMD_Hit.CMDN=UP,The_CMD_Hit.CMDP=Command+3;
			else if (0 == strncmp(Command,"li",2))
				The_CMD_Hit.CMDN=LINK;
			else if (0 == strncmp(Command,"lo",2))
				The_CMD_Hit.CMDN=LOGOUT;
			else if (0 == strncmp(Command,"ex",2))
				The_CMD_Hit.CMDN=EXIT;
			else if (0 == strncmp(Command,"he",2))
				The_CMD_Hit.CMDN=HELP;
			else;
		else;

		//	

	}

	short int FTPR_client_class::_Init_FC_(const void *SETTINGS)
	{

		const Client_Init *Init_Set((const Client_Init *)SETTINGS);	// For easly using it.
		short int Function_Result(-1);	// 0 on success,-1 on error.

		//	Env

		/*	Use setting options parameter to init all interface classes.	*/
		/*	TCP and FID had defined the default build method for client.	*/
		/*	So client do not have to apply a init ordef for all classes.	*/


		TSC=new TCP_SOCK_class;
		if (NULL == TSC)
		{
			syslog(LOG(LOG_ERR),"FTPR_Client: Can not create socket interface.");
			goto _Init_FC_Return;
		}
		else
			if (TSC->State_Of_Initialization_SOCK)
				;
			else
			{
				syslog(LOG(LOG_ERR),"FTPR_Client: Can not init socket interface.");
				goto _Init_FC_Return;
			}

		PTC=new THREAD_class;
		if (NULL == PTC)
		{
			syslog(LOG(LOG_ERR),"FTPR_Client: Can not create posix thread interface.");
			goto _Init_FC_Return;
		}
		else
			if (PTC->State_Of_Initialization_THREAD)
				;
			else
			{
				syslog(LOG(LOG_ERR),"FTPR_Client: Can not init posix thread interface.");
				goto _Init_FC_Return;
			}

		SYSS=new SYSTEM_SIGNAL_class;
		if (NULL == SYSS)
		{
			syslog(LOG(LOG_ERR),"FTPR_Client: Can not create signal interface.");
			goto _Init_FC_Return;
		}
		else
			if (SYSS->State_Of_Initialization_SIGNAL)
				;
			else
			{
				syslog(LOG(LOG_ERR),"FTPR_Client: Can not init signal interface.");
				goto _Init_FC_Return;
			}
		
		FIDC=new FID_class;	// This will invoke the default build method.
		if (NULL == FIDC)
		{
			syslog(LOG(LOG_ERR),"FTPR_Client: Can not create File&Directory interface.");
			goto _Init_FC_Return;
		}
		else
			if (FIDC->State_Of_Initialization_FID)
				if (chdir(Init_Set->DP) > 0)	// Make a illusion that is while FIDC init it would change directory.
					;
				else
				{
					syslog(LOG(LOG_ERR),"FTPR_Client: Can not change directory to download path.");
					goto _Init_FC_Return;
				}
			else
			{
				syslog(LOG(LOG_ERR),"FTPR_Client: Can not init File&Directory interface.");
				goto _Init_FC_Return;
			}

		Function_Result=0;	// We hope function would process to there.

		//	Primary

	_Init_FC_Return:

		return Function_Result;		

		//	Return
	}


	/*	Public.			*/
	FTPR_client_class::FTPR_client_class():Retry_ToLink(Default_Retry_ToLink),DownloadPath(NULL)
	{
		cout<<"FTPR_Client: Create ftprc object."<<endl;

		/*	The array memory alloc let _Init_Client_ to complete.	*/
		//	Primary
	}

	FTPR_client_class::~FTPR_client_class()
	{
		_Client_ShutDown_();
		cout<<"FTPR_Client: Destroied ftprc object."<<endl;
		syslog(LOG(LOG_NOTICE),"FTPR_Client: FTPR_Client quit...");

		//	Primary
	}



	bool FTPR_client_class::_Client_Init_(void)	/*	This function have not to as a piece in shared library.	*/
	{
		/*	At first to invoke _Read_Setting_ for get configure optionals.	*/
		/*	If have not error had detected,then invoke _Init_FC_ to initialize feature classes.	*/
		/*	If have not error had detected in _Init_FC_,then return true.	*/
		/*	Once failed for setting and init will return false from this function.	*/
		
		//	Get DownloadPath memory space.
		DownloadPath=new char[pathconf("/",_PC_PATH_MAX)];	// _PC_PATH_MAX constant could get system limit for path max length.
		if (NULL == DownloadPath)	/* Check */
		{
			cerr<<"FTPR_Client: Client can not get memory for save downloadpath."<<endl;
			return false;
		}
		else;
		//	Memory if succeed to alloced,it will be destroied by _Client_ShutDown_.

		
		bool Client_Init_State(true);	// Suppose it's success.
		short int Invoke_Result(-1);	// The setting function and init function all return 0 on success and all return -1 on fail.	
		Client_Init Client_Optionals={
				.FSS=Share_Set,
				.RTL=0,			// This variable must reset after _Read_Setting_ finished.
				.DP=(const char *&)DownloadPath,
			};	// This structure object contains the shared setting object-pointer and client settings which uniqued.

		//	Env
		

		cout<<"CONFIG FILE : "<<CONFIG_FILE<<endl;

		/*	Try to read settings.	*/
		Invoke_Result=_Read_Setting_(CONFIG_FILE);

		if (Invoke_Result < 0)
		{
			Client_Init_State=false;
			cerr<<"FTPR_Client: Client failed to read configure optionals."<<endl;
		}
		else
		{
			cout<<"FTPR_Client: Client succeed to reading setting options."<<endl;
			Client_Optionals.RTL=Retry_ToLink;	// Reset it.
		}

		/*	Try to initializing feature classes.	*/
		
		if (Client_Init_State)
		{
			Invoke_Result=_Init_FC_(&Client_Optionals);
			if (Invoke_Result < 0)
				Client_Init_State=false;
			else
			{
				// Set signal action.
			
				// SIGTERM		 Termination.
				// SIGINT		 Interrupt.
				// SIGHUP		 HungUp.
				// SIGALRM		 Alarm.

				/*	Block set had filled.	*/
				SYSS->_SIGDELSET_(BLOCK_SET,SIGTERM);
				SYSS->_SIGDELSET_(BLOCK_SET,SIGINT);
				SYSS->_SIGDELSET_(BLOCK_SET,SIGHUP);
				SYSS->_SIGDELSET_(BLOCK_SET,SIGALRM);
				
				// Change block set of this process.
				SYSS->_SIGPROCMASK_(SIG_SETMASK,NULL);

				// End to set mask.

				// Set actions for tar get signals.

				SYSS->_SIGADDSET_(NORMAL_SET,SIGINT);
				SYSS->_SIGACTION_(SIGTERM,Sigaction_Client_Sigterm);
				SYSS->_SIGDELSET_(NORMAL_SET,SIGINT);
				SYSS->_SIGADDSET_(NORMAL_SET,SIGTERM);
				SYSS->_SIGACTION_(SIGINT,Sigaction_Client_Sigint);
			
				SYSS->_SIGDELSET_(NORMAL_SET,SIGINT);
				SYSS->_SIGACTION_(SIGHUP,Sigaction_Client_Sighup);
				SYSS->_SIGACTION_(SIGALRM,Sigaction_Client_Sigalrm);
		

				cout<<"FTPR_Client: Client succeed to initializing feature modes."<<endl;
			}
		}
		else;

		//	Primary


		return Client_Init_State;

		//	Return
	}



	void FTPR_client_class::_Client_(void)
	{
		//	If init had not successfully,print failed message and quit.
		if (_Client_Init_())
		{
			cout<<"FTPR_Client: ftprc had initialized successfully!"<<endl;
			_Client_WorkUp_();
		}
		else
			cerr<<"FTPR_Client: Failed to init program."<<endl;
	}

	void FTPR_client_class::_Client_ShutDown_(void)
	{
		if (TSC)
		{
			delete TSC;
			TSC=NULL;	// Proposal constraction.
			syslog(LOG(LOG_NOTICE),"FTPR_Client: Destroied tcp socket interface.");
		}
		else;

		if (PTC)
		{
			delete PTC;
			PTC=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Client: Destroied posix thread interface.");
		}
		else;

		if (SYSS)
		{
			delete SYSS;
			SYSS=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Client: Destroied system signal interface.");
		}
		else;

		if (FIDC)
		{
			delete FIDC;
			FIDC=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Client: Destroied file&directory interface.");
		}
		else;

		if (DownloadPath)
		{
			delete[] DownloadPath;
			DownloadPath=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Client: Destroied path space memory.");
		}
		else;

		//	Primary

	}





}




