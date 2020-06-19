//	FTPR.h
//	Version : 0.1
//	Date : Mon Jun  8 20:26:19 2020
//	Last revise : Mon Jun  8 20:26:19 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This is a header of class FTPR,it will as a basic class for server and client.This class contain some method to read config file,and 
//	Functions :
//		Class method.
//	Header :
//		"Share_Header.h"
//		<sys/stat.h>

#ifndef _FTPR_H_
#define _FTPR_H_

#include"Share_Headers.h"
#include"FID_class.h"
#include"THREAD_class.h"
#include"TCP_SOCK_class.h"
#include"SYSTEM_SIGNAL_class.h"
#include<sys/stat.h>	// mode_t type.
#include<fstream>

/*	There define some command number value.	*/
/*	This command would be responed by eigher server and client.	*/
#define LS 20
#define CD 21
#define GET 22
#define UP 23

/*	There define default value of shared optionals.	*/

// This option must is only owns to server.
//#define Default_CWAITS 30	// 30s	This value will be using in the Timer of thread.
#define Default_NIOTIMEOUT 10	// 10s	This value will be using in the net io thread.
#define Default_MBUFF (1024*1024)	// 1MB	This value will be using in the Communition stage.
#define Default_FBUFF (1024*1024)	// 1MB	This value will be using in the File IO stage.(In thread)
//#define Default_CPORT 4396	// Default communition port 4396.
#define Default_FMASK (0177)	// User rw.
#define Default_RSIO 3		// Retry network io 3.






/*	And command parsing function is a virtual function,it must be overwire in the Server and Client.	*/

/*	Define error code for thread.	*/
//#define TIMER_NO_PROBLEM 0
//#define TIMER_SET_CANCEL_STATE_ERR 1
//#define TIMER_SET_CANCEL_TYPE_ERR 2
/*	Because timer will be started by download/upload thread with detach,so these code are invalid.	*/


/*	There define some error codes of Thread_FF.	*/
#define FF_NO_PROBLEM 0	// FF normaly started.
#define FF_SET_CANCEL_STATE_ERR 1	// Can not set cancel enable.
#define FF_SET_CANCEL_TYPE_ERR 2	// Can not set cancel type.
#define FF_BIND_SOCKET_ERR 3		// Can not bind listen socket.
#define FF_OPEN_LISTEN_ERR 4		// Can not open listen queue.
#define FF_OPEN_TIMER_ERR 5		// Can not open a timer.
#define FF_GET_IO_BUFF_ERR 6		// Can not get memory for file IO.
#define FF_CANCEL_TIMER_ERR 7		// Can not shutdown timer which had opened.
#define FF_GET_SUSS 8			// Succeed to finished send file work.
#define FF_UP_SUSS 9			// Succeed to finished receive file work.
#define FF_BEHAVIOR_ERR 10		// Do know what is this work flag.
#define FF_ACCEPT_BAD 11		// Accept return less than 0.
//	Main thread of Thread_FF must call 'join' wait it and receive state code from it.
/*			End.				*/




//	Named space.
namespace FTPR{

	/*	Extern name object.	*/
	using FID::FID_class;			// File IO and directory.
	using PTH::THREAD_class;		// Thread interface.
	using SOCKET::TCP_SOCK_class;		// TCP socket interface.
	using SYS_SIGNAL::SYSTEM_SIGNAL_class;	// System signal.

	struct Shared_Setting{

		// Optional CWAITS
//		unsigned short int Connect_Wait_Timeout;
		
		// Optional MBUFF.
		unsigned short int Message_Buff_Size;

		// OPtional FBUFF.
		unsigned int File_Buff_Size;
	
		// Optional CPORT.
//		unsigned short int Communite_Port;
		
		// Optional DUPORT.
//		unsigned short int Down_Up_Port;

		// Optinal FMASK.
		mode_t Make_File_Mask;
	
		// Optional RSIO.
		unsigned short int Retry_Net_IO;

		// Optional NIOTIMEOUT.
		unsigned short int Network_IO_Timeout;

	};


