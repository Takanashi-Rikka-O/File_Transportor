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
		if (Setting_Buffer)
		{
			_Read_Setting_Code=R_GET_MEM_ERR;	
			return _Read_Setting_Code;
		}
		else;
		
		// Temporary string position pointer.
		char *Temp_Char_Pointer(NULL);

		// Temporary value for get number.
		int Temporary_Value(0);

		Lable:
	

		// FTPR_Basic had been defined setting read function for shared settings.
		// When it succeed to read,return true.
		if (FTPR_Basic::_Read_Shared_Setting_(Setting_File))
			;
		else
		{
			
			_Read_Setting_Code=R_SHARE_SET_ERR;
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
			_Read_Setting_Code=R_OPEN_CONFIG_ERR;
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
					DUPORT=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer)) > 0)?Temporary_Value:Default_DUPORT;
				else if (strncmp(Setting_Buffer,"CWAITS",6) == 0)
					CWAITS=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer) > 0))?Temporary_Value:Default_CWAITS;
				else if (strncmp(Setting_Buffer,"CPORT",5) == 0)
					CPORT=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer) > 0))?Temporary_Value:Default_CPORT;
				else if (strncmp(Setting_Buffer,"RPATH",5) == 0)			
				{
					Temp_Char_Pointer=strstr(Setting_Buffer,"=");
					if (Temp_Char_Pointer)
						strncpy(RPATH,Temp_Char_Pointer+1,128);
					else
						strncpy(RPATH,Default_RPATH,128);

				}
				else;
			}

		}

		// Check it is normaly finished.
		if (Read.eof())
			_Read_Setting_Code=R_SET_OVER;
		else
			_Read_Setting_Code=R_SET_ERR;
		
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
		short int Init_Code(Init_FC_SUSS);	
	
		// Set umask.
		umask(Init_Set->SHS->Make_File_Mask);

		// Create FID.
		FIDC=new FID_class(Init_Set->Root_Path);
		// Check status.
		if (FIDC)
			if (FIDC->State_Of_Initialization_FID)
				;
			else
			{
				Init_Code=Init_FID_ERR;
				delete FIDC;
				return Init_Code;
			}
		else
		{
			Init_Code=Init_CRE_FID_ERR;
			return Init_Code;
		}

		// Create PTC.
		PTC=new THREAD_class;
		// Check status.
		if (PTC)
			if (PTC->State_Of_Initialization_THREAD)
				;
			else
			{
				Init_Code=Init_THREAD_ERR;
				delete PTC;
				delete FIDC;
				return Init_Code;
			}
		else
		{
			Init_Code=Init_CRE_THREAD_ERR;
			delete FIDC;
			return Init_Code;
		}

		// Create TSC.
		TSC=new TCP_SOCK_class(Init_Set->CPT,Init_Set->DUP,Init_Set->SHS->Network_IO_Timeout,Init_Set->SHS->Network_IO_Timeout);
		// Check status.
		if (TSC)
			if (TSC->State_Of_Initialization_SOCK)
				;
			else
			{
				Init_Code=Init_SOCK_ERR;
				delete PTC;
				delete FIDC;
				delete TSC;

				return Init_Code;
			}
		else
		{
			Init_Code=Init_CRE_SOCK_ERR;
			delete PTC;
			delete FIDC;

			return Init_Code;
		}

		// Create
		SYSS=new SYSTEM_SIGNAL_class;
		// Check status.
		if (SYSS)
			if (SYSS->State_Of_Initialization_SIGNAL)
				;	
			else
			{
				Init_Code=Init_SIGNAL_ERR;
				delete PTC;
				delete FIDC;
				delete TSC;
				delete SYSS;

				return Init_Code;
			}
		else
		{
			Init_Code=Init_CRE_SIGNAL_ERR;
			delete PTC;
			delete FIDC;
			delete TSC;

			return Init_Code;
		}

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
		short int Return_Function(0);	// For record return value.
		Server_Set Server_Set_Config={.SHS=&Share_Set};

		// At first,read setting.

		Return_Function=_Read_Setting_(CONFIG_FILE);
		// Check status.

		switch (Return_Function)
		{
			case R_GET_MEM_ERR:	// Retry.
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to get memory for setting buffer.Retry.");
				goto R_RETRY;

			case R_SET_ERR:		// Reading setting detected a none defined error. Retry.
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to open config file.Retry.");
				goto R_RETRY;

			case R_OPEN_CONFIG_ERR:	// Retry.
				syslog(LOG(LOG_ERR),"FTPR_Server: Detected a error in reading setting.Retry.");
				goto R_RETRY;

			case R_SHARE_SET_ERR:	// Retry.
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to reading shared setting.Retry.");
				goto R_RETRY;
						
			case R_SET_OVER:	// None error.
				syslog(LOG(LOG_INFO),"FTPR_Server: Succeed to read setting.");
				break;

			default:
				R_RETRY:
					
					if (_Read_Shared_Setting_(CONFIG_FILE) == R_SET_OVER)
						;
					else
					{
						
						// Failed again.
						syslog(LOG(LOG_ERR),"FTPR_Server: Retry to read setting failed.");
						return false;
					}
					break;
		}

		// Full Server_Ser.
		Server_Set_Config.DUP=DUPORT;
		Server_Set_Config.CPT=CPORT;
		Server_Set_Config.Root_Path=RPATH;
		
		// At second,initialize variables.
		// These will be setting in read setting stage.

		// At third,set root tree.
		// At fourth,change work home.
		// They are need class FID.

		// At fiveth,set signal action.
		// Must create class as before.

		// At sixth,init feature classes.
		Return_Function=_Init_FC_((const void *)&Server_Set_Config);
		switch (Return_Function)
		{
			case Init_FC_SUSS:
				syslog(LOG(LOG_NOTICE),"FTPR_Server: Succeed to init all fc.");
				break;
			case Init_FID_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to initialize File controllor.");
				goto INIT_RETRY;

			case Init_CRE_FID_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to create File controllor.");
				goto INIT_RETRY;

			case Init_THREAD_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to initialize Thread controllor.");
				goto INIT_RETRY;

			case Init_CRE_THREAD_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to create Thread controllor.");
				goto INIT_RETRY;

			case Init_SOCK_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to initialize Socket controllor.");
				goto INIT_RETRY;

			case Init_CRE_SOCK_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to create Socket controllor.");
				goto INIT_RETRY;

			case Init_SIGNAL_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to intialize Signal controllor.");
				goto INIT_RETRY;

			case Init_CRE_SIGNAL_ERR:
				syslog(LOG(LOG_ERR),"FTPR_Server: Failed to create Signal controllor.");
			
			default:

				INIT_RETRY:

				// Retry.
				if (PTC)
					delete PTC;
				else;
	
				if (FIDC)
					delete FIDC;
				else;

				if (TSC)
					delete TSC;
				else;

				if (SYSS)
					delete SYSS;
				else;

				syslog(LOG(LOG_NOTICE),"FTPR_Server: Retry to init feature controllor.");

				Return_Function=_Init_FC_((const void *)&Server_Set_Config);
				if (Return_Function == Init_FC_SUSS)
					syslog(LOG(LOG_INFO),"FTPR_Server: Retry succeed.");
				else
				{
					syslog(LOG(LOG_ERR),"FTPR_Server: Retry failed.");
					return false;
				}

		}

		return true;

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
				throw "FTPR_Server: Faild to get msg buffer,retry.";
		}
		catch(const char *Error_Str)
		{
			syslog(LOG(LOG_NOTICE),Error_Str);
			UserCmd=new char[Share_Set.Message_Buff_Size];
			if (UserCmd)
				;
			else
			{
				syslog(LOG(LOG_ERR),"FTPR_Server: Retry to get msg buffer fault.");
				return;
			}

		}

		// Return value.

		short int **Result(NULL);	// This pointer for 'pthread_exit' return.

		
		// For accept.
		int New_Chat(-1);

		// Thread_FF arguments.
		NETIOINFO NetIO;	// This structure have some tcp setting parameters.
		FGU Thread_Resource;	// This structure will send to Thread_FF.

		// For file stat.
		struct stat Temp_File_Info;

		// For peer address.
		struct sockaddr_in Client_Address;
		socklen_t Address_Length(1);
		char Address_String[16];	// For string.

		// Bind socket.
		if (TSC->_BIND_(MAIN_SOCKET) < 0)
		{
			syslog(LOG(LOG_ERR),"FTPR_Server: Can not bind communication socket.");
			goto _WorkUp_Quit;
		}
		else
			if (TSC->_LISTEN_(MAIN_SOCKET,1) < 0)
			{
				syslog(LOG(LOG_ERR),"FTPR_Server: Can not open listen queue for communication socket.");
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
								// Cycle to write data.

								_LS_(New_Chat,UserCmd);

								break;

							case CD:
		
								_CD_(New_Chat,(const char *)The_CMD_Hit.CMDP);
								break;

							case GET:
							{
								Temp_File_Info=FIDC->_LSTAT_((const char *)The_CMD_Hit.CMDP);
								if (Temp_File_Info.st_size == 0) // Check file if existed.
								{


									syslog(LOG(LOG_ERR),"FTPR_Server: Client require file - %s is possible not be existed(or have error).",The_CMD_Hit.CMDP);
									TSC->_WRITE_SOCK_(New_Chat,"The file probably had not been existed.",39);

									break;
								}
								else;

								// GET_UP Structure.This structure will send to client.
								strncpy(File_Info.FILENAME,(const char *)The_CMD_Hit.CMDP,128);	// Copy file name.
								File_Info.UsePort=DUPORT;		// Port will use.
								File_Info.FileSize=Temp_File_Info.st_size;


								if (FIDC->_OPEN_((const char *)The_CMD_Hit.CMDP,O_RDONLY) < 0) // Try to open file with read only.
								{
										// Failed to open file.
									syslog(LOG(LOG_ERR),"FTPR_Server: Server failed to open file - %s.",The_CMD_Hit.CMDP);
									TSC->_WRITE_SOCK_(New_Chat,"Failed to open file.",20);

									break;	
								}
								else;
								
								// Ready to transfer file.

								// Set NETIOINFO.
								NetIO.Network_File_IO_Buffer=Share_Set.File_Buff_Size;
								NetIO.What_Behavior=GET_FILE;
								NetIO.Retry_Number=Share_Set.Retry_Net_IO;

								// Set FGU.
								Thread_Resource.Tcp_Sock=TSC;	
								Thread_Resource.Fid=FIDC;

								Thread_Resource.IO_Thread=PTC;	
								Thread_Resource.Timer_Timeout=CWAITS;
								Thread_Resource.Tcp_IO=NetIO;

								pthread_t Down_Thread;

								// Open thread.	
								PTC->_LOCK_MUTEX_();	// Get lock.

								if (PTC->_CREATE_THREAD_(&Down_Thread,NULL,FTPR::Thread_FF,&Thread_Resource) != 0)
								{
									PTC->_UNLOCK_MUTEX_();	// Prevent dead lock.
									syslog(LOG(LOG_ERR),"FTPR_Server: Can not open thread.");
									TSC->_WRITE_SOCK_(New_Chat,"Can not start thread.",21);
								}
								else
								{
									PTC->_UNLOCK_MUTEX_();	// Unlock .
									// On there,client must sleep 3s for synchronize.

									TSC->_WRITE_SOCK_(New_Chat,"#OK#",4);	// Tell client.
									TSC->_WRITE_SOCK_(New_Chat,&File_Info,sizeof(File_Info));
									PTC->_LOCK_MUTEX_();
									PTC->_JOIN_THREAD_(Down_Thread,(void **)Result);
									PTC->_UNLOCK_MUTEX_();

									// Check thread status.
									switch (**Result)
									{

										case FF_SET_CANCEL_STATE_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot enable cancel.");
											break;

										case FF_SET_CANCEL_TYPE_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot set cancel.");
											break;

										case FF_BIND_SOCKET_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot bind file transfer socket.");
												break;

										case FF_OPEN_LISTEN_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot open listen queue.");
											break;

										case FF_OPEN_TIMER_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot open timer.");
											break;

										case FF_GET_IO_BUFF_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot get file buffer.");
											break;

										case FF_CANCEL_TIMER_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot cancel timer.");
											break;

										case FF_GET_SUSS:
											syslog(LOG(LOG_NOTICE),"FTPR_Server: Thread succeed to send a file.");
											TSC->_WRITE_SOCK_(New_Chat,"Transfer file succeed.",22);
											break;

										case FF_GET_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread detected error in downfile.");
											TSC->_WRITE_SOCK_(New_Chat,"Transfer file failed.",21);
											break;
											break;
 
										case FF_BEHAVIOR_ERR:
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread don't know what the behavior is.");
											break;

										case FF_ACCEPT_BAD:	
											syslog(LOG(LOG_ERR),"FTPR_Server: Thread accept bad.");
											break;

										default:
											;
									}
								}

								FIDC->_CLOSE_(FIDC->_Get_Latest_Open_File_());	// Close file.

								break;

							}
									
							case UP:
							{
								// Must check this file whether had existed.

								Temp_File_Info=FIDC->_LSTAT_((const char *)The_CMD_Hit.CMDP);
								if (Temp_File_Info.st_size > 0)	// File existed.
								{
									syslog(LOG(LOG_ERR),"FTPR_Server: The file required to upload had been existed - %s.",The_CMD_Hit.CMDP);
									TSC->_WRITE_SOCK_(New_Chat,"File had exited.",16);
									break;
								}
								else;

								// Server must get file information at front.
								TSC->_WRITE_SOCK_(New_Chat,"#UPINFO#",8);
								
								TSC->_POLL_(MAIN_SOCKET);	// Poll IO.

								if (! TSC->_Check_Main_(POLLRDNORM))
								{
									syslog(LOG(LOG_ERR),"FTPR_Server: Can not receive up file info from client.");
									TSC->_WRITE_SOCK_(New_Chat,"Failed to get file info.",24);
									break;
								}
								else
									if (TSC->_READ_SOCK_(New_Chat,&File_Info,sizeof(GET_UP)) != sizeof(GET_UP))
									{
										syslog(LOG(LOG_ERR),"FTPR_Server: Error file info received.");
										TSC->_WRITE_SOCK_(New_Chat,"Error file info.",16);
										break;
									}
									else;
								
								
								// Try create a tempfile.

								char TempFile_Name[]="FTPR_Server.XXXXXX";	// Tempfile name.
								int TempFile_Des=FIDC->_MKSTEMP_(TempFile_Name); // Get tempfile.

								struct flock LOCKFILE={			// File lock.
										.l_type=F_WRLCK,
										.l_whence=SEEK_SET,	//	|
										.l_start=0,		// Lock all.
										.l_len=0,		//	|
									};
									
								if (TempFile_Des > 0)
								{
									// Ftruncate.

									if (FIDC->_FTRUNCATE_(TempFile_Des,File_Info.FileSize) < 0)
									{
										// Failed to change file length.
										syslog(LOG(LOG_ERR),"FTPR_Server: Server can not change file length.");
										TSC->_WRITE_SOCK_(New_Chat,"Failed to change file size.",27);
										FIDC->_CLOSE_(TempFile_Des);	// Close temp file.
														// This will make file deleted.
										break;
									}
									else;
																				
									// Get file lock.
									if (FIDC->_LOCK_FILE_(FIDC->_Get_Latest_Open_File_(),F_GETLK,&LOCKFILE) != -1)
									{

										File_Info.UsePort=DUPORT;	// File transfer port.

										// Set NETIOINFO.
										NetIO.Network_File_IO_Buffer=Share_Set.File_Buff_Size;
										NetIO.What_Behavior=UP_FILE;
										NetIO.Retry_Number=Share_Set.Retry_Net_IO;

										// Set FGU.
										Thread_Resource.Tcp_Sock=TSC;
										Thread_Resource.Fid=FIDC;
										Thread_Resource.IO_Thread=PTC;
										Thread_Resource.Timer_Timeout=CWAITS;
										Thread_Resource.Tcp_IO=NetIO;

										pthread_t Up_Thread;	// For new thread.

										PTC->_LOCK_MUTEX_();	// Get lock.

										if (PTC->_CREATE_THREAD_(&Up_Thread,NULL,FTPR::Thread_FF,&Thread_Resource) != 0)
										{
											// Failed to open file.

											PTC->_UNLOCK_MUTEX_();	// Prevent dead lock.

											syslog(LOG(LOG_ERR),"FTPR_Server: Can not open thread in upload.");
											TSC->_WRITE_SOCK_(New_Chat,"Can not start thread.",21);
											FIDC->_LOCK_FILE_(TempFile_Des,F_UNLCK,&LOCKFILE);
						
										}
										else
										{			
											PTC->_UNLOCK_MUTEX_();
											TSC->_WRITE_SOCK_(New_Chat,"#OK#",4);
											PTC->_LOCK_MUTEX_();
											PTC->_JOIN_THREAD_(Up_Thread,(void **)Result);
											PTC->_UNLOCK_MUTEX_();

											FIDC->_LOCK_FILE_(TempFile_Des,F_UNLCK,&LOCKFILE);

											// Check status.
											switch (**Result)
											{
												case FF_UP_SUSS:	// Succeed.
													FIDC->_LINK_(TempFile_Name,(const char *)The_CMD_Hit.CMDP);	
													syslog(LOG(LOG_NOTICE),"FTPR_Server: Succeed to received a file.");
													TSC->_WRITE_SOCK_(New_Chat,"Success for upload.",19);

													break;

												case FF_SET_CANCEL_STATE_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot enable cancel.");
													break;

												case FF_SET_CANCEL_TYPE_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot set cancel.");
													break;

												case FF_BIND_SOCKET_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot bind file transfer socket.");
													break;

												case FF_OPEN_LISTEN_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot open listen queue.");
													break;

												case FF_OPEN_TIMER_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot open timer.");
													break;

												case FF_GET_IO_BUFF_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot get file buffer.");
													break;

												case FF_CANCEL_TIMER_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread cannot cancel timer.");
													break;

												case FF_UP_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread detected error in upfile.");
													TSC->_WRITE_SOCK_(New_Chat,"Receive file failed.",20);
													
													break;
 
												case FF_BEHAVIOR_ERR:
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread don't know what the behavior is.");
													break;

												case FF_ACCEPT_BAD:	
													syslog(LOG(LOG_ERR),"FTPR_Server: Thread accept bad.");
													break;
										
												default:
													;
											}
										
								


										}

									}
									else
									{
										syslog(LOG(LOG_ERR),"FTPR_Server: Can not get file lock - %s.",TempFile_Name);
										TSC->_WRITE_SOCK_(New_Chat,"Can not get file lock.",22);
									}

									FIDC->_CLOSE_(TempFile_Des);	// Close file
								}
								else
								{

									syslog(LOG(LOG_ERR),"FTPR_Server: Can not create tempfile.");
									TSC->_WRITE_SOCK_(New_Chat,"Can not create tempfile.",24);
								}

							}

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
			delete FIDC;
		else;
		if (PTC)
			delete PTC;
		else;
		if (TSC)
			delete TSC;
		else;
		if (SYSS)
			delete SYSS;
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
			TSC->_WRITE_SOCK_(Peer_Sock,"Changed directory.",18);
		else
			TSC->_WRITE_SOCK_(Peer_Sock,"Failed to change directory.",27);

	}


}
