//	TheFTPR_Thread_Lib.cpp
//	Version : 0.1.1
//	Date : Thu Jun 18 20:20:29 2020
//	Last revise : Thu Jun 18 20:20:29 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		For simply update for thread of FTPR,make thread function and timer function as independed library.
//	Functions :
//		void * Thread_Timer(void *Arg);		// Timer.
//		void * Thread_FF(void *Arg);		// Work thread function.
//	Header :
//		Use this lib must contain "FTPR.h"
//
//	Fix :
//		1> Set Tcp_IO as a pointer for as quickly as parameter passing.
//		2> TempFile will create by caller,so is _GET_FILE_ or _UP_FILE_.Thread does not respones doing that.
//		3> Checking Identify to switch work mode.And client have not to open timer.
//		4> Made get,up as functions.
//


#include"FTPR.h"

namespace FTPR{

	// There define some functions them are not own class.
	// Because this timer as detach model to start up,so it must record logs by itself.	
	void *Thread_Timer(void *Arg)
	{

		// At first,declare a TMC pointer.
		TMC *Timer_TMC=(TMC *)Arg;	// For simply to use struct.

		if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL) < 0)	// Set timer could be cancel.
		{
			// This situation is means that the cancel state invalid.
			syslog(LOG(LOG_ERR),"FTPR_Timer: Set cancel enable failed.");
			pthread_exit(NULL);
		}
		else
			if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL) < 0)
			{
				// Set cancel type failed.
				syslog(LOG(LOG_ERR),"FTPR_Timer: Set cancel asynchronous failed.");
				pthread_exit(NULL);
			}
			else;

		syslog(LOG(LOG_NOTICE),"FTPR_Timer: Timing up.");

		for (unsigned short int Timing(0); Timing < Timer_TMC->Timeout_Value; ++Timing)
			sleep(1);

		syslog(LOG(LOG_NOTICE),"FTPR_Timer: Timeout!");
		
		if (Timer_TMC->Timer_Thread->_CANCEL_THREAD_(Timer_TMC->Cancel_TID) < 0)
			syslog(LOG(LOG_ERR),"FTPR_Timer: Cancel thread was failed!");
		else
			syslog(LOG(LOG_NOTICE),"FTPR_Timer: Cancel request send.");

		pthread_exit(NULL);

	}

	// Error logger will be recorded by _FTPR_Logger_.
	// File descriptor and socket env all would be setting in by caller 'main thread'.
	void *Thread_FF(void *Arg)
	{

		// Because translater would alloc memory for variables left at them be using.
		// So could not initialize them.

		short int FF_Code(0);	// Error code.
		pthread_t Timer_TID;	// This variable save timer' TID.
		unsigned short int Retry;	// This variable only used by client.
		short int Invoke_Result(-1);	// Function invoke result.
		// At first,declare a FGU pointer for simply to use structure.
		FGU *FF_FGU((FGU *)Arg);
		// Timer parameters.
		TMC Timer_Argument;	// Timer structure.

		struct sockaddr_in Addr;	// Save peer address.
		socklen_t Addr_Size(sizeof(struct sockaddr_in));	// Structure size.
		char Address_Str[16];		// xxx.xxx.xxx.xxx

		int New_Socket(-1);	// Temp socket.

		const size_t Buff_Size(FF_FGU->Net_io->Network_File_IO_Buffer);	// Buffer length to alloc memorr.
		// Get file buffer.
		char *FileIOB=new char[Buff_Size];	// FBS. In heap.
		if (NULL == FileIOB)
		{
			FF_Code=FF_GET_IO_BUFF_ERR;
			goto _Thread_FF_QUIT_;	// To end.
		}
		else;

		// Set thread cleanup handler.
		pthread_cleanup_push(_Thread_FF_Cleanup_,(void *)FileIOB);
		pthread_cleanup_pop(0);
		

		//	Env


		// These localtion variables will save in stack frame.
		// Try to get mutex.
		FF_FGU->Thread->_LOCK_MUTEX_(); // lock.

		if (FF_FGU->Work_id == SERVER_ID)	// Server must set cancel type,because it will open timer.
		{
			// Server use this structure to save peer info.
			Addr_Size=sizeof(struct sockaddr_in);
			memset(&Addr,'\0',Addr_Size);

			// Set timer'argument structure.
			Timer_Argument.Cancel_TID=FF_FGU->Thread->_GET_ID_SELF_();
			Timer_Argument.Timeout_Value=FF_FGU->Timer_Timeout;
			Timer_Argument.Timer_Thread=FF_FGU->Thread;

			if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL) < 0)	// Set cancel env.
			{
				FF_Code=FF_SET_CANCEL_STATE_ERR;	// Failed state set.
				FF_FGU->Thread->_UNLOCK_MUTEX_();	// Unlock.
				goto _Thread_FF_QUIT_;	
			}
			else
				if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL) < 0)
				{
					FF_Code=FF_SET_CANCEL_TYPE_ERR;
					FF_FGU->Thread->_UNLOCK_MUTEX_();	// Unlock.
					goto _Thread_FF_QUIT_;
				}
				else;
		}
		else;


		// Try to bind socket DU.

		switch (FF_FGU->Work_id)
		{
			/*	Server bind socket for client come to connect.	*/
			case SERVER_ID:
				if (FF_FGU->Tcp_sock->_BIND_(THREAD_SOCKET) < 0)
				{
					FF_Code=FF_BIND_SOCKET_ERR;
					FF_FGU->Thread->_UNLOCK_MUTEX_();	// Unlock.
					goto _Thread_FF_QUIT_;
				}
				else
					if (FF_FGU->Tcp_sock->_LISTEN_(THREAD_SOCKET,1) < 0)
					{
						FF_Code=FF_OPEN_LISTEN_ERR;
						FF_FGU->Thread->_UNLOCK_MUTEX_();	// Unlock.
						goto _Thread_FF_QUIT_;
					}
					else;
				break;
			
			/*	Client try to connect server,have retry limit.	*/
			case CLIENT_ID:

				New_Socket=FF_FGU->Tcp_sock->_GET_SOCKET_(THREAD_SOCKET);
				// Client have to link with server,if failed will retry until limit.
				for (Retry=FF_FGU->Net_io->Retry_Link; Retry > 0; --Retry)
					if ((Invoke_Result=FF_FGU->Tcp_sock->_CONNECT_(THREAD_SOCKET)) < 0)
					{
						FF_FGU->Tcp_sock->_RESET_SOCKETS_(THREAD_SOCKET);	// Reset socket when connect invoking failed.
						continue;
					}
					else
						break;

				if (Invoke_Result == 0)	// Check result.
					;
				else
				{
					// Failed to make connection.
					FF_Code=FF_CONNECT_SOCKET_ERR;
					FF_FGU->Thread->_UNLOCK_MUTEX_();
					goto _Thread_FF_QUIT_;
				}

				break;
			default:
				FF_Code=FF_ID_ERR;
				FF_FGU->Thread->_UNLOCK_MUTEX_();
				goto _Thread_FF_QUIT_;
		}
		
		FF_FGU->Thread->_UNLOCK_MUTEX_();	// Unlock.
			
		// Open timer.	Client have not to open it.
		if (FF_FGU->Work_id == SERVER_ID && FF_FGU->Thread->_CREATE_THREAD_(&Timer_TID,&FF_FGU->Thread->Thread_Attr,Thread_Timer,&Timer_Argument) < 0)
		{
			FF_Code=FF_OPEN_TIMER_ERR;	// Open timer failed.
			goto _Thread_FF_QUIT_;
		}
		else;

		if (FF_FGU->Work_id == SERVER_ID)
			if ((New_Socket=FF_FGU->Tcp_sock->_ACCEPT_(THREAD_SOCKET,(struct sockaddr *)&Addr,&Addr_Size)) < 0)	// Wait a connect.
			{
					// Bad socket.
					FF_Code=FF_ACCEPT_BAD;
					(void)FF_FGU->Thread->_CANCEL_THREAD_(Timer_TID);
					goto _Thread_FF_QUIT_;	
			}
			else;
		else;

		// Up to now,had made a connection with client.

		// Cancel timer.
		if (FF_FGU->Work_id == SERVER_ID && FF_FGU->Thread->_CANCEL_THREAD_(Timer_TID) < 0)
		{	
			// Failed cancel timer.
			FF_Code=FF_CANCEL_TIMER_ERR;
			goto _Thread_FF_QUIT_;
		}
		else;


		// Because work function has not check buffer whether is null,so caller must prevent had not sent a null pointer to it.

		switch (FF_FGU->Work_id)
		{

			// Server record link info.
			case SERVER_ID:
				// Write a record to log.
				syslog(LOG(LOG_NOTICE),"FTPR_FF: Connected by %s ."
					,FF_FGU->Tcp_sock->_INET_NTOP_(Addr.sin_family,&Addr.sin_addr,Address_Str,16));
				// Check which behavior GET or UP.
				// Adding new feature,when in the stage 'UP_FILE',sever call 'poll' to wait data.

				switch (FF_FGU->Net_io->What_Behavior)
				{

					case GET_FILE:		// Client request to get a file.
						Invoke_Result=_Thread_FF_Up_(FF_FGU,FileIOB,FF_FGU->Net_io->Network_File_IO_Buffer,New_Socket);
						FF_Code=(Invoke_Result == -1)?FF_GET_ERR:FF_GET_SUSS;
						
						break;

					case UP_FILE:		// Client request to up a file.
						Invoke_Result=_Thread_FF_Get_(FF_FGU,FileIOB,FF_FGU->Net_io->Network_File_IO_Buffer,New_Socket);
						FF_Code=(Invoke_Result == -1)?FF_UP_ERR:FF_UP_SUSS;
						break;
					default:
						// Flag does not defined.
						FF_Code=FF_BEHAVIOR_ERR;	// Error code.
				}	
				break;

			case CLIENT_ID:

				/*	Client'work mode is in contrast with server.	*/

				switch (FF_FGU->Net_io->What_Behavior)
				{
					case GET_FILE:

						// Download file from server.
						Invoke_Result=_Thread_FF_Get_(FF_FGU,FileIOB,FF_FGU->Net_io->Network_File_IO_Buffer,New_Socket);
						FF_Code=(Invoke_Result == -1)?FF_GET_ERR:FF_GET_SUSS;
						break;

					case UP_FILE:
						// Upload file to server.
						Invoke_Result=_Thread_FF_Up_(FF_FGU,FileIOB,FF_FGU->Net_io->Network_File_IO_Buffer,New_Socket);
						FF_Code=(Invoke_Result == -1)?FF_UP_ERR:FF_UP_SUSS;
						break;
					default:
						FF_Code=FF_BEHAVIOR_ERR;	// Behavior had not defined.
				}
				break;

			default:
				FF_Code=FF_ID_ERR;
		}
	
		//	Primary

		_Thread_FF_QUIT_:


			if (FF_FGU->Work_id == SERVER_ID)
				FF_FGU->Tcp_sock->_SHUTDOWN_(New_Socket,SHUT_RDWR);	// Close socket.
			else;

			/*	Server process this code will close the socket which had bind.	*/
			/*	Client process this code will close the link had connected.	*/
