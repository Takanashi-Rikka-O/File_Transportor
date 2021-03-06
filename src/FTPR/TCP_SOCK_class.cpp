//	TCP_SOCK_class.cpp
//	Version : 0.1.2
//	Date : Sat May 23 15:12:36 2020
//	Last revise : Sat Jul 18 21:19:? 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Defintion of Class TCP_SOCK_Class.
//	Functions :
//		Socket functions.
//	Header :
//		"TCP_SOCK_class.h"
//
//	Fix :
//		1> Change method for get host information when processing in server.
//		2> Adding new build method for client.
//		3> Adding interfaces for change data-members.
//		4> New interface _RESET_SOCKETS_ for reset sockets.
//		5> Shutdown will invoke close to set off the file descriptor for that number can reuse.


#include"TCP_SOCK_class.h"


namespace SOCKET{

	//	Build method is the default.

	// [0]->Main,[1]->Thread.
	

	TCP_SOCK_class::TCP_SOCK_class():SOCKETS(NULL),ADDRINFO(NULL)	// The default build method.
	{
		/*	Clear memory.	*/
		memset(Host_Name,'\0',128);
		memset(&ADDR_M,'\0',sizeof(struct sockaddr_in)),ADDR_T=ADDR_M;
		memset(&Main,'\0',sizeof(struct pollfd)),Thread=Main;
		/*	Set default time.	*/
		Wait_IO_Time_Main=Wait_IO_Time_DownUp=5000;

		State_Of_Initialization_SOCK=true;	// Suppose it is succeed.

		/*	Get memory and create sockets.	*/
		SOCKETS=new int[1][2];	//	16B
		if (NULL == SOCKETS)
		{
			State_Of_Initialization_SOCK=false;
			syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not allocs memory.");	
			return;
		}
		else;

		// Sockets.
		**SOCKETS=_SOCKET_(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		*(*SOCKETS+1)=_SOCKET_(AF_INET,SOCK_STREAM,IPPROTO_TCP);

		// Check.
		if (**SOCKETS < 0 || *(*SOCKETS+1) < 0)
		{
			State_Of_Initialization_SOCK=false;
			syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not create sockets.");
		}
		else;

	}

	TCP_SOCK_class::TCP_SOCK_class(unsigned short int Comm_Port,unsigned short int FT_Port,unsigned short int Main_Timing,unsigned short int Thread_Timing)
	{
		//	Suppose the state was succeed.
		State_Of_Initialization_SOCK=true;


		/*	Initialize.	*/
		ADDRINFO=NULL;
		SOCKETS=NULL;
		memset(&ADDR_M,'\0',sizeof(struct sockaddr_in));
		memset(&ADDR_T,'\0',sizeof(struct sockaddr_in));
		memset(&Main,'\0',sizeof(struct pollfd));
		memset(&Thread,'\0',sizeof(struct pollfd));
		memset(Host_Name,'\0',sizeof(Host_Name));

		// Open option.
		int Switch(1);	// For socket option.

		// Initialize the main and thread structure.
		Main.fd=Thread.fd=-1;
		Main.events=Thread.events=POLLNVAL;	// Invalid event.

		// Socket poll timeout.
		Wait_IO_Time_Main=Main_Timing,Wait_IO_Time_DownUp=Thread_Timing;

		// Control to get address infomation.
		struct addrinfo Control={
				.ai_flags=0,		// Default.
				.ai_family=AF_INET,	// Internet address.
				.ai_socktype=SOCK_RAW,	// Normaly socket.
				.ai_protocol=IPPROTO_IP, // IP.
				.ai_addrlen=0,
				.ai_addr=NULL,
				.ai_canonname=NULL,
				.ai_next=NULL
			};

		int Returninfo(0);


		if (_GETHOSTNAME_(Host_Name,127) < 0)	// hostname.
		{
			//	On error.
			syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not get hostname. Host Name : %s .",Host_Name);	//	Get host name.
			State_Of_Initialization_SOCK=false;
		}
		else
		{
			if ((Returninfo=getaddrinfo(Host_Name,NULL,&Control,&ADDRINFO)) == 0)	//	Get host info.
			{
				syslog(LOG(LOG_NOTICE),"FTPR_SOCK: Host : %s  IP : %s Port1 : %hu Port2 : %hu ."
					,Host_Name
					,inet_ntoa(((struct sockaddr_in *)ADDRINFO->ai_addr)->sin_addr)
					,Comm_Port
					,FT_Port
					);

				//	Set server address in IPv4.

				ADDR_M.sin_family=ADDRINFO->ai_family;	//	Host ip family.
				ADDR_M.sin_addr=((struct sockaddr_in *)ADDRINFO->ai_addr)->sin_addr;	//	Currently host IPv4 address.
		
				// Use same family and address.
				ADDR_T=ADDR_M;	// Structure assignement.But port had not assigned.

				// Assign ports.
				ADDR_M.sin_port=_HTONS_(Comm_Port);
				ADDR_T.sin_port=_HTONS_(FT_Port);


				SOCKETS=new int[1][2];		//	Get memory.	
				if (SOCKETS)
				{

					**SOCKETS=_SOCKET_(AF_INET,SOCK_STREAM,IPPROTO_TCP);	// Try to open socket.
					if (**SOCKETS < 0)
					{
						// Open socket failed.
						// Open failed for socket main.
						syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not open socket for communication."); 
						State_Of_Initialization_SOCK=false;
					}
					else
					{
						*(*SOCKETS+1)=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	// Try to open socket.
						if (*(*SOCKETS+1) < 0)				// Open failed for socket thread.
						{
							// Open socket failed.
							syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not open socket for file transfer.");
							State_Of_Initialization_SOCK=false;
						}
						else
						{
							// Socket reuse immediately.
							if (_SETSOCKOPT_(THREAD_SOCKET,SOL_SOCKET,SO_REUSEPORT,&Switch,sizeof(int)) < 0)
							{
								syslog(LOG(LOG_ERR),"FTPR_SOCK: Set socket option fault.");
								State_Of_Initialization_SOCK=false;
							}
							else	
								syslog(LOG(LOG_INFO),"FTPR_SOCK: Success to initialized socket interface.");

						}
					}

				}
				else
				{
					syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not get memory for sockets.");	// This memory apply for SOCKETS.
					State_Of_Initialization_SOCK=false;
				}
			}	
			else
			{

		
			#ifdef DEBUG
				syslog(LOG(LOG_ERR),"Code : %d .",Returninfo);
				syslog(LOG(LOG_ERR),"EAI_ADDRFAMILY : %d.",EAI_ADDRFAMILY);
				syslog(LOG(LOG_ERR),"EAI_AGAIN : %d.",EAI_AGAIN);
				syslog(LOG(LOG_ERR),"EAI_BADFLAGS : %d.",EAI_BADFLAGS);
				syslog(LOG(LOG_ERR),"EAI_FAIL : %d.",EAI_FAIL);
				syslog(LOG(LOG_ERR),"EAI_FAMILY : %d.",EAI_FAMILY);
				syslog(LOG(LOG_ERR),"EAI_MEMORY : %d.",EAI_MEMORY);
				syslog(LOG(LOG_ERR),"EAI_NODATA : %d.",EAI_NODATA);
				syslog(LOG(LOG_ERR),"EAI_NONAME : %d.",EAI_NONAME);
				syslog(LOG(LOG_ERR),"EAI_SERVICE : %d.",EAI_SERVICE);
				syslog(LOG(LOG_ERR),"EAI_SOCKTYPE : %d.",EAI_SOCKTYPE);
				syslog(LOG(LOG_ERR),"EAI_SYSTEM : %d.",EAI_SYSTEM);
			#endif

				syslog(LOG(LOG_ERR),"FTPR_SOCK: Cannot get %s\'info Code : %d.",Host_Name,Returninfo);
				syslog(LOG(LOG_ERR),"FTPR_SOCK: ADDRINFO is %p.",ADDRINFO);
				State_Of_Initialization_SOCK=false;	//	Initialize fault.
			}
				
		}

	
	}

	TCP_SOCK_class::~TCP_SOCK_class()
	{
		if (ADDRINFO)	// If it is not null,then clear linked-list.
		{
			freeaddrinfo(ADDRINFO);
			syslog(LOG(LOG_NOTICE),"FTPR_SOCK: Clear node info.");
		}
		else;

		if (SOCKETS)
		{
			//	Close sockets.
			if (**SOCKETS > 0)
				(void)_SHUTDOWN_(**SOCKETS,SHUT_RDWR);
			else;
	
			if (*(*SOCKETS+1) > 0)
				(void)_SHUTDOWN_((*SOCKETS)[1],SHUT_RDWR);
			else;

			//	Recycle resource.
			delete[] SOCKETS;
		}
			
		syslog(LOG(LOG_INFO),"FTPR_SOCK: Destroied tcp socket.");	// Log.
	}	


	/*	Create socket.	*/
	int TCP_SOCK_class::_SOCKET_(int FAMILY,int TYPE,int PROTOCOL)
	{
		return socket(FAMILY,TYPE,PROTOCOL);	// Server or client do not need call this function,so set it in private zone.
	}


	/*	Change byte order.	*/
	uint16_t TCP_SOCK_class::_HTONS_(uint16_t HOST16BITVALUE)
	{
		//	host to net.
		return htons(HOST16BITVALUE);
	}

	/*	Change byte order.	*/
	uint16_t TCP_SOCK_class::_NTOHS_(uint16_t NET16BITVALUE)
	{
		return ntohs(NET16BITVALUE);
	}


	/*	Address string to net address.	*/
	int TCP_SOCK_class::_INET_PTON_(int FAMILY,const char* STRPTR,void* ADDRPTR)
	{
		return inet_pton(FAMILY,STRPTR,ADDRPTR);
	}


	/*	Net address to address string.	*/
	const char* TCP_SOCK_class::_INET_NTOP_(int FAMILY,const void* ADDRPTR,char* STRPTR,size_t LEN)
	{
		return inet_ntop(FAMILY,ADDRPTR,STRPTR,LEN);
	}

	/*	Get host info by host name.	*/
	struct hostent* TCP_SOCK_class::_GETHOSTBYNAME_(const char* HOST)
	{
		return gethostbyname(HOST);
	}

	/*	Bind nsap and tsap.	*/
	int TCP_SOCK_class::_BIND_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				// Server bind communited TSAP-NSAP.
				return bind(**SOCKETS,(const struct sockaddr *)&ADDR_M,sizeof(ADDR_M));	// Bind main.

			case THREAD_SOCKET:
				// Server bind file transport TSAP-NSAP.
				return bind(*(*SOCKETS+1),(const struct sockaddr *)&ADDR_T,sizeof(ADDR_T)); // Bind thread.
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Bind Flag no defined.");
				return -2;	// Differentiate error value.
		}
	}

	/*	Try to make a link with server.	*/
	int TCP_SOCK_class::_CONNECT_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				// Try to link communited port.
				return connect(**SOCKETS,(const struct sockaddr *)&ADDR_M,sizeof(ADDR_M));
			case THREAD_SOCKET:
				// Try to link file transport port.
				return connect(*(*SOCKETS+1),(const struct sockaddr *)&ADDR_T,sizeof(ADDR_T));
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Connect Flag no defined.");
				return -2;	
		}
	}


	/*	Wait client connection.	*/
	int TCP_SOCK_class::_ACCEPT_(const short int Which,struct sockaddr* CLIENTADDR,socklen_t* ADDRLEN)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return accept(**SOCKETS,CLIENTADDR,ADDRLEN);
			case THREAD_SOCKET:
				return accept(*(*SOCKETS+1),CLIENTADDR,ADDRLEN);
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Accept Flag no defined.");
				return -2;
		}
	}
	

	/*	Set state of socket to listen.	*/
	int TCP_SOCK_class::_LISTEN_(const short int Which,int BACKLOG)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return listen(**SOCKETS,BACKLOG);
			case THREAD_SOCKET:
				return listen(*(*SOCKETS+1),BACKLOG);
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Listen flag no defined.");
				return -2;
		}
	}

	void TCP_SOCK_class::_SET_WAIT_TIME_(const short int Which,const unsigned short int & Timing_Seconds)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				Wait_IO_Time_Main=Timing_Seconds;
				break;
			case THREAD_SOCKET:
				Wait_IO_Time_DownUp=Timing_Seconds;
				break;
			default:
				syslog(LOG(LOG_NOTICE),"FTPR_SOCK: Set time flag had not defined.");
		}

	}
	
	unsigned short int TCP_SOCK_class::_GET_WAIT_TIME_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return Wait_IO_Time_Main;
			case THREAD_SOCKET:
				return Wait_IO_Time_DownUp;
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Flag get time had not defined.");
				return 0;
		}
	}

	struct pollfd TCP_SOCK_class::_GET_POLLFD_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return Main;
			case THREAD_SOCKET:
				return Thread;
			default:
				syslog(LOG(LOG_NOTICE),"FTPR_SOCK: Get poll object the flag had not defined.");
				return Main;
		}

	}

	void TCP_SOCK_class::_SET_POLLFD_(const short int Which,struct pollfd & New_Target)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				Main=New_Target;
				break;

			case THREAD_SOCKET:
				Thread=New_Target;
				break;
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Poll set flag no defined.");
		}
	}

	/*	I/O multi-reuse.	*/
	 int TCP_SOCK_class::_POLL_(const short int Which_To_Wait)
	{
		// User must call this function called _POLL_SET_ before.
		switch (Which_To_Wait)
		{
			/*	poll use milliseconds to wait object.	*/

			case MAIN_SOCKET:	// Main
				return poll(&Main,1,Wait_IO_Time_Main*1000);		

			case THREAD_SOCKET:	// Thread.
				
				return poll(&Thread,1,Wait_IO_Time_DownUp*1000);

			default:
				// The flag does not defined.Return -2.

				syslog(LOG(LOG_ERR),"FTPR_SOCK: Poll flag no defined.");
				return -2;
		}		
	}

	void TCP_SOCK_class::_SET_PORT_(const short int Which,const unsigned short int PORT)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				ADDR_M.sin_port=_HTONS_(PORT);
				break;
			case THREAD_SOCKET:
				ADDR_T.sin_port=_HTONS_(PORT);
				break;
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Set port flag had not defined.");
		}
	}

	unsigned short int TCP_SOCK_class::_GET_PORT_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return _NTOHS_(ADDR_M.sin_port);
			case THREAD_SOCKET:
				return _NTOHS_(ADDR_T.sin_port);
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Get port flag had not defined.(Return 0)");
				return 0;
		}
	}

	void TCP_SOCK_class::_SET_ADDR_(const short int Which,struct sockaddr *NewAddr)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				ADDR_M=*(struct sockaddr_in *)NewAddr;
				break;
			case THREAD_SOCKET:
				ADDR_T=*(struct sockaddr_in *)NewAddr;
				break;
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Set addr flag had not defined.");
		}
	}

	struct sockaddr_in TCP_SOCK_class::_GET_ADDR_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return ADDR_M;
			case THREAD_SOCKET:
				return ADDR_T;
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Get addr had not defined.(Return comm)");
				return ADDR_M;
		}
	}

	int TCP_SOCK_class::_GET_SOCKET_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:	// Ge main socket.For communition.
				return **SOCKETS;
			case THREAD_SOCKET:	// Get thread socket.For download and upload.
				return *(*SOCKETS+1);
			default:	// None defined.
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Don't know which socket want to get.");
				return -1;
		}
	}

	void TCP_SOCK_class::_SET_SOCKET_(const short int Which,const int New_Sock)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				**SOCKETS=New_Sock;
				break;
			case THREAD_SOCKET:
				*(*SOCKETS+1)=New_Sock;
				break;
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Set socket flag had not defined.");
		}
	}
	/*	Get socket option.	*/
	 int TCP_SOCK_class::_GETSOCKOPT_(const short int Which,int LEVEL,int OPTNAME,void* OPTVAL,socklen_t* OPTLEN)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return getsockopt(**SOCKETS,LEVEL,OPTNAME,OPTVAL,OPTLEN);
			case THREAD_SOCKET:
				return getsockopt(*(*SOCKETS+1),LEVEL,OPTNAME,OPTVAL,OPTLEN);
			default:
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Get socket option for flag no defined.");
				return -2;
		}
	}

	/*	Set socket option.	*/
	 int TCP_SOCK_class::_SETSOCKOPT_(const short int Which,int LEVEL,int OPTNAME,const void* OPTVAL,socklen_t OPTLEN)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				return setsockopt(**SOCKETS,LEVEL,OPTNAME,OPTVAL,OPTLEN);
			case THREAD_SOCKET:
				return setsockopt(*(*SOCKETS+1),LEVEL,OPTNAME,OPTVAL,OPTLEN);
			default:
				
				// None defined.
				// Because the function 'setsockopt' will return -1 while it failed,so this default return -2.
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Set socket option for flag no defined.");
				return -2;
		}
	}

	/*	Get host name.	*/
	inline int TCP_SOCK_class::_GETHOSTNAME_(char* HOSTNAME,const size_t BUFF_LEN)
	{
		return gethostname(HOSTNAME,BUFF_LEN);
	}

	/*	Network IO.	*/

	ssize_t TCP_SOCK_class::_READ_SOCK_(const int SocketI,void *Net_Buffer,const size_t To_Read)
	{
		ssize_t NextRead(To_Read);	// Record the size of next reading.
		ssize_t Had_Readed(0);		// Record had received in the time.
		ssize_t Temporary_Num(0);	// Record onece readed.
		
		// Define a char pointer assignment with Net_Buffer.

		char *Read_Pointer((char *)Net_Buffer);

		while (Had_Readed < To_Read && NextRead > 0)
		{
			//	Try to read data.
			Temporary_Num=read(SocketI,Read_Pointer,NextRead);
			if (Temporary_Num > 0)
			{
				// Update info.
				Had_Readed+=Temporary_Num;
				NextRead=To_Read-Had_Readed;	// The next read.
				Read_Pointer+=Temporary_Num;	// To next positon.
			}
			else
				break;	// EOF / ERR.
		}

		return Had_Readed; // Return really readed.
	}

	ssize_t TCP_SOCK_class::_WRITE_SOCK_(const int SocketW,const void *Net_Buffer,const size_t To_Write)
	{
		ssize_t NextWrite(To_Write);	// Once to write.
		ssize_t Had_Written(0);		// Had wrote.
		ssize_t Temporary_Num(0);	// Onece to writed.
		const char *Write_Pointer((const char *)Net_Buffer);	// Record write position for next writing.
	
		while (Had_Written < To_Write && NextWrite > 0)
		{
			Temporary_Num=write(SocketW,Write_Pointer,NextWrite);
			if (Temporary_Num > 0)
			{
				// Update.
				Had_Written+=Temporary_Num;	// Had written.
				NextWrite=To_Write-Had_Written;	// Writing size for left.
				Write_Pointer+=Temporary_Num;	// To next data position.
			}
			else
				break;
		}
		return Had_Written;	// Return really to written.
	}


	int TCP_SOCK_class::_SHUTDOWN_(int SOCKFD,int HOWTO)
	{
			int Return(shutdown(SOCKFD,HOWTO));
			(void)close(SOCKFD);
			return Return;
	}

	bool TCP_SOCK_class::_RESET_SOCKETS_(const unsigned short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				(void)_SHUTDOWN_(**SOCKETS,SHUT_RDWR);	// Close old.
				if ((**SOCKETS=_SOCKET_(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) // Open new socket.
					return false;
				else
					return true;

			case THREAD_SOCKET:
				(void)_SHUTDOWN_(*(*SOCKETS+1),SHUT_RDWR);
				if ((*(*SOCKETS+1)=_SOCKET_(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
					return false;
				else	
					return true;

			case MAIN_SOCKET|THREAD_SOCKET:		// Recursive.
				if (_RESET_SOCKETS_(MAIN_SOCKET))
					if (_RESET_SOCKETS_(THREAD_SOCKET))
						return true;
					else
						return false;
				else
					return false;

			default:
				return false;
		}

	}
}

