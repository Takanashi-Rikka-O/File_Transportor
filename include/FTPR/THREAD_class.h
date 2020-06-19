//	THREAD_class.h
//	Version : 0.1
//	Date : Sun May 24 16:40:32 2020
//	Last revise : Sun May 24 16:40:32 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This class of POSIX Thread control,it would be containing by FTPR class.This class contain some pthread control method and 
//	some necessary variables.For prevent two threads processing at same time,them must have a thread lock to synchronize.
//
//	Functions :
//		Some pthread control functions.
//
//	Header :
//		<pthread.h>		//	POSIX.1 thread.
//		<unistd.h>		//	UNIX Standard functions.
//		<exception>		//	C++ exception.
//		<cstdlib>		//	ISO C Standard lib.
//		<cstdbool>		//	ISO C Standard bool type.
//		<syslog.h>		//	System log.
//

#ifndef _THREAD_CLASS_H_
#define _THREAD_CLASS_H_

#include<pthread.h>

#include"Share_Headers.h"



namespace PTH{

	class THREAD_class{

		// Because Download Upload thread will open a new thread as timer,so does not contain any pthread_t object.


		public:
		
			THREAD_class();		//	Build method.
			~THREAD_class();				//	Destroy method.

			bool State_Of_Initialization_THREAD;		//	This variable to record the status of initialization for this class.


			// Because the two object will be using by timer,so move them to public zone.
			pthread_attr_t Thread_Attr;	//	POSIX Thread attrbute object.

			pthread_mutex_t Thread_Mutex;	//	POSIX Thread mutex lock.



			pthread_t _GET_ID_SELF_(void);			//	Thread call this function to get ID of itself.

			int _CREATE_THREAD_(pthread_t *TID,pthread_attr_t *ATTR,void *(*WORK_FUNC)(void *),void *ARG);	//	Open a thread.

			int _CANCEL_THREAD_(pthread_t TID);	//	Cancel a thread.

			int _DETACH_THREAD_(pthread_t TID);	//	Detach a thread.

			int _INIT_ATTR_(void);	//	Initialize for thread attr object.
			
			int _DESTROY_ATTR_(void);	//	Destroy a thread attr object.

			int _GET_DETACH_STATE_(int* DETACHE_STATE);	//	Get thread detach state.
		
			int _SET_DETACH_STATE_(int DETACH_STATE=PTHREAD_CREATE_DETACHED);	//	Set thread detach state.

			int _JOIN_THREAD_(pthread_t TID,void** Result_Ptr)	//	Wait a thread.
			{
				return pthread_join(TID,Result_Ptr);	// Do not join wait a detach pthread.
			}

				//	Detach state have :
				//		PTHREAD_CREATE_DETACHED / PTHREAD_CREATE_JOINABLE ;

			// Posix threads lock.
			int _INIT_MUTEX_(void)	// Init pthread mutex,and use default attribut.
			{
				return pthread_mutex_init(&Thread_Mutex,NULL);	// Init.
			}
			int _DESTROY_MUTEX_(void)	// Detroy a pthread mutex object.
			{
				return pthread_mutex_destroy(&Thread_Mutex);
			}
		
			// Lock and unlock.
			int _LOCK_MUTEX_(void)		// Lock,block.
			{
				return pthread_mutex_lock(&Thread_Mutex);
			}
			int _UNLOCK_MUTEX_(void)	// Unlock.
			{
				return pthread_mutex_unlock(&Thread_Mutex);
			}

	};



}

#endif	