	/*	For up and down file.	*/
	using GET_UP=struct DownUp{
				
				// The port number of DU thread.
				unsigned short int UsePort;

				// The name of file which next transfer.
				char FILENAME[128];

				// File size.
				size_t FileSize;	

			};	// Use tcp socket to send this structure object.

	/*	Which command had been hot.	*/
	using WCH=struct Which_Command_Hot{

		/*	This structure contain a command Number and a command
 		* a command element void * pointer.	*/	
		
			/* Command number */
			unsigned short int CMDN;
		
			/* Command element pointer */
			void *CMDP;

			};

	/*	Define timer thread argument structure.	*/
	using TMC=struct Time_Manager_To_Cancel{
			// The thread id of thread which will be cancel.
			pthread_t Cancel_TID;
			// Timing value.
			unsigned short int Timeout_Value;
			// THREAD_class interface class.
			THREAD_class *Timer_Thread;
			};	// It will be sent to Thread_FF.

	using NETIOINFO=struct Network_IO_Inforamtion{

			// Option NIOTIMEOUT.
			unsigned short Network_IO_Timeout_Value;
			// Option FBUFF.
			unsigned short Network_File_IO_Buffer;
			// What behavior hot.
			short int What_Behavior;
			// Retry Number.
			unsigned short Retry_Number;

			};


	/*	Define GET/UP command thread argument structure.	*/
	using FGU=struct File_Get_Up{
			// TCP interface class.
			TCP_SOCK_class *Tcp_Sock;
			// File Directory IO interface class.
			FID_class *Fid;
			// Posix thread interface class.
			THREAD_class *IO_Thread;
			// Timer timeout value.
			unsigned short int Timer_Timeout;
			
			// Network io info.
			NETIOINFO Tcp_IO;

			};

	// Define timer thread model and GET/UP thread model.
	// void * (*)(void *);
	void *Thread_Timer(void *Arg);	// Timer.

	void *Thread_FF(void *Arg);	// File transfer.



	/*	Define two function for Timer thread and Download/Upload thread.	*/
	/*	And some structures for them will using.	*/
	/*	Because pthread_create can not set a classic function as start pointer,so define them with normaly C function type.	*/


	class FTPR_Basic{

		protected:
	
			struct Shared_Setting Share_Set;	// Setting structure.
			WCH The_CMD_Hit;		// This structure save the comamnd hit from client.
			

			THREAD_class *PTC;	// Class pointer of THREAD_class.
			FID_class *FIDC;	// Class pointer of FID_class.
			TCP_SOCK_class *TSC;	// Class pointer of TCP_SOCK_class.
			SYSTEM_SIGNAL_class *SYSS;	// System signal.

			bool _Read_Shared_Setting_(const char *Setting_File);	// Read shared setting.
			
			int _Get_Number_Of_Optional_(const char *Option);	// Calculate number from config.
	
			virtual short int _Read_Setting_(const char *Setting_File);	// Config read.
	
			virtual short int _Init_FC_(const void *SETTINGS)	// Use setting init feature classes.
			{
				syslog(LOG_ERR|LOG_USER,"Don't use this virtual function to init feature classes.");
			}
	
			/* Command parsing. */			
			virtual WCH * _Command_Parsing_(const char *Command)
			{
				syslog(LOG_ERR|LOG_USER,"Don't use this virtual function to Parsing command.");
			}

			GET_UP File_Info;	// This object will be using at client request to download or upload a file.

			virtual bool _RECEIVE_FILE_INFO_(const TCP_SOCK_class & TCP_OBJ,GET_UP *Target)	// This function only be overwrite in client.
			{
				// Server have not use this function,but is make a GET_UP object and full it.
			}


		public:

			bool State_Of_Initialization_FTPR;	// Record status in intialization.
	
			FTPR_Basic();		// Default build method.
			virtual ~FTPR_Basic();	// Clear method.It will as a basic class,so it must be virtual method.

			FTPR_Basic(const FTPR_Basic & Temp_Object)
			{
				syslog(LOG_ERR,"This class does not supported copy.");
			}


	};








}




#endif
