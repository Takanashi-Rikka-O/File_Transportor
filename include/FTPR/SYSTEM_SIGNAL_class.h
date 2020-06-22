//	SYSTEM_SIGNAL_class.h
//	Version : 0.1
//	Date : Wed Jun 10 20:17:51 2020
//	Last revise : Wed Jun 10 20:17:51 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This header contain defintion of class SYS_SIGNAL_class.This class is a interface class for system signal in the named space 'SYSTEM_SIGNAL'.
//	FTPR contain this class,and server or client could using this class to setting signal action in its environment.
//
//	Functions :
//		Some signal interface functions.
//
//	Header :
//		<signal.h>		//	System signal.
//		"Share_Headers.h"	//	Shared headers.


#ifndef _SYSTEM_SIGNAL_CLASS_H_
#define _SYSTEM_SIGNAL_CLASS_H_

#include<signal.h>
#include"Share_Headers.h"

/*	Define model for set.	*/
#define BLOCK_SET 1
#define NORMAL_SET 2
/*	End of model.	*/

namespace SYS_SIGNAL{

	class SYSTEM_SIGNAL_class{

		private :	// Private members.
			
			// Signal set object.
			sigset_t Signal_Set;

			// Signal set block.
			sigset_t Signal_Block;	

			// Signal structure.
			struct sigaction Sigact;
			struct sigaction Oldact;

		public :	// Public members.

			// State register.
			bool State_Of_Initialization_SIGNAL;

			SYSTEM_SIGNAL_class();	// Build function.
			~SYSTEM_SIGNAL_class();	// Destruct function.

			// Interfaces.
			
			/*	Signal set.	*/
			int _SIGEMPTYSET_(const unsigned short int Which);
			int _SIGADDSET_(const unsigned short int Which,int SIG);
			int _SIGDELSET_(const unsigned short int Which,int SIG);
			int _SIGISMEMBER_(const unsigned short int Which,int SIG);
	
			/*	Signal action.	*/
			int _SIGACTION_(int SIG,void(*ACTION)(int,siginfo_t *,void *),const int FLAG);
			
			/*	Suspend call.	*/
			int _PAUSE_(void);
			int _SIGSUSPEND_(const sigset_t *SIGMASK);

			/*	Signal	send.	*/
			int _KILL_(pid_t PID,int SIG);
			int _SIGQUEUE_(pid_t PID,int SIG,const union sigval INFO);

			/*	Signal block.	*/
			int _SIGPROCMASK_(int HOW,const sigset_t *SET,sigset_t *OLDSET);
			// 'HOW' is effictive while the SET is not null.
			int _SIGPENDING_(sigset_t *Had_Block);
		
			/*	Get set.	*/
			sigset_t _GET_LAST_SET_(void)
			{
				return Signal_Set;
			}

			/*	Get the old sigaction set.	*/
			struct sigaction _GET_LAST_SIGACTON_(void)
			{
				return Oldact;
			}

			/*	Get signal info.	*/	// None usage of this function,because siginfo_t object often using in sigaction function.
//			siginfo_t _GET_LAST_INFO_(void);

	};





}



#endif


/*	Linux system signals.

 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
 6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
63) SIGRTMAX-1	64) SIGRTMAX	

*/
