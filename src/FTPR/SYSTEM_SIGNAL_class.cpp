//	SYSTEM_SIGNAL_class.cpp
//	Version : 0.2
//	Date : Thu Jun 11 18:55:55 2020
//	Last revise : Sat Jun 20 21:42:55 2020
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
//	Fix :
//		1> Adjustment record format,delete extral non usage members.

#include"SYSTEM_SIGNAL_class.h"

namespace SYS_SIGNAL{


	// Methods.
	
	/*	Build and clear.	*/

	SYSTEM_SIGNAL_class::SYSTEM_SIGNAL_class()
	{
		// Suppose the State is true.
		State_Of_Initialization_SIGNAL=true;
		
		// Set memory.
		memset(&Sigact,'\0',sizeof(struct sigaction));
		memset(&Oldact,'\0',sizeof(struct sigaction));

		// Auto to initialize sigset at class creating.
		
		if (_SIGFILLSET_(BLOCK_SET) < 0)
		{
			// Set normaly set was fault.
			syslog(LOG(LOG_ERR),"FTPR_SIGNAL: Can not fill signal block set.");
			State_Of_Initialization_SIGNAL=false;
		}
		else
			if (_SIGEMPTYSET_(NORMAL_SET) < 0)
			{
				syslog(LOG(LOG_ERR),"FTPR_SIGNAL: Can not empty signal normaly set.");
				State_Of_Initialization_SIGNAL=false;
			}
			else;

		/*	Signal action set.	*/

		Sigact.sa_mask=SignalBlock;
		Sigact.sa_flags=0;	// Default flags.
		Sigact.sa_flags|=SA_SIGINFO;	// Always receive the extended information.
		Sigact.sa_sigaction=NULL;

		/*	Record log.		*/

		syslog(LOG(LOG_NOTICE),"FTPR_SIGNAL: Success to initialized signal interface.");

	}
	
	SYSTEM_SIGNAL_class::~SYSTEM_SIGNAL_class()
	{
		// Because this class does not use any heap memory,so do not anymore.
		syslog(LOG(LOG_INFO),"FTPR_SIGNAL: Success to deleted signal interface.");	
	}

	// Define others method.
	
	int SYSTEM_SIGNAL_class::_SIGEMPTYSET_(const unsigned short int Which)
	{
		// This class have two for signal set,a block and a normaly.

		switch (Which)
		{
			case BLOCK_SET:
				// Empty block set.
				return sigemptyset(&SignalBlock);
			
			case NORMAL_SET:
				// Empty normaly set.
				return sigemptyset(&SignalSet);

			default:
				// Return 8 is means that Which had not be defined.
				return 8;
		}

	}

	int SYSTEM_SIGNAL_class::_SIGFILLSET_(const unsigned short int Which)
	{
		switch (Which)
		{
			case BLOCK_SET:
				return sigfillset(&SignalBlock);
			case NORMAL_SET:
				return sigfillset(&SignalSet);
			default:
				return 8;


		}
	}

	int SYSTEM_SIGNAL_class::_SIGADDSET_(const unsigned short int Which,int SIG)
	{
		// Adding signal to set.
		switch (Which)
		{
			case BLOCK_SET:
				// Add to block.
				return sigaddset(&SignalBlock,SIG);

			case NORMAL_SET:
				// Add to normal.
				return sigaddset(&SignalSet,SIG);

			default:	
				// If add return 8,the which had not be defined.
				
				return 8;

		}
	}

	int SYSTEM_SIGNAL_class::_SIGDELSET_(const unsigned short int Which,int SIG)
	{


		// Del signal to set.
		switch (Which)
		{
			case BLOCK_SET:
				// Del to block.
				return sigdelset(&SignalBlock,SIG);


			case NORMAL_SET:
				// Del to normal.
				return sigdelset(&SignalSet,SIG);

			default:	
				// If del return 8,the which had not be defined.
				
				return 8;
		}
	}

	int SYSTEM_SIGNAL_class::_SIGISMEMBER_(const unsigned short int Which,int SIG)
	{

		// Checking signal in set.
		switch (Which)
		{
			case BLOCK_SET:
				// Checking in block.
				return sigismember(&SignalBlock,SIG);


			case NORMAL_SET:
				// Checking in normal.
				return sigismember(&SignalSet,SIG);

			default:	
				// If Ismember return 8,the which had not be defined.
				
				return 8;
		}
	}


	int SYSTEM_SIGNAL_class::_SIGACTION_(int SIG,void(*ACTION)(int,siginfo_t *,void *),const int FLAG)	// Default.
	{
		Sigact.sa_flags&=FLAG;			// At first,close other flags.
		Sigact.sa_flags|=FLAG|SA_SIGINFO;	// Open this FLAG and SA_SIGINFO.
		Sigact.sa_sigaction=ACTION;		// Sigaction function.
		Sigact.sa_mask=SignalSet;		// Mask,default is empty.
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

	int SYSTEM_SIGNAL_class::_SIGPROCMASK_(int HOW,sigset_t *OLDSET)
	{
		// Set signal block set.
		return sigprocmask(HOW,&SignalBlock,OLDSET);
	}

	inline int SYSTEM_SIGNAL_class::_SIGPENDING_(sigset_t *Had_Block)
	{
		return sigpending(Had_Block);
	}

}
