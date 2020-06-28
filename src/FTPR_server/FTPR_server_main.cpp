//	FTPR_server_main.cpp
//	Version : 0.1
//	Date : Sat Jun 27 15:53:05 2020
//	Last revise : Sat Jun 27 15:53:05 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Fork child process and start server as guard process.
//	Functions :
//	Header :
//		"FTPR_server.h"

#include"FTPR_server.h"


bool Server_Should_Not_To_Stop(true); // Server will check this variable before next work startingi.

int main(void)
{
	pid_t *FORK=new pid_t;	// Save fork return.
	if (FORK)
	{
		*FORK=fork();	// Fork.

		if (*FORK != 0)
		{
			delete FORK;	// Parent process quit.
			_exit(0);
		}
		else
		{
			// There is child proces..
			*FORK=setsid();	// Open new session.
			if (*FORK == -1)
			{
				// Failed.
				syslog(LOG(LOG_ERR),"ftprsd: Can not open new session.");
				delete FORK;
				_exit(0);
			}
			else
			{
				syslog(LOG(LOG_NOTICE),"ftprsd: New processes group %ld,leader is %ld.",getpid(),getpid());	// Record.
				// Close 0,1,2.

				(void)close(STDIN_FILENO);
				(void)close(STDOUT_FILENO);
				(void)close(STDERR_FILENO);
				syslog(LOG(LOG_NOTICE),"ftprsd: Close stdio,stdout,stderr.");

				// Create server.
				using namespace FTPR_SERVER;
				FTPR_Server_class *Server=new FTPR_Server_class;

				if (Server == NULL)
				{
					syslog(LOG(LOG_NOTICE),"ftprsd: Failed to create server deamon.");
					_exit(0);
				}
				else
					Server->_Server_();
	
				delete Server;

				syslog(LOG(LOG_NOTICE),"ftprsd: Ready to quit.");
			}


		}
	}
	else
		syslog(LOG(LOG_ERR),"ftprsd: Can not get memory for PID.");

	exit(0);	// Quit.
}