//			FF_FGU->Tcp_sock->_SHUTDOWN_(FF_FGU->Tcp_sock->_GET_SOCKET_(THREAD_SOCKET),SHUT_RDWR);
			// File descriptor will be closing by caller.			
		
			#ifdef DEBUG

			syslog(LOG(LOG_ERR),"Thread: FF_Code is %hd",FF_Code);

			#endif

			pthread_exit((void *)FF_Code);	// quit thread.	


			// While pthread_exit is invoke,cleanup handler will be processing.	

		//	Return

	}

	short int _Thread_FF_Get_(FGU * & Tool,char *Buff,const size_t Buff_Len,const int TmpSocket)
	{
		// Read data and write to temp file.


		ssize_t Last_Read(0);	// Record the latest read.
		int TmpFile(Tool->Fid->_Get_TempFile_());	// Get temp file.
		short int Return_Value(0);	// Return value.
		unsigned short int Retry(Tool->Net_io->Retry_Number);
		struct pollfd OldPOLL(Tool->Tcp_sock->_GET_POLLFD_(THREAD_SOCKET));	// Before function return this data will be restoring.
		struct pollfd NewPOLL={
				.fd=TmpSocket,
				.events=POLLRDNORM,
			};	// In this function.
		
		//	Env

		Tool->Tcp_sock->_SET_POLLFD_(THREAD_SOCKET,NewPOLL);	// Set new poll.
		
		/*	Cycle.	*/
		while (Tool->Tcp_sock->_POLL_(THREAD_SOCKET) != -1 && Retry > 0)
		{
			if (Tool->Tcp_sock->_Check_Thread_(POLLRDNORM))
			{
				Retry=Tool->Net_io->Retry_Number;	// Reset.
				if ((Last_Read=Tool->Fid->_READ_FILE_(TmpSocket,Buff,Buff_Len)) > 0)
					if (Tool->Fid->_WRITE_FILE_(TmpFile,Buff,Last_Read) < 0)
						Return_Value=-1;
					else;
				else
					break;
			}
			else if (Tool->Tcp_sock->_Check_Thread_(POLLRDHUP))
			{
				Return_Value=0;
				break;
			}
			else
				--Retry;
		}
		
		//	Primary

		Tool->Tcp_sock->_SET_POLLFD_(THREAD_SOCKET,OldPOLL);	// Reset.
		return Return_Value;	// Return result. 0 on success,-1 on error.

		//	Return
	}


	short int _Thread_FF_Up_(FGU * & Tool,char *Buff,const size_t Buff_Len,const int TmpSocket)
	{
		// Read data and write to socket.

		ssize_t Last_Read(0);	// Latest read.
		int DataFile(Tool->Fid->_Get_Latest_Open_File_());	// Get latest opened.
		short int Return_Value(0);
		
		//	Env
	
		while ((Last_Read=Tool->Fid->_READ_FILE_(DataFile,Buff,Buff_Len)) > 0)
			if (Tool->Tcp_sock->_WRITE_SOCK_(TmpSocket,Buff,Last_Read) < 0)
				Return_Value=-1;
			else;

		//	Primary
	

		return Return_Value;	// Result. 0 on success,-1 on error.
		//	Return
	}

	void _Thread_FF_Cleanup_(void *Arg)
	{
		// Destroy memory.
		
		if (Arg)
			delete[] (char *)Arg;
		else;
	}


}
