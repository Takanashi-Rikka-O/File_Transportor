//	TheFTPR_Thread_Lib.cpp
//	Version : 0.1
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
	void *Thread_FF(void *Arg)
	{
		short int FF_Code(FF_NO_PROBLEM);	// Error code.

		// At first,declare a FGU pointer for simply to use structure.
		FGU *FF_FGU((FGU *)Arg);

		// Try to get mutex.
		FF_FGU->IO_Thread->_LOCK_MUTEX_(); // Block.

		// Set cancel env.
		if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL) < 0)
		{
			FF_Code=FF_SET_CANCEL_STATE_ERR;	// Failed state set.
			FF_FGU->IO_Thread->_UNLOCK_MUTEX_();	// Unlock.
			pthread_exit(&FF_Code);
			
		}
		else
			if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL) < 0)
			{
				FF_Code=FF_SET_CANCEL_TYPE_ERR;
				FF_FGU->IO_Thread->_UNLOCK_MUTEX_();	// Unlock.
				pthread_exit(&FF_Code);
			}
			else;


		// Try to bind socket DU.

		if (FF_FGU->Tcp_Sock->_BIND_(THREAD_SOCKET) < 0)
		{
			FF_Code=FF_BIND_SOCKET_ERR;
			FF_FGU->IO_Thread->_UNLOCK_MUTEX_();	// Unlock.
			pthread_exit(&FF_Code);
		}
		else
			if (FF_FGU->Tcp_Sock->_LISTEN_(THREAD_SOCKET,1) < 0)
			{
				FF_Code=FF_OPEN_LISTEN_ERR;
				FF_FGU->IO_Thread->_UNLOCK_MUTEX_();	// Unlock.
				pthread_exit(&FF_Code);
			}
			else;
		
		FF_FGU->IO_Thread->_UNLOCK_MUTEX_();	// Unlock.
		
		// Open timer.
		
		pthread_t Timer_TID;	// This variable save timer' TID.
		TMC Timer_Argument={
				.Cancel_TID=FF_FGU->IO_Thread->_GET_ID_SELF_(),
				.Timeout_Value=FF_FGU->Timer_Timeout,	
				.Timer_Thread=FF_FGU->IO_Thread,	
			};	// Timer parameter.


	
		// Open timer.	
		if (FF_FGU->IO_Thread->_CREATE_THREAD_(&Timer_TID,&FF_FGU->IO_Thread->Thread_Attr,Thread_Timer,&Timer_Argument) < 0)
		{
			FF_Code=FF_OPEN_TIMER_ERR;
			pthread_exit(&FF_Code);
		}
		else;

		// File transport cycle entrance.

		struct sockaddr_in Peer_Addr;	// Save peer address.
		socklen_t Addr_Size;		// Structure size.
		char Address_Str[16];		// xxx.xxx.xxx.xxx
		// These localtion variables will save in stack frame.


		// Get file buffer.
		char *FileIOB=new char[FF_FGU->Tcp_IO->Network_File_IO_Buffer];	// FBS. In heap.

		// Check pointer.
		if (NULL != FileIOB)
		{
			FF_Code=FF_GET_IO_BUFF_ERR;					// Error code.
			(void)FF_FGU->IO_Thread->_CANCEL_THREAD_(Timer_TID);		// Try to close timer.
			pthread_exit(&FF_Code);
		}
		else;

		// Wait a connect.
		int New_Socket=FF_FGU->Tcp_Sock->_ACCEPT_(THREAD_SOCKET,(struct sockaddr *)&Peer_Addr,&Addr_Size);

		// Up to now,had made a connection with client.

		// Cancel timer.
		if (FF_FGU->IO_Thread->_CANCEL_THREAD_(Timer_TID) < 0)
		{	
			delete[] FileIOB;	// Delete buffer.

			// Failed cancel timer.
			FF_Code=FF_CANCEL_TIMER_ERR;
			pthread_exit(&FF_Code);
		}
		else
			syslog(LOG(LOG_NOTICE),"FTPR_FF: Shutdown timer.");


		if (New_Socket > 0)
		{
			// Write a record to log.
			syslog(LOG(LOG_NOTICE),"FTPR_FF: Connected by %s .",FF_FGU->Tcp_Sock->_INET_NTOP_(Peer_Addr.sin_family,&Peer_Addr.sin_addr,Address_Str,16));
			// Check which behavior GET or UP.


			// Adding new feature,when in the stage 'UP_FILE',sever call 'poll' to wait data.

			switch (FF_FGU->Tcp_IO->What_Behavior)
			{

				case GET_FILE:		// Client request to get a file.

				{

					int Temp_Get_File=FF_FGU->Fid->_Get_Latest_Open_File_();	
					// Main thread have to open file and check file open status at before.

					ssize_t Once_Read_File=0;	// Record size had readed at onece.

					while ((Once_Read_File=FF_FGU->Fid->_READ_FILE_(Temp_Get_File,FileIOB,FF_FGU->Tcp_IO->Network_File_IO_Buffer) > 0))
			 		{
						// Write to peer.
						if (FF_FGU->Tcp_Sock->_WRITE_SOCK_(New_Socket,FileIOB,Once_Read_File) < 0)
							FF_Code=-1;	// For check whether have error for file transfer.
						else;
					}

					// Up to now,all file had been sent,close this connection.
					// Main thread respones to open file or trunc file,this thread will close file after send over.

					// Work down.

					FF_Code=(FF_Code == -1)?FF_GET_ERR:FF_GET_SUSS;	// Succeed send.

					break;
	
				}

				case UP_FILE:		// Client request to up a file.

				{	

					int Temp_Up_File=FF_FGU->Fid->_Get_TempFile_();
					// Main thread have to open file and check file open status at before.

					ssize_t Once_Read_Net=0;	// Record size had readed at onece.

					// Now need not to set poll timeout,because TCP_SOCK_class will set it when it has create.
					//FF_FGU->Tcp_Sock->_POLL_SET_(THREAD_SOCKET,POLLRDNORM,FF_FGU->Tcp_IO.Network_IO_Timeout_Value);	// Set pollfd.
					unsigned short int Retry(FF_FGU->Tcp_IO->Retry_Number);	// Get network io retry number limit.

					while (FF_FGU->Tcp_Sock->_POLL_(THREAD_SOCKET) != -1 && Retry > 0)
					{
						if (FF_FGU->Tcp_Sock->_Check_Thread_(POLLRDNORM))	// TCP could read.
						{
							// Reset retry limit.
							Retry=FF_FGU->Tcp_IO->Retry_Number;
							// Network io.
							
							Once_Read_Net=FF_FGU->Tcp_Sock->_READ_SOCK_(New_Socket,FileIOB,FF_FGU->Tcp_IO->Network_File_IO_Buffer);
							if (FF_FGU->Fid->_WRITE_FILE_(Temp_Up_File,FileIOB,Once_Read_Net) < 0)
								FF_Code=-1;
							else;
						}
						else
							--Retry;
					}

					// Up to now,all file had been received,close this connection.
					// Main thread respones to open file or trunc file,this thread will close file after send over.

					// Work down.
					FF_Code=(FF_Code == -1)?FF_UP_ERR:FF_UP_SUSS;	// Succeed receive.

			
					break;

				}

				default:

					// Flag does not defined.

					FF_Code=FF_BEHAVIOR_ERR;	// Error code.
			}


	
			delete[] FileIOB;	// Recycle memory.
			FF_FGU->Tcp_Sock->_SHUTDOWN_(New_Socket,SHUT_RDWR);		// Close socket.
	
			// File descriptor will be closing by caller.			

			pthread_exit(&FF_Code);	// quit thread.		
	

		}
		else
		{
			delete[] FileIOB;	// Delete buffer.
			// Bad socket.
			FF_Code=FF_ACCEPT_BAD;
			pthread_exit(&FF_Code);
		}

	}


}
