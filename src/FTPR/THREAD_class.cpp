//	THREAD_class.cpp
//	Version : 0.1
//	Date : Sat May 23 15:12:26 2020
//	Last revise : Sat May 23 15:12:26 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Definition of the class 'THREAD_class'.
//
//	Functions :
//		Some POSIX Thread control.
//
//	Header :
//		"THREAD_class.h"


#include"THREAD_class.h"

//	Attention : Some times use keyword '' would make 'C' interfaces can not be called by C++ program in normally.

namespace PTH{


	THREAD_class::THREAD_class()
	{
		State_Of_Initialization_THREAD=true;	// Suppose it is true.

		// Init attribute object.
		if (_INIT_ATTR_() != 0)
		{

			State_Of_Initialization_THREAD=false;	//	Fail.
		}
		else
			if (_SET_DETACH_STATE_()!= 0)
			{
				syslog(LOG(LOG_ERR),"FTPR_THREAD: Set detach state for thread attrbute object was fault!");
				State_Of_Initialization_THREAD=false;
			}
			else;

		// Init mutex object.

		if (_INIT_MUTEX_() != 0)
		{
			syslog(LOG(LOG_ERR),"FTPR_THREAD: Init mutex failed.");
			State_Of_Initialization_THREAD=false;
		}
		else;

	}

	THREAD_class::~THREAD_class()
	{
		if (State_Of_Initialization_THREAD)
		{

			if (_DESTROY_ATTR_() != 0)
				syslog(LOG(LOG_ERR),"FTPR_THREAD: Error,process can not destroy thread attrbute object!");
			else;
	
			syslog(LOG(LOG_NOTICE),"FTPR_THREAD: Destroy mutex.");
	
			if (_DESTROY_MUTEX_() != 0)
				syslog(LOG(LOG_ERR),"FTPR_THREAD: Failed to delete mutex.");
			else;

			syslog(LOG(LOG_INFO),"FTPR_THREAD: Thread object deleting!");
		}
		else;
	}

	//	Public zone.
	
	pthread_t THREAD_class::_GET_ID_SELF_(void)
	{
		return pthread_self();	//	Get ID of itself.
	}

	int THREAD_class::_CREATE_THREAD_(pthread_t* TID,pthread_attr_t *ATTR,void*(*WORK_FUNC)(void*),void* ARG)
	{
		//	Open new thread,
		return pthread_create(TID,ATTR,WORK_FUNC,ARG);	
	}	

	 int THREAD_class::_CANCEL_THREAD_(pthread_t TID)
	{
		return pthread_cancel(TID);	//	Request to cancel a thread.
	}

	 int THREAD_class::_DETACH_THREAD_(pthread_t TID)
	{
		return pthread_detach(TID);	//	Try to detach a thread.
	}

	 int THREAD_class::_INIT_ATTR_(void)
	{
		return pthread_attr_init(&Thread_Attr);	//	Initialize.
	}

	 int THREAD_class::_DESTROY_ATTR_(void)
	{
		return pthread_attr_destroy(&Thread_Attr);	//	Destroy.
	}

	 int THREAD_class::_GET_DETACH_STATE_(int* DETACH_STATE)
	{	
		//	Get detach state.
		return pthread_attr_getdetachstate(&Thread_Attr,DETACH_STATE);
	}	

	 int THREAD_class::_SET_DETACH_STATE_(int DETACH_STATE)
	{
		return pthread_attr_setdetachstate(&Thread_Attr,DETACH_STATE);
	}


}
