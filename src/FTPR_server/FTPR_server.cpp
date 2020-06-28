//	FTPR_server.cpp
//	Version : 0.2
//	Date : Fri Jun 12 10:31:41 2020
//	Last revise : Fri Jun 26 20:44:? 2020
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
//
//	Fix :
//		1> Make command work code as models.
//		2> Delete retry for init.
//		3> Make a server logger to record all logs.


#include"FTPR_server.h"

namespace FTPR_SERVER{






// Define signal action.

	void _Server_Signal_Sigterm_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGTERM received from %ld - terminate",Receive_Info->si_signo,Receive_Info->si_pid);

		// How to stop server,a simple method is declare a global variable.
		// And in the code,only this function to changed it.
		// Server finished a working would check this variable,if it is flase,stop.

		Server_Should_Not_To_Stop=false;	// Server should to stop.

	}

	void _Server_Signal_Sigint_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGINT received from %ld - terminate",Receive_Info->si_signo,Receive_Info->si_pid);
	
		union sigval INFO={.sival_int=1};	// union.
		sigqueue(getpid(),SIGTERM,INFO);	// Send SIGTERM to itself.
	}

	void _Server_Signal_Sighup_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGHUP received from %ld - ignores",Receive_Info->si_signo,Receive_Info->si_pid);
	}

	void _Server_Signal_Sigalrm_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGALRM received from %ld - ignores",Receive_Info->si_signo,Receive_Info->si_pid);
	}





	// C++ standard io.
	using std::ifstream;
	using std::ios;

	// Virtual Functions in protected.
	// And for simply to update,will make this function as a share lib.
	short int FTPR_Server_class::_Read_Setting_(const char *Setting_File)
	{

		// For make same format to return function code,declare a variable.
		short int _Read_Setting_Code(0);


		// Get reading buffer.
		char *Setting_Buffer=new char[64];	// 64B.
		if (! Setting_Buffer)
		{
			_Read_Setting_Code=S_RS_GET_MEM_ERR;	
			return _Read_Setting_Code;
		}
		else;
		
		// Temporary string position pointer.
		char *Temp_Char_Pointer(NULL);

		// Temporary value for get number.
		int Temporary_Value(0);
	

		// FTPR_Basic had been defined setting read function for shared settings.
		// When it succeed to read,return true.
		if (FTPR_Basic::_Read_Shared_Setting_(Setting_File))
			;
		else
		{
			
			_Read_Setting_Code=S_RS_SHARE_SET_ERR;
			delete[] Setting_Buffer;
			return _Read_Setting_Code;
		}

		ifstream Read;	// Define a ifstream object.
		Read.open(Setting_File,ios::in);	// ASCII input.

		/* Check it was succeed to opened.	*/
		if (Read.is_open())
			;
		else
		{
			_Read_Setting_Code=S_RS_OPEN_CONFIG_ERR;
			delete[] Setting_Buffer;
			return _Read_Setting_Code;
		}

		// Entry a cycle to read unique setting optionals.
		// Stop while fail bit set out.
		while (! Read.fail())
		{
			// Line to read.
			Read.getline(Setting_Buffer,64);
				
			if ('#' == *Setting_Buffer) // Skip comment.
				continue;
			else
			{
				if (strncmp(Setting_Buffer,"DUPORT",6) == 0)
					DownUp_Port=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer)) > 0)?Temporary_Value:Default_DUPORT;
				else if (strncmp(Setting_Buffer,"CWAITS",6) == 0)
					Comm_Wait_Seconds=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer) > 0))?Temporary_Value:Default_CWAITS;
				else if (strncmp(Setting_Buffer,"CPORT",5) == 0)
					Comm_Port=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer) > 0))?Temporary_Value:Default_CPORT;
				else if (strncmp(Setting_Buffer,"RPATH",5) == 0)			
				{
					Temp_Char_Pointer=strstr(Setting_Buffer,"=");
					if (Temp_Char_Pointer)
						_Copy_String_(Root_Path,Temp_Char_Pointer+1,strlen(Temp_Char_Pointer+1));
					else
						_Copy_String_(Root_Path,Default_RPATH,10);
				}
				else;
			}
		}

		// Check it is normaly finished.
		if (Read.eof())
			_Read_Setting_Code=S_RS_SET_OVER;
		else
			_Read_Setting_Code=S_RS_SET_ERR;
		
		// Close file.
		Read.clear();	// Flush flag.
		Read.close();
		
		delete[] Setting_Buffer;

		return _Read_Setting_Code;

	}


	
	short int FTPR_Server_class::_Init_FC_(const void *SETTINGS)
	{
		// For simply use the parameter,defined a structure pointer.
		const Server_Set *Init_Set=(const Server_Set *)SETTINGS;
	
		// Error code return variable.
		short int Init_Code(S_INIT_FC_SUSS);	
	
		// Set umask.
		umask(Init_Set->SHS->Make_File_Mask);

		// And then create class objects and check init status.
	
		// Create FID.
		FIDC=new FID_class(Init_Set->Root_Path);
		// Check status.
		if (FIDC)
			if (FIDC->State_Of_Initialization_FID)
			{
				PTC=new THREAD_class;	// Create POSIX THREAD.
				if (PTC)
					if (PTC->State_Of_Initialization_THREAD)
					{
						TSC=new TCP_SOCK_class(Init_Set->CPT,Init_Set->DUP,Init_Set->SHS->Network_IO_Timeout,Init_Set->SHS->Network_IO_Timeout);
						if (TSC)
							if (TSC->State_Of_Initialization_SOCK)
							{
								SYSS=new SYSTEM_SIGNAL_class;
								if (SYSS)
								{
									if (SYSS->State_Of_Initialization_SIGNAL)
									{
										Init_Code=S_INIT_FC_SUSS;
										goto INIT_QUIT;
									}
									else
									{
										Init_Code=S_INIT_SIGNAL_ERR;
										goto INIT_LABEL1;
									}
								}
								else
								{
									Init_Code=S_INIT_CRE_SIGNAL_ERR;
									goto INIT_LABEL2;
								}
							}
							else
							{
								Init_Code=S_INIT_SOCK_ERR;
								goto INIT_LABEL2;
							}
						else
						{
							Init_Code=S_INIT_CRE_SOCK_ERR;
							goto INIT_LABEL3;
						}
					}
					else
					{
						Init_Code=S_INIT_THREAD_ERR;
						goto INIT_LABEL3;
					}
				else
				{
					Init_Code=S_INIT_CRE_THREAD_ERR;
					goto INIT_LABEL4;
				}

			}
			else
			{
				Init_Code=S_INIT_FID_ERR;
				goto INIT_LABEL4;
			}
		else
		{
			Init_Code=S_INIT_CRE_FID_ERR;
			goto INIT_QUIT;
		}
		
		// These symbols for the case of feature classes init failed to be made.

		INIT_LABEL1:	
			delete SYSS;
		INIT_LABEL2:	
			delete TSC;
		INIT_LABEL3:
			delete PTC;
		INIT_LABEL4:
			delete FIDC;


		INIT_QUIT:

			return Init_Code;
		
	}


	void FTPR_Server_class::_Command_Parsing_(const char *Command)
	{
		// Command server to respones :
		// 	ls,cd,logout,GET,UP.
		// This function will scan command string and full structure WCH in FTPR_Basic.

		// For improve speed,this function will use two character to guess command.

		if (Command)
		{
			if (strncmp(Command,"ls",2) == 0)
				The_CMD_Hit.CMDN=LS,The_CMD_Hit.CMDP=NULL;
			else if (strncmp(Command,"cd",2) == 0)
				The_CMD_Hit.CMDN=CD,The_CMD_Hit.CMDP=Command+3;
			else if (strncmp(Command,"lo",2) == 0)
				The_CMD_Hit.CMDN=LOGOUT,The_CMD_Hit.CMDP=NULL;
			else if (strncmp(Command,"GE",2) == 0)	
				The_CMD_Hit.CMDN=GET,The_CMD_Hit.CMDP=Command+4;
			else if (strncmp(Command,"UP",2) == 0)
				The_CMD_Hit.CMDN=UP,The_CMD_Hit.CMDP=Command+3;
			else	// None defined command.
				The_CMD_Hit.CMDN=0,The_CMD_Hit.CMDP=NULL;
		}
		else	// NULL pointer also like none command defintion.
			The_CMD_Hit.CMDN=0,The_CMD_Hit.CMDP=NULL;		
	}


	bool FTPR_Server_class::_Server_Init_(void)
	{
		bool _Server_Init_Status_(true);	// Suppose it's succeed.
		short int Return_Function(0);	// For record return value.
		Server_Set Server_Set_Config={.SHS=&Share_Set};	// Share set.

		// At first,read setting.

		Return_Function=_Read_Setting_(CONFIG_FILE);
		if (Return_Function != S_RS_SET_OVER)
		{
			// Once record for last.
			_Server_Logger_(Return_Function);	
			_Server_Init_Status_=false;
			return _Server_Init_Status_;
		}
		else
			_Server_Logger_(Return_Function);
		

		// Full Server_Ser.
		Server_Set_Config.DUP=DownUp_Port;
		Server_Set_Config.CPT=Comm_Port;
		Server_Set_Config.Root_Path=Root_Path;
		
		// At second,initialize variables.
		// These will be setting in read setting stage.

		// At third,set root tree.
		// At fourth,change work home.
		// They are need class FID.

		// At fiveth,set signal action.
		// Must create class as before.

		// At sixth,init feature classes.
		Return_Function=_Init_FC_((const void *)&Server_Set_Config);
		if (Return_Function == S_INIT_FC_SUSS)
		{
			// Set signal action. 0->T,-1->F.
			if (SYSS->_SIGACTION_(SIGALRM,_Server_Signal_Sigalrm_) < 0)
				Return_Function=S_SIGALRM_ERR,_Server_Init_Status_=false;
			else
				if (SYSS->_SIGACTION_(SIGHUP,_Server_Signal_Sighup_) < 0)
					Return_Function=S_SIGHUP_ERR,_Server_Init_Status_=false;
				else
					if (SYSS->_SIGACTION_(SIGINT,_Server_Signal_Sigint_) < 0)
						Return_Function=S_SIGINT_ERR,_Server_Init_Status_=false;
					else
						if (SYSS->_SIGACTION_(SIGTERM,_Server_Signal_Sigterm_) < 0)
							Return_Function=S_SIGTERM_ERR,_Server_Init_Status_=false;
						else;
		}
		else
			_Server_Init_Status_=false;

		_Server_Logger_(Return_Function);

		return _Server_Init_Status_;

	}



	void FTPR_Server_class::_Server_WorkUp_(void)
	{
		char *UserCmd(NULL);
		try{
			// For command buffer.
			UserCmd=new char[Share_Set.Message_Buff_Size];
			if (UserCmd)
				;
			else
				throw 0;
		}
		catch(short int ZERO)
		{
			UserCmd=new char[Share_Set.Message_Buff_Size];
			if (UserCmd)
				;
			else
			{
				_Server_Logger_(S_FAIL_MEM);	// Because WORKUP_MEM_ERR = 0
				return;
			}

		}
		
		// For accept.
		int New_Chat(-1);

		// Thread_FF arguments.
		// This structure have some tcp setting parameters.
		NETIOINFO NetIO={
			.Network_File_IO_Buffer=Share_Set.File_Buff_Size,
			.What_Behavior=0,
			.Retry_Number=Share_Set.Retry_Net_IO,
			};

		// This structure will send to Thread_FF.
		FGU Thread_Resource={
			.Tcp_Sock=TSC,
			.Fid=FIDC,
			.IO_Thread=PTC,
			.Timer_Timeout=Comm_Wait_Seconds,
			.Tcp_IO=&NetIO,	
			};

		// Their member value could be determines before them had be used.

		// For peer address.
		struct sockaddr_in Client_Address;
		socklen_t Address_Length(1);
		char Address_String[16];	// For string.

		// Bind socket.
		if (TSC->_BIND_(MAIN_SOCKET) < 0)
		{
			_Server_Logger_(S_BIND_ERR);
//			syslog(LOG(LOG_ERR),"FTPR_Server: Can not bind communication socket.");
			goto _WorkUp_Quit;
		}
		else
			if (TSC->_LISTEN_(MAIN_SOCKET,1) < 0)
			{
				_Server_Logger_(S_LISTEN_ERR);
//				syslog(LOG(LOG_ERR),"FTPR_Server: Can not open listen queue for communication socket.");
				goto _WorkUp_Quit;
			}
			else;

		// Entry cycle.

		while (true && Server_Should_Not_To_Stop)
		{
			// Wait connection arrived.	
			New_Chat=TSC->_ACCEPT_(MAIN_SOCKET,(struct sockaddr *)&Client_Address,&Address_Length);
			if (New_Chat > 0)
			{
				// Succeed to accept a connection.
				syslog(LOG(LOG_NOTICE),"FTPR_Server: Succeed to accept a connection %s:%hu",TSC->_INET_NTOP_(AF_INET,&Client_Address,Address_String,16),TSC->_NTOHS_(Client_Address.sin_port));

				TSC->_POLL_SET_(New_Chat,MAIN_SOCKET,POLLRDNORM);	// Set pollfd.

				// Entry communication cycle.

				do
				{

					TSC->_POLL_(MAIN_SOCKET);	// Wait poll timeout or data arrived.
					if (! TSC->_Check_Main_(POLLRDNORM))
						continue;
					else;	
						
					// Client sent data.

					if (TSC->_READ_SOCK_(New_Chat,UserCmd,Share_Set.Message_Buff_Size) > 0)
					{
						// Call command parasing.

						_Command_Parsing_(UserCmd);

						// Check WCH structure.

						switch (The_CMD_Hit.CMDN)
						{
							case LS:
								_Server_Logger_(S_CMD_LS);							

								// Cycle to write data.
								_LS_(New_Chat,UserCmd);

								break;

							case CD:
								_Server_Logger_(S_CMD_CD);
								
								_CD_(New_Chat,(const char *)The_CMD_Hit.CMDP);
								break;

							case GET:
								_Server_Logger_(S_CMD_GET);

								NetIO.What_Behavior=GET_FILE;	// Set cause.
								_GET_FILE_(&Thread_Resource,New_Chat);	// Process this cmd.

								break;
																
							case UP:
								_Server_Logger_(S_CMD_UP);
								NetIO.What_Behavior=UP_FILE;
								_UP_FILE_(&Thread_Resource,New_Chat);	// Process this cmd.

								break;
			
							case LOGOUT:
								goto UserLogout;	// User want to quit.
						
							default:
								syslog(LOG(LOG_ERR),"FTPR_Server: None defined command.");
						}

						// Switch end.
					}
					else
						break;	// EOF/error.

				} while (true && Server_Should_Not_To_Stop);

				// User logout.

				UserLogout:
					TSC->_SHUTDOWN_(New_Chat,SHUT_RDWR);

			}
			else
			{	
				// Failed to accept a connection.
				syslog(LOG(LOG_NOTICE),"FTPR_Server: Failed to accept a connection %s:%hu",TSC->_INET_NTOP_(AF_INET,&Client_Address,Address_String,16),TSC->_NTOHS_(Client_Address.sin_port));
			}
			// Accept.	

		}	// While


		_WorkUp_Quit:

			delete[] UserCmd;	// Recycle resource.
	}

	void FTPR_Server_class::_Server_Shutdown_(void)
	{
		// The destroy working will be responing by their clear method.

		if (FIDC)
		{
			delete FIDC;
			syslog(LOG(LOG_NOTICE),"FTPR_Server: Delete File-Directory obj.");
		}
		else;
		if (PTC)
		{
			delete PTC;
			syslog(LOG(LOG_NOTICE),"FTPR_Server: Delete Posix Thread obj.");
		}
		else;
		if (TSC)
		{
			syslog(LOG(LOG_NOTICE),"FTPR_Server: Delete Socket obj.");
			delete TSC;
		}
		else;
		if (SYSS)
		{
			delete SYSS;
			syslog(LOG(LOG_NOTICE),"FTRP_Server: Delete Signal obj.");
		}
		else;

		syslog(LOG(LOG_NOTICE),"FTPR_Server: Server shutdown.");

	}

	void FTPR_Server_class::_LS_(const int Peer_Sock,char *LS_Buffer)
	{
		do
		{
			FIDC->_TRAVERSE_(LS_Buffer);
			if (*LS_Buffer != '\0')
				TSC->_WRITE_SOCK_(Peer_Sock,LS_Buffer,sizeof(LS_Buffer));
			else;
		} while(true);
	}

	void FTPR_Server_class::_CD_(const int Peer_Sock,const char *New_Dir)
	{
		// Peer_Sock for tell peer result.

		if (FIDC->_CHDIR_(New_Dir))
		{
			TSC->_WRITE_SOCK_(Peer_Sock,"Changed directory.",18);
			_Server_Logger_(S_CD_T);
		}
		else
		{
			TSC->_WRITE_SOCK_(Peer_Sock,"Failed to change directory.",27);
			_Server_Logger_(S_CD_F);
		}

	}

	bool FTPR_Server_class::_GET_FILE_(FGU *T_Resource,int & Client_Socket)
	{
		bool _GET_FILE_Status_(true);	// Suppose it's true.
		char Msg[64];	// This array use to save messages to send client.
		short int **Get_Thread_Msg(NULL);
		struct stat TempFile_Info=FIDC->_LSTAT_((const char *)The_CMD_Hit.CMDP);
		pthread_t PT;		// Posix thread.

		/*	Variables zone.	*/

		// Check the file whether have existed.
		if (0 == TempFile_Info.st_size)
		{
			// There file had not existed.
			_Server_Logger_(S_FILE_NOTEX);
			_GET_FILE_Status_=false;
			_Copy_String_(Msg,"File not existed.",17);
			goto _GET_FILE_QUIT;
		}
		else
		{

			File_Info.UsePort=DownUp_Port;		// Set port for file download.
			File_Info.FileSize=TempFile_Info.st_size;	// Set file size.
			_Copy_String_(File_Info.FILENAME,(const char *)The_CMD_Hit.CMDP,strlen((const char *)The_CMD_Hit.CMDP));	// Set file name.
		
		}

		// Now ready to file transfer -- 
		// Open file :
		
		if (FIDC->_OPEN_(File_Info.FILENAME,O_RDONLY) < 0)
		{
			// Open file failed.
			// Tell client the case.

			_Copy_String_(Msg,"Server open file failed.",24);
			_GET_FILE_Status_=false;
			_Server_Logger_(S_OPEN_FILE_F);	// Record log.

			goto _GET_FILE_QUIT;
		}
		else
		{
			// Ready to create thread for transfer file.

			PTC->_LOCK_MUTEX_();	// Lock thread.
			if (PTC->_CREATE_THREAD_(&PT,NULL,Thread_FF,T_Resource) != 0)
			{
				// Create thread fail.
				_GET_FILE_Status_=false;
				_Server_Logger_(S_CRE_PT_F);	// Record.
				_Copy_String_(Msg,"Server can not open downloader.",31);
			}
			else
			{
				// Read don't need to get file lock.

				PTC->_UNLOCK_MUTEX_();	// Release lock to thread.
				
				// Tell client file information.
				TSC->_WRITE_SOCK_(Client_Socket,"#OK#",4);	// Notice client to receive file information from socket.
				TSC->_WRITE_SOCK_(Client_Socket,&File_Info,sizeof(File_Info));

				PTC->_LOCK_MUTEX_();
				PTC->_JOIN_THREAD_(PT,(void **)Get_Thread_Msg);	// Wait thread.
				FIDC->_CLOSE_(FIDC->_Get_Latest_Open_File_());	// Close file.
			
				if (**Get_Thread_Msg != FF_GET_SUSS)
				{
					_GET_FILE_Status_=false;
					_Copy_String_(Msg,"In the time of transfer had detected error.",43);
					_FTPR_Logger_(**Get_Thread_Msg);	// FTPR logger.
				//	_Server_Logger_(**Get_Thread_Msg); There call FTPR'logger.
				}
				else
					_Copy_String_(Msg,"Succeed to transfer file.",25);

			}

			PTC->_UNLOCK_MUTEX_();	// Unlock.	
		}

		_GET_FILE_QUIT:

			TSC->_WRITE_SOCK_(Client_Socket,Msg,strlen(Msg));	// Tell client the case.
			return _GET_FILE_Status_;
	}

	bool FTPR_Server_class::_UP_FILE_(FGU *T_Resource,int & Client_Socket)
	{
		// UP FILE,server must receive file information from client by socket.
		// Server send '#UPINFO#' to notice client send information.

		bool _UP_FILE_Status_(true);	// Suppose true to finished.
		char Msg[64];			// Save messages to send client.
		char TempFile_Name[]="FTPRStmp.XXXXXX";	// Temp file name arrary.
		int TempFileDes(-1);		// TempFile descriptor.
		short int **Get_Thread_Msg(NULL);	// Get thread exit state.
		struct flock File_Lock={
			.l_type=F_WRLCK,
			.l_whence=SEEK_SET,
			.l_start=0,
			.l_len=0,
			};	// File lock. Get lock for all bytes of file.
		pthread_t PT;	// Posix thread,
		struct stat TempFile_Info=FIDC->_LSTAT_((const char*)The_CMD_Hit.CMDP);	// Try to get file info.

		/*	Variables zone.	*/
		
		// Check file whether existed.
		if (TempFile_Info.st_size > 0)	// File had existed.
		{
			_UP_FILE_Status_=false;
			_Copy_String_(Msg,"File had existed.",17);
			goto _UP_FILE_QUIT;
		}
		else
		{
			// Allow client to send file.
			// Try to open tempfile,and then get file information from client.
			TempFileDes=FIDC->_MKSTEMP_(TempFile_Name);

			if (TempFileDes < 0)
			{
				// Failed to create tempfile.
				_UP_FILE_Status_=false;
				_Copy_String_(Msg,"Server can not create tmpfile.",30);	// Set message.
				_Server_Logger_(S_FILE_HADEX);
				goto _UP_FILE_QUIT;
			}
			else
			{
				// Succeed to create tmpfile,now try to get file information.
				TSC->_WRITE_SOCK_(Client_Socket,"#UPFILE#",8);	// Request file info.
				TSC->_POLL_(MAIN_SOCKET);	// Network select.
				// Check whether have event come.
				if (TSC->_Check_Main_(POLLRDNORM))
				{
					if (TSC->_READ_SOCK_(Client_Socket,&File_Info,sizeof(File_Info)) <= 0)	// Read info.
					{
						// Read info error.
						_Copy_String_(Msg,"Server read file info error.",28);
						_UP_FILE_Status_=false;
						_Server_Logger_(S_GETF_INFO_F);
						goto _UP_FILE_QUIT;
					}
					else
					{
						File_Info.UsePort=DownUp_Port;	// Set thread port.
					
						// Try to set file length.

						if (FIDC->_FTRUNCATE_(TempFileDes,File_Info.FileSize) < 0)
						{
							// Reset file length failed.
							_Copy_String_(Msg,"Server can not reset file length.",33);
							_UP_FILE_Status_=false;
							_Server_Logger_(S_SETF_LEN_F);
							goto _UP_FILE_QUIT;
						}
						else;
					}
				}
				else
				{
					// poll failed.
					_Copy_String_(Msg,"Server can not receive file info.",33);
					_UP_FILE_Status_=false;
					_Server_Logger_(S_REVF_INFO_F);
					goto _UP_FILE_QUIT;
				}
			}
		}
		
		// Up to here,server had been succeed to receive file info.
		
		// Get file lock.
		if (FIDC->_LOCK_FILE_(TempFileDes,F_GETLK,&File_Lock) == -1)
		{
			// Failed to get file lock.
			_UP_FILE_Status_=false;
			_Copy_String_(Msg,"Server cannot lock up file.",27);	// Tell client.
			_Server_Logger_(S_GETF_LOCK_F);	// Record.
			goto _UP_FILE_QUIT;
		}
		else;	// Succeed to get file lock.
	
		// Open thread.

		PTC->_LOCK_MUTEX_();	// Lock thread.
		if (PTC->_CREATE_THREAD_(&PT,NULL,Thread_FF,T_Resource) != 0)
		{
			// Failed to create thread.
			_UP_FILE_Status_=false;
			_Copy_String_(Msg,"Server cannot open uploader.",28);
			_Server_Logger_(S_CRE_PT_F);
		}
		else
		{
			PTC->_UNLOCK_MUTEX_();	// Release lock.
			TSC->_WRITE_SOCK_(Client_Socket,"#OK#",4);	// Notice client receive file info.
			TSC->_WRITE_SOCK_(Client_Socket,&File_Info,sizeof(File_Info));	// Send info.
			PTC->_LOCK_MUTEX_();	// Lock thread.
		
			// Wait thread.
			PTC->_JOIN_THREAD_(PT,(void **)Get_Thread_Msg);
			// Check
			if (**Get_Thread_Msg != FF_UP_SUSS)
			{
				_UP_FILE_Status_=false;
				_Copy_String_(Msg,"In the time of received file had detected error.",48);
				_FTPR_Logger_(**Get_Thread_Msg);	// FTPR logger.
			//	_Server_Logger_(**Get_Thread_Msg);	There will invoke FTPR'logger.
			}
			else
				if (FIDC->_LINK_(TempFile_Name,File_Info.FILENAME) < 0)	// Failed to make hardware link.
				{
					_UP_FILE_Status_=false;
					_Copy_String_(Msg,"Server can not save file.",25);
					_Server_Logger_(S_LNK_FILE_F);	// Record.
				}
				else
					_Copy_String_(Msg,"Succeed to received file.",25);
		}

		PTC->_UNLOCK_MUTEX_();	// Release mutex.

		// Unlock file.

		File_Lock.l_type=F_UNLCK;	// Unlock.
		(void)FIDC->_LOCK_FILE_(TempFileDes,F_SETLK,&File_Lock);

		_UP_FILE_QUIT:

			FIDC->_CLOSE_(TempFileDes);	// Close temp file.
			TSC->_WRITE_SOCK_(Client_Socket,Msg,strlen(Msg));	// Tell client what happened.
			return _UP_FILE_Status_;
	}


	// This function respones to record logs of WorkUp function.
	void FTPR_Server_class::_Server_Logger_(const short int Log_Code)
	{
		char LogMsg[64+1]="\0";	// Local arrary.

		// Full \0 to the array.

		__asm__ (
			// Use RCX RAX RBX register.
			"pushq	%%rcx\n\t"
			"pushq	%%rax\n\t"
			"pushq	%%rbx\n\t"
			"movq	%0,%%rbx\n\t"
			"movq	$0x0,%%rax\n\t"
			"movq	$0x41,%%rcx\n\t"
			"ClearLogMsg:\n\t"
			"movb	$0x00,(%%rbx,%%rax,0x01)\n\t"
			"loop	ClearLogMsg\n\t"
			"popq	%%rbx\n\t"
			"popq	%%rax\n\t"
			"popq	%%rcx\n\t"
			:
			: "r"(LogMsg)
			: "%rax","%rcx"
			);


		switch (Log_Code)
		{
			// Read settings.

			case S_RS_GET_MEM_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to get mem for RS.",38);
				break;

			case S_RS_SHARE_SET_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to set share settings.",42);
				break;

			case S_RS_OPEN_CONFIG_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to open config file.",40);
				break;

			case S_RS_SET_OVER:
				_Copy_String_(LogMsg,"FTPR_Server: Succeed to complete server.",40);
				break;

			case S_RS_SET_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Detected error in RS.",34);
				break;

			// Init feature classes.

			case S_INIT_FC_SUSS:
				_Copy_String_(LogMsg,"FTPR_Server: Succeed to init feature classes.",45);
				break;

			case S_INIT_FID_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to init File-Dir feature.",45);
				break;

			case S_INIT_CRE_FID_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to create File-Dir obj.",43);
				break;

			case S_INIT_THREAD_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to init Posix-Thread.",41);
				break;

			case S_INIT_CRE_THREAD_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to create Posix-Thread obj.",47);
				break;

			case S_INIT_SOCK_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to init Socket.",35);
				break;

			case S_INIT_CRE_SOCK_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to create socket obj.",41);
				break;

			case S_INIT_SIGNAL_ERR:	
				_Copy_String_(LogMsg,"FTPR_Server: Failed to init signal.",35);
				break;

			case S_INIT_CRE_SIGNAL_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to create signal obj.",41);
				break;

			// Init signal.

			case S_SIGALRM_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to set sigaction for sigalrm.",49);
				break;

			case S_SIGHUP_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to set sigaction for sighup.",48);
				break;

			case S_SIGINT_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to set sigaction for sigint.",48);
				break;

			case S_SIGTERM_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to set sigaction for sigterm.",49);
				break;
	
			// Responer.

			// LS

			case S_CMD_LS:
				_Copy_String_(LogMsg,"FTPR_Server: Receive cmd : ls.",30);
				break;

			case S_LS_T:
				_Copy_String_(LogMsg,"FTPR_Server: List directory items.",34);
				break;

			// CD
		
			case S_CMD_CD:	
				_Copy_String_(LogMsg,"FTPR_Server: Receive cmd : cd.",30);
				break;
	
			case S_CD_T:	
				_Copy_String_(LogMsg,"FTPR_Server: Changed directory.",31);
				break;

			case S_CD_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to change directory.",40);
				break;

			// GET FILE.
			
			case S_CMD_GET:
				_Copy_String_(LogMsg,"FTPR_Server: Receive cmd : get.",31);
				break;

			case S_FILE_NOTEX:
				_Copy_String_(LogMsg,"FTPR_Server: File have not exist in down.",41);
				break;

			case S_OPEN_FILE_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to open file in down.",41);
				break;
			
			// UP FILE.			
		
			case S_CMD_UP:
				_Copy_String_(LogMsg,"FTPR_Server: Receive cmd : up.",30);
				break;

			case S_FILE_HADEX:
				_Copy_String_(LogMsg,"FTPR_Server: File had existed in up.",36);
				break;

			case S_GETF_INFO_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to get file info in up.",43);
				break;

			case S_SETF_LEN_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to set file length in up.",45);
				break;

			case S_REVF_INFO_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to receive file info in up.",47);
				break;

			case S_GETF_LOCK_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to get file lock in up.",43);
				break;

			case S_LNK_FILE_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to make hard link for file in up.",53);
				break;

			case S_CRE_PT_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to start posix thread.",42);
				break;
			
			default:	// Default to do nothing,because that code does not be defined.
				_Copy_String_(LogMsg,"FTPR_Server: Error code had not defined.",40);
		}

		syslog(LOG(LOG_ERR),LogMsg);	// Write to log.

	}


}
