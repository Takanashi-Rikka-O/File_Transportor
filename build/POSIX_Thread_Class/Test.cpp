#include<iostream>

#include"THREAD_class.h"


using namespace std;
using namespace PTH;

using KKSK=void *(*)(void *);

struct TR{

	pthread_t T_ID;
	unsigned short int Timeout;

};

void *Timer(void *Arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	pthread_t TempTID=((THREAD_class *)Arg)->_GET_ID_SELF_();

	for (unsigned short int Timing=0; Timing < 10; ++Timing)
	{
		((THREAD_class *)Arg)->_LOCK_MUTEX_();
		cout<<"TID = 2 "<<" Index= "<<Timing<< "Lock"<<endl;
		
		// Test dead lock.
		if (Timing == 5)
			exit(1);
		else;

		((THREAD_class *)Arg)->_UNLOCK_MUTEX_();
	}

	pthread_exit(NULL);
}

void* FUNC(void* arg)
{
	THREAD_class* Temp((THREAD_class*)arg);

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	pthread_t Temp_TID,My_TID;

	My_TID=Temp->_GET_ID_SELF_();

	Temp->_CREATE_THREAD_(&Temp_TID,&(Temp->Thread_Attr),Timer,Temp);	//	Open timer.
	for (int Index=0; Index < 10; ++Index)
	{
		Temp->_LOCK_MUTEX_();
		cout<<"TID = 1"<<" Index= "<<Index<<" Lock"<<endl;
		Temp->_UNLOCK_MUTEX_();
	}

	pthread_exit(NULL);
}

int main(void)
{
	KKSK fc=FUNC;

	pthread_t ID;
	THREAD_class Test;
	Test._UNLOCK_MUTEX_();
	Test._CREATE_THREAD_(&ID,NULL,fc,(void*)&Test);

	cout<<"Main waitting\n";

	if (Test._JOIN_THREAD_(ID,NULL) == EINVAL)
	{
		cout<<"Join failed\n";
	}
	else;


	cout<<"Main quit\n";

	return 0;

}

