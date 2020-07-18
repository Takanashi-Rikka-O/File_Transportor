//	libFTPR_Server.cpp
//	Version : 0.1
//	Date : Thu Jul 16 10:15:35 2020
//	Last revise : Sat Jul 18 21:18:? 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This file contain some server interfaces,them will as shared library and link to program.For upgarde,use this method as well.
//		And the lib functions :
//			<read settings>
//			<work up>
//
//	Functions :
//		short int _Read_Setting_(const char *Setting_File);	// This is a virtual function.
//		void _Server_WorkUp_(void);
//
//	Header :
//		FTPR_server.h
//
//	Fix : 

#include"FTPR_server.h"


namespace FTPR_SERVER{

	using std::ifstream;
	using std::ios_base;

	// Virtual Functions in protected.
	// And for simply to update,will make this function as a share lib.
	short int FTPR_Server_class::_Read_Setting_(const char *Setting_File)
	{
		#ifdef DEBUG

		syslog(LOG(LOG_NOTICE),"FTPR_Server: Configure file - %s.",Setting_File);

		#endif

		// For make same format to return function code,declare a variable.
		short int _Read_Setting_Code(0);


		// Get reading buffer.
		char *Setting_Buffer=new char[64+1];	// 64B.
		if (! Setting_Buffer)
		{
			_Read_Setting_Code=S_RS_GET_MEM_ERR;	
			return _Read_Setting_Code;
		}
		else;
		
		// Temporary string position pointer.
		char *Temp_Char_Pointer(NULL);

		// Temporary value for get number.
		int Temporary_Value(0);
	

		// FTPR_Basic had been defined setting read function for shared settings.
		// When it succeed to read,return true.
		if (FTPR_Basic::_Read_Shared_Setting_(Setting_File))
			;
		else
		{
			
			_Read_Setting_Code=S_RS_SHARE_SET_ERR;
			delete[] Setting_Buffer;
			return _Read_Setting_Code;
		}

		ifstream Read;	// Define a ifstream object.
		Read.open(Setting_File,ios_base::in);	// ASCII input.

		/* Check it was succeed to opened.	*/
		if (Read.is_open())
			;
		else
		{
			_Read_Setting_Code=S_RS_OPEN_CONFIG_ERR;
			delete[] Setting_Buffer;
			return _Read_Setting_Code;
		}

		// Entry a cycle to read unique setting optionals.
		// Stop while fail bit set out.
		while (true)
		{

			Read.getline(Setting_Buffer,65);	// Read 64B
			if (Read.eof())	// If eof,break cycle.
				break;
			else if (Read.fail())	// This case is normaly to be caused by left characters in stream but had readed the max limit.
			{
				Read.clear();
				continue;
			}
			else;

			if ('#' == *Setting_Buffer || '\0' == *Setting_Buffer) // Skip comment.
				continue;
			else
			{
				if (strncmp(Setting_Buffer,"DUPORT",6) == 0)
					DownUp_Port=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer)) > 0)?Temporary_Value:Default_DUPORT;
				else if (strncmp(Setting_Buffer,"CWAITS",6) == 0)
					Comm_Wait_Seconds=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer)) > 0)?Temporary_Value:Default_CWAITS;
				else if (strncmp(Setting_Buffer,"CPORT",5) == 0)
					Comm_Port=((Temporary_Value=FTPR_Basic::_Get_Number_Of_Optional_(Setting_Buffer)) > 0)?Temporary_Value:Default_CPORT;
				else if (strncmp(Setting_Buffer,"RPATH",5) == 0)			
				{
					Temp_Char_Pointer=strstr(Setting_Buffer,"=");
					if (*(Temp_Char_Pointer+1) != '\0')
						_Copy_String_(Root_Path,Temp_Char_Pointer+1,strlen(Temp_Char_Pointer+1));
					else
						_Copy_String_(Root_Path,Default_RPATH,10);
				}
				else;
			}
		}

		// Check it is normaly finished.
		if (Read.eof())
			_Read_Setting_Code=S_RS_SET_OVER;
		else
			_Read_Setting_Code=S_RS_SET_ERR;
		
		// Close file.
		Read.clear();	// Flush flag.
		Read.close();
		
		delete[] Setting_Buffer;

		return _Read_Setting_Code;

	}


	void FTPR_Server_class::_Server_WorkUp_(void)
	{
	
		_Server_Logger_(S_ENTRY_WORK);	// Entrance record.
		
		char *UserCmd(NULL);
		try{
			// For command buffer.
			UserCmd=new char[Share_Set.Message_Buff_Size];
			if (UserCmd)
				;
			else
				throw 0;
		}
		catch(short int ZERO)
		{
			UserCmd=new char[Share_Set.Message_Buff_Size];
			if (UserCmd)
				;
			else
			{
				_Server_Logger_(S_FAIL_MEM);	// Because WORKUP_MEM_ERR = 0
				return;
			}

		}
		
		// For accept.
		int New_Chat(-1);

		// Thread_FF arguments.
		// This structure have some tcp setting parameters.
		NETIOINFO NetIO={
			.Network_File_IO_Buffer=Share_Set.File_Buff_Size,
			.What_Behavior=0,
			.Retry_Number=Share_Set.Retry_Net_IO,
			};

		// This structure will send to Thread_FF.
		FGU Thread_Resource={
			.Tcp_sock=TSC,
			.Fid=FIDC,
			.Thread=PTC,
			.Timer_Timeout=Comm_Wait_Seconds,
			.Net_io=&NetIO,
			.Work_id=SERVER_ID,
			};

		// Their member value could be determines before them had be used.

		// For peer address.
		struct sockaddr_in Client_Address;
		socklen_t Address_Length(sizeof(struct sockaddr_in));
		char Address_String[16];	// For string.

		// For poll.
		struct pollfd TempPoll;

		// Bind socket.
		if (TSC->_BIND_(MAIN_SOCKET) < 0)
		{
			_Server_Logger_(S_BIND_ERR);
//			syslog(LOG(LOG_ERR),"FTPR_Server: Can not bind communication socket.");
			goto _WorkUp_Quit;
		}
		else
			if (TSC->_LISTEN_(MAIN_SOCKET,1) < 0)
			{
				_Server_Logger_(S_LISTEN_ERR);
//				syslog(LOG(LOG_ERR),"FTPR_Server: Can not open listen queue for communication socket.");
				goto _WorkUp_Quit;
			}
			else;

		// Entry cycle.


		syslog(LOG(LOG_ERR),"FTPR_Server: Entry wait connection.");

		while (true && Server_Should_Not_To_Stop)
		{

			_Server_Logger_(S_ENTRY_WORK_ACCEPT);	// Record.

			// Wait connection arrived.	
			New_Chat=TSC->_ACCEPT_(MAIN_SOCKET,(struct sockaddr *)&Client_Address,&Address_Length);
			if (New_Chat > 0)
			{
				// Set wait target.
				TempPoll.fd=New_Chat;
				TempPoll.events=POLLRDNORM;

				// Succeed to accept a connection.
				syslog(LOG(LOG_NOTICE),"FTPR_Server: Succeed to accept a connection %s:%hu"
					,TSC->_INET_NTOP_(AF_INET,&Client_Address.sin_addr,Address_String,16)
					,TSC->_NTOHS_(Client_Address.sin_port)
				);

				TSC->_SET_POLLFD_(MAIN_SOCKET,TempPoll);	// Set pollfd.

				// Entry communication cycle.

				do
				{

					TSC->_POLL_(MAIN_SOCKET);	// Wait poll timeout or data arrived.
					if (! TSC->_Check_Main_(POLLRDNORM))
						continue;
					else if (TSC->_Check_Main_(POLLRDHUP))
						break;
					else;	

					memset(UserCmd,'\0',Share_Set.Message_Buff_Size);	// Clear old data.
						
					// Client sent data.

//					if (TSC->_READ_SOCK_(New_Chat,UserCmd,Share_Set.Message_Buff_Size) > 0)
					if (read(New_Chat,UserCmd,Share_Set.Message_Buff_Size) > 0)
					{
						// Call command parasing.

						_Command_Parsing_(UserCmd);

						// Check WCH structure.

						switch (The_CMD_Hit.CMDN)
						{
							case LS:
								_Server_Logger_(S_CMD_LS);
								
								// Cycle to write data.
								_LS_(New_Chat,UserCmd);

								break;

							case CD:
								_Server_Logger_(S_CMD_CD);
								
								_CD_(New_Chat,(const char *)The_CMD_Hit.CMDP);
								break;

							case GET:
								_Server_Logger_(S_CMD_GET);
								NetIO.What_Behavior=GET_FILE;	// Set cause.
								_GET_FILE_(&Thread_Resource,New_Chat);	// Process this cmd.

								TSC->_RESET_SOCKETS_(THREAD_SOCKET);	// Cancel bind.

								syslog(LOG(LOG_NOTICE),"FTPR_Server: Server close thread.");

								break;
																
							case UP:
								_Server_Logger_(S_CMD_UP);
								NetIO.What_Behavior=UP_FILE;
								_UP_FILE_(&Thread_Resource,New_Chat);	// Process this cmd.

								TSC->_RESET_SOCKETS_(THREAD_SOCKET);	// Cancel bind.

								syslog(LOG(LOG_NOTICE),"FTPR_Server: Server close thread.");

								break;
			
							case LOGOUT:
								goto UserLogout;	// User want to quit.
						
							default:
								syslog(LOG(LOG_ERR),"FTPR_Server: None defined command.");
						}

						// Switch end.
					}
					else
						break;	// EOF/error.

				} while (true && Server_Should_Not_To_Stop);

				// User logout.

				UserLogout:
					TSC->_SHUTDOWN_(New_Chat,SHUT_RDWR);

			}
			else
			{	
				// Failed to accept a connection.
				syslog(LOG(LOG_NOTICE),"FTPR_Server: Failed to accept a connection .");
			}
			// Accept.	

		}	// While


		_WorkUp_Quit:

			delete[] UserCmd;	// Recycle resource.
	}


}
