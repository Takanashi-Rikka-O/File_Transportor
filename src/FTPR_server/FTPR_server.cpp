//	FTPR_server.cpp
//	Version : 0.2.1
//	Date : Fri Jun 12 10:31:41 2020
//	Last revise : Fri Sep 18 09:51:? 2020
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
//		4> Fix the TCP class failed to init.
//		5> After server had finished 'up file' successfully,it will remake linked-list.
//		6> Adding 'unlink' for tempfile,because the file created via 'mkstemp' would not automatically to be deleted.


#include"FTPR_server.h"

namespace FTPR_SERVER{


// Define signal action.

	void _Server_Signal_Sigterm_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGTERM received from %u - terminate",Receive_Info->si_signo,Receive_Info->si_pid);

		// How to stop server,a simple method is declare a global variable.
		// And in the code,only this function to changed it.
		// Server finished a working would check this variable,if it is flase,stop.

		Server_Should_Not_To_Stop=false;	// Server should to stop.

	}

	void _Server_Signal_Sigint_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGINT received from %u - terminate",Receive_Info->si_signo,Receive_Info->si_pid);
	
		union sigval INFO={.sival_int=1};	// union.
		sigqueue(getpid(),SIGTERM,INFO);	// Send SIGTERM to itself.
	}

	void _Server_Signal_Sighup_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGHUP received from %u - ignores",Receive_Info->si_signo,Receive_Info->si_pid);
	}

	void _Server_Signal_Sigalrm_(int Sig,siginfo_t *Receive_Info,void *Content)
	{
		syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal %d SIGALRM received from %u - ignores",Receive_Info->si_signo,Receive_Info->si_pid);
	}


	
	short int FTPR_Server_class::_Init_FC_(const void *SETTINGS)
	{
		// For simply use the parameter,defined a structure pointer.
		const Server_Set *Init_Set=(const Server_Set *)SETTINGS;
	
		// Error code return variable.
		short int Init_Code(-1);	
	
		// Set umask.
		umask(Init_Set->SHS->Make_File_Mask);

		// And then create class objects and check init status.




		// After create feature classes.
		// 1> PTC.
		PTC=new THREAD_class;
		if (PTC)
			if (PTC->State_Of_Initialization_THREAD)
				;
			else
			{
				Init_Code=S_INIT_THREAD_ERR;	// Failed to init.
				goto INIT_QUIT;
			}
		else
		{
			Init_Code=S_INIT_CRE_THREAD_ERR;
			goto INIT_QUIT;
		}

		// 2> TSC.
		TSC=new TCP_SOCK_class(Init_Set->CPT,Init_Set->DUP,Init_Set->SHS->Network_IO_Timeout,Init_Set->SHS->Network_IO_Timeout);
		if (TSC)
		{
			if (TSC->State_Of_Initialization_SOCK)
				;
			else
			{
				Init_Code=S_INIT_SOCK_ERR;
				goto INIT_QUIT;
			}
		}
		else
		{
			Init_Code=S_INIT_CRE_SOCK_ERR;
			goto INIT_QUIT;
		}

		// 3> SYSS.
		SYSS=new SYSTEM_SIGNAL_class;
		if (SYSS)
			if (SYSS->State_Of_Initialization_SIGNAL)
				;
			else
			{
				Init_Code=S_INIT_SIGNAL_ERR;
				goto INIT_QUIT;
			}
		else
		{
			Init_Code=S_INIT_CRE_SIGNAL_ERR;
			goto INIT_QUIT;
		}

		// 4> FIDC.
		FIDC=new FID_class(Init_Set->Root_Path);	// Try to make a FID class.
		if (FIDC)
			if (FIDC->State_Of_Initialization_FID)	// Check if sure to be initialized.
				;
			else
			{
				Init_Code=S_INIT_FID_ERR;	// Failed to init.
				goto INIT_QUIT;
			}
		else
		{
			Init_Code=S_INIT_CRE_FID_ERR;		// Failed to create.
			goto INIT_QUIT;
		}


		Init_Code=S_INIT_FC_SUSS;	// Success.We want it process to there and no any error.


		INIT_QUIT:

			return Init_Code;
		
	}


	void FTPR_Server_class::_Command_Parsing_(const char *Command)
	{
		// Command server to respones :
		// 	ls,cd,logout,GET,UP.
		// This function will scan command string and full structure WCH in FTPR_Basic.

		// For improve speed,this function will use two character to guess command.

		The_CMD_Hit.CMDN=255; 	// None defined command.
		The_CMD_Hit.CMDP=NULL;

		if (Command)
		{
			if (strncmp(Command,"ls",2) == 0)
				The_CMD_Hit.CMDN=LS;
			else if (strncmp(Command,"cd",2) == 0)
				The_CMD_Hit.CMDN=CD,The_CMD_Hit.CMDP=Command+3;
			else if (strncmp(Command,"lo",2) == 0)
				The_CMD_Hit.CMDN=LOGOUT;
			else if (strncmp(Command,"ge",2) == 0)	
				The_CMD_Hit.CMDN=GET,The_CMD_Hit.CMDP=Command+4;
			else if (strncmp(Command,"up",2) == 0)
				The_CMD_Hit.CMDN=UP,The_CMD_Hit.CMDP=Command+3;
			else;
		}
		else;	// NULL pointer also like none command defintion.

	}


	bool FTPR_Server_class::_Server_Init_(void)
	{

		_Server_Logger_(S_ENTRY_INIT);	// Entrance record.

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

		syslog(LOG(LOG_ERR),"FTPR_Server: Set - DUP : %hu , CPT : %hu , RP : %s .",DownUp_Port,Comm_Port,Root_Path);
		
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
			// Set signal block set.
			SYSS->_SIGDELSET_(BLOCK_SET,SIGTERM);
			SYSS->_SIGDELSET_(BLOCK_SET,SIGINT);
			SYSS->_SIGDELSET_(BLOCK_SET,SIGALRM);
			SYSS->_SIGDELSET_(BLOCK_SET,SIGHUP);
		
			if (SYSS->_SIGPROCMASK_(SIG_SETMASK,NULL) < 0)
			{
				Return_Function=S_SETSIGBLK_ERR;
				goto _Server_Init_QUIT_;
			}
			else;
		
			// SIGINT will send SIGTERM to itself,so must after SIGINT capturer returned,the SIGTERM be recevied.		

			// Set signal action. 0->T,-1->F.
			if (SYSS->_SIGACTION_(SIGALRM,_Server_Signal_Sigalrm_) < 0)
				Return_Function=S_SIGALRM_ERR,_Server_Init_Status_=false;
			else
				if (SYSS->_SIGACTION_(SIGHUP,_Server_Signal_Sighup_) < 0)
					Return_Function=S_SIGHUP_ERR,_Server_Init_Status_=false;
				else
				{
					SYSS->_SIGADDSET_(NORMAL_SET,SIGTERM);	// While SIGINT capturer work,block the SIGTERM until it returned.
					if (SYSS->_SIGACTION_(SIGINT,_Server_Signal_Sigint_) < 0)
						Return_Function=S_SIGINT_ERR,_Server_Init_Status_=false;
					else
					{
						SYSS->_SIGDELSET_(NORMAL_SET,SIGTERM);
						SYSS->_SIGADDSET_(NORMAL_SET,SIGINT);	// While SIGTERM capturer work,block SIGINT until it returned.

						if (SYSS->_SIGACTION_(SIGTERM,_Server_Signal_Sigterm_) < 0)
							Return_Function=S_SIGTERM_ERR,_Server_Init_Status_=false;
						else;
					}
				}
		}
		else
			_Server_Init_Status_=false;

	_Server_Init_QUIT_:

		_Server_Logger_(Return_Function);

		syslog(LOG(LOG_NOTICE),"FTPR_Server: Finished init.");

		return _Server_Init_Status_;

	}



	void FTPR_Server_class::_LS_(const int Peer_Sock,char *LS_Buffer)
	{
		
		do
		{
			FIDC->_TRAVERSE_(LS_Buffer);
			if (*LS_Buffer != '\0')
				TSC->_WRITE_SOCK_(Peer_Sock,LS_Buffer,strlen(LS_Buffer)+1);
			else
				break;	// Next reading mut return head iterm,so break cycle.
		} while(true);

		TSC->_WRITE_SOCK_(Peer_Sock,"#EOF#",6);	// End.

	}

	void FTPR_Server_class::_CD_(const int Peer_Sock,const char *New_Dir)
	{
		// Peer_Sock for tell peer result.

		syslog(LOG(LOG_NOTICE),"FTPR_Server: New directory target - %s ",New_Dir);

		if (FIDC->_CHDIR_(New_Dir))
		{
			write(Peer_Sock,"Changed directory.",18+1);
			_Server_Logger_(S_CD_T);
		}
		else
		{
			write(Peer_Sock,"Failed to change directory.",27+1);
			_Server_Logger_(S_CD_F);
		}

	}

	// This function will open thread,so it must set all env for thread in the front.
	void FTPR_Server_class::_GET_FILE_(FGU *T_Resource,int & Client_Socket)
	{
		char Msg[64];	// This array use to save messages to send client.
		short int *Get_Thread_Msg(NULL);
		struct stat TempFile_Info=FIDC->_LSTAT_((const char *)The_CMD_Hit.CMDP);	// Get target file information.
		pthread_t PT;		// Posix thread.

		/*	Variables zone.	*/

		// Check the file whether have existed.
		if (0 == TempFile_Info.st_size)
		{
			// There file had not existed.
			_Server_Logger_(S_FILE_NOTEX);
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
				_Server_Logger_(S_CRE_PT_F);	// Record.
				_Copy_String_(Msg,"Server can not open downloader.",31);
			}
			else
			{
				// Read don't need to get file lock.

				PTC->_UNLOCK_MUTEX_();	// Release lock to thread.
				
				// Tell client file information.
				TSC->_WRITE_SOCK_(Client_Socket,"#OK#",4);	// Notice client to receive file information from socket.
				sleep(1);
				TSC->_WRITE_SOCK_(Client_Socket,&File_Info,sizeof(File_Info));

				PTC->_LOCK_MUTEX_();
				PTC->_JOIN_THREAD_(PT,(void **)&Get_Thread_Msg);	// Wait thread.
				FIDC->_CLOSE_(FIDC->_Get_Latest_Open_File_());	// Close file.
			
				if ((long int)Get_Thread_Msg != FF_GET_SUSS)
					_FTPR_Logger_((long int)Get_Thread_Msg);	// FTPR logger.
				else;

			}

			PTC->_UNLOCK_MUTEX_();	// Unlock.	

			return;
		}

		_GET_FILE_QUIT:

			TSC->_WRITE_SOCK_(Client_Socket,Msg,strlen(Msg));	// Tell client the case.
	}

	void FTPR_Server_class::_UP_FILE_(FGU *T_Resource,int & Client_Socket)
	{
		// UP FILE,server must receive file information from client by socket.
		// Server send '#UPINFO#' to notice client send information.

		char Msg[64];			// Save messages to send client.
		char TempFile_Name[]="FTPRStmp.XXXXXX";	// Temp file name arrary.
		int TempFileDes(-1);		// TempFile descriptor.
		short int *Get_Thread_Msg(NULL);	// Get thread exit state.
		struct stat Temp_File_Stat;	// For temp file stat which have just received from client.
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
				_Copy_String_(Msg,"Server can not create tmpfile.",30);	// Set message.
				_Server_Logger_(S_FILE_HADEX);
				goto _UP_FILE_QUIT;
			}
			else
			{
				// Succeed to create tmpfile,now try to get file information.
				TSC->_WRITE_SOCK_(Client_Socket,"#UPINFO#",8);	// Request file info.
				TSC->_POLL_(MAIN_SOCKET);	// Network select.
				// Check whether have event come.
				if (TSC->_Check_Main_(POLLRDNORM))
				{
					if (TSC->_READ_SOCK_(Client_Socket,&File_Info,sizeof(GET_UP)) != sizeof(GET_UP))	// Read info.
					{
						// Read info error.
						_Copy_String_(Msg,"Server read file info error.",28);
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
			_Copy_String_(Msg,"Server cannot open uploader.",28);
			_Server_Logger_(S_CRE_PT_F);
		}
		else
		{
			PTC->_UNLOCK_MUTEX_();	// Release lock.
			TSC->_WRITE_SOCK_(Client_Socket,"#OK#",4);	// Notice client receive file info.
			sleep(1);
			TSC->_WRITE_SOCK_(Client_Socket,&File_Info,sizeof(File_Info));	// Send info.
			PTC->_LOCK_MUTEX_();	// Lock thread.
		
			// Wait thread.
			PTC->_JOIN_THREAD_(PT,(void **)&Get_Thread_Msg);
			// Check
			if ((long int)Get_Thread_Msg != FF_UP_SUSS)
				_FTPR_Logger_((long int)Get_Thread_Msg);	// FTPR logger.
			else
				if (fstat(TempFileDes,&Temp_File_Stat) < 0)
					_Server_Logger_(S_TMP_FILE_STAT_F);
				else
					if (Temp_File_Stat.st_size == File_Info.FileSize)
						if (FIDC->_LINK_(TempFile_Name,File_Info.FILENAME) < 0)	// Failed to make hardware link.
							_Server_Logger_(S_LNK_FILE_F);	// Record.
						else
							FIDC->_REMAKE_LIST_();	// Because have new file had link to present directory.
					else
						_Server_Logger_(S_TMP_FILE_SIZE_F);	// Size error.
		}

		PTC->_UNLOCK_MUTEX_();	// Release mutex.

		// Unlock file.

		File_Lock.l_type=F_UNLCK;	// Unlock.
		(void)FIDC->_LOCK_FILE_(TempFileDes,F_SETLK,&File_Lock);
		FIDC->_UNLINK_(TempFile_Name);	/* Because 'mkstemp' create tmpfile would not automatically to be deleted. */

		_UP_FILE_QUIT:

			FIDC->_CLOSE_(TempFileDes);	// Close temp file.
			TSC->_WRITE_SOCK_(Client_Socket,Msg,strlen(Msg));
	}

	void FTPR_Server_class::_Server_Shutdown_(void)
	{
		if (PTC)
		{
			delete PTC;
			PTC=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Server: Posix thread object had destroied.");
		}
		else;

		if (TSC)
		{
			delete TSC;
			TSC=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Server: Socket object had destroied.");
		}
		else;

		if (SYSS)
		{
			delete SYSS;
			SYSS=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Server: Signal object had destroied.");
		}
		else;

		if (FIDC)
		{
			delete FIDC;
			FIDC=NULL;
			syslog(LOG(LOG_NOTICE),"FTPR_Server: File&Directory object had destroied.");
		}
		else;

		syslog(LOG(LOG_NOTICE),"FTPR_Server: Objects destroying over.");
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

			case S_SETSIGBLK_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to set server'signal block set.",51);
				break;

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

			case S_FAIL_MEM:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to get memmory of user command buffer.",58);
				break;

			case S_BIND_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to bind socket.",35);
				break;
			
			case S_LISTEN_ERR:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to open listen queue.",41);
				break;

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
			
			case S_TMP_FILE_STAT_F:
				_Copy_String_(LogMsg,"FTPR_Server: Failed to get tempfile'information.",48);
				break;

			case S_TMP_FILE_SIZE_F:
				_Copy_String_(LogMsg,"FTPR_Server: Temp file size not as same with target file.",57);
				break;

			// Entrance

			case S_ENTRY_INIT:
				_Copy_String_(LogMsg,"FTPR_Server: Reached target server init.",40);
				break;

			case S_ENTRY_WORK:
				_Copy_String_(LogMsg,"FTPR_Server: Reached target server work.",40);
				break;

			case S_ENTRY_DOWN:
				_Copy_String_(LogMsg,"FTPR_Server: Reached target server down.",40);
				break;

			case S_ENTRY_SERVER:
				_Copy_String_(LogMsg,"FTPR_Server: Reached target server start.",41);
				break;

			case S_ENTRY_WORK_ACCEPT:
				_Copy_String_(LogMsg,"FTPR_Server: Server wait connection...",38);
				break;
			
			default:	// Default to do nothing,because that code does not be defined.
				_Copy_String_(LogMsg,"FTPR_Server: Error code had not defined.",40);
		}

		syslog(LOG(LOG_ERR),LogMsg);	// Write to log.

	}


}
