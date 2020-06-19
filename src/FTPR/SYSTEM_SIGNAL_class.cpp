//	SYSTEM_SIGNAL_class.cpp
//	Version : 0.1
//	Date : Thu Jun 11 18:55:55 2020
//	Last revise : Thu Jun 11 18:55:55 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This file contains the definition of class methods in the class SYSTEM_SIGNAL_class.
//
//	Functions :
//		Some definitions.
//
//	Header :
//		"SYSTEM_SIGNAL_class.h"

namespace SYS_SIGNAL{


	// Methods.
	
	/*	Build and clear.	*/

	SYSTEM_SIGNAL_class::SYSTEM_SIGNAL_class()
	{
		// Suppose the State is true.
		State_Of_Initialization_SIGNAL=true;	

		// Auto to initialize sigset at class creating.
		
		if (_SIGEMPTYSET_(BLOCK_SET) < 0)
		{
			// Set normaly set was fault.
			syslog(LOG(LOG_ERR),"Can not empty signal block set.");
			State_Of_Initialization_SIGNAL=false;
		}
		else
			if (_SIGEMPTYSET_(NORMAL_SET) < 0)
			{
				syslog(LOG(LOG_ERR),"Can not empty signal normaly set.");
				State_Of_Initialization_SIGNAL=false;
			}
			else;

		/*	Signal action set.	*/

		Sigact.sa_mask=Signal_Block;
		Sigact.sa_flags=0;	// Default flags.
		Sigact.sa_flags|=SA_SIGINFO;
		Sigact.sa_sigaction=NULL;

		/*	Signal union object.	*/

		Sigval.sival_int=0;
		Sigval.sival_ptr=NULL;

		/*	Record log.		*/

		syslog(LOG(LOG_NOTICE),"Had created a signal class object.");

	}
	
	SYSTEM_SIGNAL_class::~SYSTEM_SIGNAL_class()
	{
		// Because this class does not use any heap memory,so do not anymore.
		syslog(LOG(LOG_INFO),"Had deleted signal class.");	
	}

	// Define others method.
	
	int SYSTEM_SIGNAL_class::_SIGEMPTYSET_(const unsigned short int Which)
	{
		// This class have two for signal set,a block and a normaly.

		switch (Which)
		{
			case BLOCK_SET:
				// Empty block set.
				return sigempty(&Signal_Block);
			
			case NORMAL_SET:
				// Empty normaly set.
				return sigempty(&Signal_Set);

			default:
				// Return 8 is means that Which had not be defined.
				return 8;
		}

	}

	int _SIGADDSET_(const unsigned short int Which,int SIG)
	{
		// Adding signal to set.
		switch (Which)
		{
			case BLOCK_SET:
				// Add to block.
				return sigaddset(&Signal_Block,SIG);


			case NORMAL_SET:
				// Add to normal.
				return sigaddset(&Signal_Set,SIG);

			default:	
				// If add return 8,the which had not be defined.
				
				return 8;

		}
	}

	int _SIGDELSET_(const unsigned short int Which,int SIG)
	{


		// Del signal to set.
		switch (Which)
		{
			case BLOCK_SET:
				// Del to block.
				return sigdelset(&Signal_Block,SIG);


			case NORMAL_SET:
				// Del to normal.
				return sigdelset(&Signal_Set,SIG);

			default:	
				// If del return 8,the which had not be defined.
				
				return 8;
	}

	int _SIGISMEMBER_(const unsigned short int Which,int SIG)
	{

		// Checking signal in set.
		switch (Which)
		{
			case BLOCK_SET:
				// Checking in block.
				return sigismember(&Signal_Block,SIG);


			case NORMAL_SET:
				// Checking in normal.
				return sigismember(&Signal_Set,SIG);

			default:	
				// If Ismember return 8,the which had not be defined.
				
				return 8;
	}


	int SYSTEM_SIGNAL_class::_SIGACTION_(int SIG,(void)(*ACTION)(int,siginfo_t *,void *),const int FLAG)
	{
		Sigact.sa_flags&=FLAG;			// At first,close other flags.
		Sigact.sa_flags|=FLAG|SA_SIGINFO;	// Open this FLAG and SA_SIGINFO.
		Sigact.sa_sigaction=ACTION;		// Sigaction function.
		return sigaction(SIG,&Sigact,&Oldact);
	}

	inline int SYSTEM_SIGNAL_class::_PAUSE_(void)
	{
		return pause();
	}

	inline int SYSTEM_SIGNAL_class::_SIGSUSPEND_(const sigset_t *SIGMASK)
	{
		return sigsuspend(SIGMASK);
	}

	inline int SYSTEM_SIGNAL_class::_KILL_(pid_t PID,int SIG)
	{
		return kill(PID,SIG);	// Send signal.
	}

	inline int SYSTEM_SIGNAL_class::_SIGQUEUE_(pid_t PID,int SIG,const union sigval INFO)
	{
		return sigqueue(PID,SIG,INFO);	// Send signal.
	}

	inline int SYSTEM_SIGNAL_class::_SIGPROCMASK_(int HOW,const sigset_t *SET,sigset_t *OLDSET)
	{
		// Set signal block set.
		return sigprocmask(HOW,SET,Signal,OLDSET);
	}

	inline int SYSTEM_SIGNAL_class::_SIGPENDING_(sigset_t *Had_Block)
	{
		return sigpending(Had_Block);
	}

	sigset_t SYSTEM_SIGNAL_class::_GET_LAST_SET_(const short int Which)
	{
		switch (Which)
		{
			case NORMAL_SET:
				// Normal set.
				return Signal_Set;

			case BLOCK_SET:	// Otherwise return block set.
			default:
				return Signal_Block;
		}

	}

	inline siginfo_t SYSTEM_SIGNAL_class::_GET_LAST_INFO_(void)
	{
		return Siginfo;
	}
}
