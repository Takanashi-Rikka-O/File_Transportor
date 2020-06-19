/*	This file contain some headers them are also contained in other file.	*/

#ifndef _SHARE_HEADERS_H_
#define _SHARE_HEADERS_H_

#include<unistd.h>	/*	UNIX Interfaces			*/
#include<fcntl.h>	/*	UNIX File control		*/
#include<sys/types.h>	/*	UNIX System types		*/
#include<syslog.h>	/*	Linux logger			*/
#include<cstdbool>	/*	C++ ISO C Bool			*/
#include<cstdlib>	/*	C++ ISO C Lib			*/
#include<cstring>	/*	C++ ISO C String		*/
#include<cstdarg>	/*	C++ ISO C Multi parameters	*/
#include<exception>	/*	C++ exception			*/
#include<signal.h>

/*	My assembly function header	*/
#include"_Copy_String_.h"
//	void _Copy_String_(char *des,const char *src,const size_t);

//	For record log as useful,define a marco function to use them.
#define LOG(level) ((level)|LOG_USER)

/*	Define global status description symbol.	*/
#define MAIN_SOCKET 1		// This symbol for descripts communicte socket.
#define THREAD_SOCKET 2		// This symbol for descripts thread socket of GET/UP.
#define GET_FILE 1		// This symbol record which state for thread.
#define UP_FILE 2		// This symbol record which state for thread.
/*	These global description symbol will contain in the share headers.	*/


#endif



