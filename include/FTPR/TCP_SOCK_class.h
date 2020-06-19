//	TCP_SOCK_class.h
//	Version : 0.1
//	Date : Fri May 29 10:26:56 2020
//	Last revise : Fri May 29 10:26:56 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Define tcp socket operation class.
//
//	Functions :
//		Some socket control interfaces.
//
//	Header :
//		<unistd.h>		UNIX Standard functions.
//		<syslog.h>		System log record.
//		<sys/socket.h>		Socket.
//		<netinet/in.h>		Net protocol.
//		<netdb.h>		Net database.
//		<arpa/inet.h>		ARP.
//		<poll.h>		poll();
//		

#ifndef _TCP_SOCK_CLASS_H_
#define _TCP_SOCK_CLASS_H_

#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<poll.h>

#include"Share_Headers.h"

// Because had been contianed global descript symbol,do not define symbols for this file.

namespace SOCKET{

	//	This class is a zone of tcp socket.

	class TCP_SOCK_class{

		private:	//	Private zone.
		
			/*	This pointer would be pointing to a array which has two for elements.	*/
			/*	[0] is communication socket.	*/
			/*	[1] is file transport socket.	*/
			int (*SOCKETS)[2];	

			/*	This pointer would be pointing to a array which has two for elements.	*/
			/*	[0] is communication port.	*/
			/*	[1] is file transport port.	*/
			unsigned short int (*PORTS)[2];	//	Ports pointer.

			char Host_Name[128];	//	Host name.	
	
			struct sockaddr_in ADDR_M;	//	Address structure. IPv4.This structure built for Main thread.
			struct sockaddr_in ADDR_T;	// 	Address structure. IPv4.This structure built for DownUp thread.

			struct hostent* HOSTINFO;	//	Structure hostent pointer.

			unsigned short int Wait_IO_Time_Main;	// This variable control the timeout for Main thread I/O.
			unsigned short int Wait_IO_Time_DownUp;	// This variable control the timeout for Down/Up thread I/O.

			struct pollfd Main;	// Wait main.
			struct pollfd Thread;	// Wait thread.


			/*	Create socket.	*/
			int _SOCKET_(int FAMILY,int TYPE,int PROTOCOL);	// Server and client don't have to call this function to create anpothers sockets.
			// This function will be calling by build method.


		public:		//	Public zone.

			//	Monitoring status.
			bool State_Of_Initialization_SOCK;

			TCP_SOCK_class();	//	build
			~TCP_SOCK_class();	//	clear.

			/*	Host byte order convertion.	*/

			//	_HTONS_ and _NTOHS_ are respone port convert.	
			uint16_t _HTONS_(uint16_t HOST16BITVALUE);	
			uint16_t _NTOHS_(uint16_t NET16BITVALUE);
					
			//	_INET_PTON_ and _INET_NTOP_ are respone IPv4/IPv6 address convert.	
			/*	Print to net.	*/
			int _INET_PTON_(int FAMILY,const char* STRPTR,void* ADDRPTR);
			/*	Net to print.	*/
			const char* _INET_NTOP_(int FAMILY,const void* ADDRPTR,char* STRPTR,size_t LEN);

			/*	Open netdb and get host info.	*/
			struct hostent* _GETHOSTBYNAME_(const char* HOST);	// Search host infomation in net database.

			/*	Bind NSAP with TSAP.	*/
			int _BIND_(const short int Which);	// Try to bind a socket.
								// If client bind a socket and then connect to server,the network asynchronize error will 
								// return to client process.

			/*	Make a connection.	*/
			int _CONNECT_(const short int Which);	// Try to connect.

			/*	Entry listen model.	*/
			int _LISTEN_(const short int Which,int BACKLOG); // Open listen.

			/*	Entry connection waiting model.	*/
			int _ACCEPT_(const short int Which,struct sockaddr* CLIENTADDR,socklen_t* ADDRLEN);	// Wait link.
		
			/*	Shutdown socket.	*/
			int _SHUTDOWN_(int SOCKFD,int HOWTO);	// Either connected object call this function,will send a FIN to peer.
			//	HOWTO =
			//		SHUT_RD , shut read down.
			//		SHUT_WR , shut write down.
			//		SHUT_RDWR , shut I/O down.
			
			/*	Wait network event arrive.	*/
			int _POLL_(const short int Which_To_Wait);
			/* For simply to use this class,user could not specify pollfd array and timeout,but user must specify wait which event.	*/
			

			// M_Or_D control set which pollfd.
			bool _POLL_SET_(const short int Comm_DU,const int Event,const int TIMEOUT_Value);	
			// Use this function must apply a constraint,the arraies must as int-0 end.

			bool _Check_Main_(const int Event)	// If revent by poll is equal to this parameter,return true.
			{
				return (Event == Main.revents)?true:false;	// Check the latest poll is really to happened event of want.
			}

			bool _Check_Thread_(const int Event)	// If revent by poll is equal to this parameter,return true.
			{
				return (Event == Thread.revents)?true:false;
			}			
	
			/*	Get and Set socket options.	*/
			int _GETSOCKOPT_(const short int Which,int LEVEL,int OPTNAME,void* OPTVAL,socklen_t* OPTLEN);
			int _SETSOCKOPT_(const short int Which,int LEVEL,int OPTNAME,const void* OPTVAL,socklen_t OPTLEN);

			/*	Get host name.	*/
			int _GETHOSTNAME_(char* HOSTNAME,const size_t BUFF_LEN);

			/*	Network socket IO functions	*/
			ssize_t _READ_SOCK_(const int SocketI,void *Net_Buffer,const size_t To_Read);	// If read a '\0',reading 
			ssize_t _WRITE_SOCK_(const int SocketW,const void *Net_Buffer,const size_t To_Write);
		
			// Use this function to set ports.
			bool _Set_Ports_(const short int Which,const unsigned short int Port_Number);	// [0]->Main,[1]->Thread.	
			int _Get_Socket_(const short int Which);

	};







}

//	Header end.

#endif
