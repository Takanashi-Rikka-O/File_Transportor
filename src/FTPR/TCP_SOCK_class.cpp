//	TCP_SOCK_class.cpp
//	Version : 0.1
//	Date : Sat May 23 15:12:36 2020
//	Last revise : Sat May 23 15:12:36 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Defintion of Class TCP_SOCK_Class.
//	Functions :
//		Socket functions.
//	Header :
//		"TCP_SOCK_class.h"


#include"TCP_SOCK_class.h"

namespace SOCKET{

	//	Build method is the default.

	// [0]->Main,[1]->Thread.
	
	TCP_SOCK_class::TCP_SOCK_class(unsigned short int Comm_Port,unsigned short int FT_Port,unsigned short int Main_Timing,unsigned short int Thread_Timing)
	{
		//	Suppose the state was succeed.
		State_Of_Initialization_SOCK=true;

		// Open option.
		int Switch(1);

		// Initialize the main and thread structure.
		Main.fd=Thread.fd=-1;
		Main.events=Thread.events=POLLNVAL;	// Invalid event.

		// Socket poll timeout.
		Wait_IO_Time_Main=Main_Timing,Wait_IO_Time_DownUp=Thread_Timing;

		if (_GETHOSTNAME_(Host_Name,127) < 0)	// hostname.
		{
			//	On error.
			syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not get hostname.");	//	Get host name.
			State_Of_Initialization_SOCK=false;
		}
		else
		{
			if ((HOSTINFO=gethostbyname(Host_Name)))	//	Get host info.
			{
				syslog(LOG(LOG_INFO),"FTPR_SOCK: Initialization succee In SOCK.");

				//	Set server address in IPv4.

				ADDR_M.sin_family=HOSTINFO->h_addrtype;	//	Host ip family.
				ADDR_M.sin_addr=*(struct in_addr*)*(HOSTINFO->h_addr_list);	//	Currently host IPv4 address.
				//ADDR.sin_port=_HTONS_(TSAP);	//	Transport service access point. The port must read from file.
			
				//	End.

				ADDR_T=ADDR_M;	// Structure assignement.But port had not assigned.


				SOCKETS=new int[1][2];		//	Get memory.	
				if (SOCKETS)
				{

					**SOCKETS=_SOCKET_(AF_INET,SOCK_STREAM,IPPROTO_TCP);	// Try to open socket.
					if (**SOCKETS < 0)
					{
						// Open socket failed.
						syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not open socketm"); 	// Open failed for socket main.
						State_Of_Initialization_SOCK=false;
						delete[] SOCKETS;
					}
					else
					{
						*(*SOCKETS+1)=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	// Try to open socket.
						if (*(*SOCKETS+1) < 0)				// Open failed for socket thread.
						{
							// Open socket failed.
							syslog(LOG(LOG_ERR),"FTPR_SOCK: Can not open sockett");
							_SHUTDOWN_(**SOCKETS,SHUT_RDWR);	// In there,main socket had been opened.
							delete[] SOCKETS;
						}
						else
						{
							if (_SETSOCKOPT_(THREAD_SOCKET,SOL_SOCKET,SO_REUSEPORT,&Switch,sizeof(int)) < 0)
							{
								syslog(LOG(LOG_ERR),"FTPR_SOCK: Set socket option fault.");
								State_Of_Initialization_SOCK=false;
								delete[] SOCKETS;
								goto TCP_SOCK_class_Quit;	// Jump to nested if-else.
							}
							else;
								
							syslog(LOG(LOG_INFO),"FTPR_SOCK: Initialized for sockets.");

							PORTS=new unsigned short int[1][2];	//	Get memory.
							if (PORTS)
							{
								**PORTS=Comm_Port;	// Communication.
								*(*PORTS+1)=FT_Port;	// File transfer.
							}
							else
							{
								syslog(LOG(LOG_INFO),"FTPR_SOCK: Can not get memory for ports.");
								_SHUTDOWN_(**SOCKETS,SHUT_RDWR);
								_SHUTDOWN_(*(*SOCKETS+1),SHUT_RDWR);
								delete[] SOCKETS;
								State_Of_Initialization_SOCK=false;	
							}
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
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Cannot get host info.");
				State_Of_Initialization_SOCK=false;	//	Initialize fault.
			}
				
		}

		TCP_SOCK_class_Quit:

			__asm__("nop\n\t");
	}

	TCP_SOCK_class::~TCP_SOCK_class()
	{
		if (State_Of_Initialization_SOCK)
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
			delete[] PORTS;
			
			syslog(LOG(LOG_INFO),"FTPR_SOCK: Destroied tcp socket.");	// Log.
		}
		else
			syslog(LOG(LOG_INFO),"FTPR_SOCK: Can not destroy tcp socket,because it had not been created.");	//	Record.
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



	/*	I/O multi-reuse.	*/
	 int TCP_SOCK_class::_POLL_(const short int Which_To_Wait)
	{
		// User must call this function called _POLL_SET_ before.
		switch (Which_To_Wait)
		{
			case MAIN_SOCKET:	// Main

				return poll(&Main,1,Wait_IO_Time_Main);		

			case THREAD_SOCKET:	// Thread.
				
				return poll(&Thread,1,Wait_IO_Time_DownUp);

			default:
				// The flag does not defined.Return -2.

				syslog(LOG(LOG_ERR),"FTPR_SOCK: Poll flag no defined.");
				return -2;
		}		
	}

	void TCP_SOCK_class::_POLL_SET_(int SocketWait,const short int Which,const int Event)
	{
		switch (Which)
		{
			case MAIN_SOCKET:
				
				Main.fd=SocketWait;
				Main.events=Event;	
				break;

			case THREAD_SOCKET:
	
				Thread.fd=SocketWait;
				Thread.events=Event;
				break;
			default:
				;

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
	 int TCP_SOCK_class::_GETHOSTNAME_(char* HOSTNAME,const size_t BUFF_LEN)
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

	int TCP_SOCK_class::_Get_Socket_(const short int Which)
	{
		switch (Which)
		{
			case MAIN_SOCKET:	// Ge main socket.For communition.
				return **SOCKETS;
			case THREAD_SOCKET:	// Get thread socket.For download and upload.
				return *(*SOCKETS+1);
			default:	// None defined.
				syslog(LOG(LOG_ERR),"FTPR_SOCK: Don't know which socket to get.");
				return -1;
		}
	}

	 int TCP_SOCK_class::_SHUTDOWN_(int SOCKFD,int HOWTO)
	{
			return shutdown(SOCKFD,HOWTO);
	}
}

