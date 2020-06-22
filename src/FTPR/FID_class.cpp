//	FID_class.cpp
//	Version : 0.2
//	Date : Sat May 23 15:12:12 2020
//	Last revise : Sat Jun 20 22:30:? 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		Class FID_class definition.
//
//	Functions :
//		
//		Some file and directory and lock operation functions.
//
//	Header :
//		"FID_class.h"
//	Fix :
//		1> Adjustment build method and record format.(Now when class was be created it will make program change work home.)


#include"FID_class.h"
#include<iostream>

//#define GET_MEM_ERR_W 1
//#define GET_MEM_ERR_N 2
//#define GET_MEM_ERR_O 3

//#define ERR_STR_MEM "Can not get memory - Code : %d"


namespace FID{
	
	FID_class::FID_class(const char *New_Work_Home):Directory(NULL),List_Head(NULL),Current_Length_List(0),Current_Length_FDES(0)	//	Build method.
	{

		//	State

		State_Of_Initialization_FID=true;	//	Suppose it's succeed.

		FID_TempFile=-1;

		//	Get os limits.

		_PATH_MAX_=pathconf(".",_PC_PATH_MAX);	//	Path name of current directory.
		_NAME_MAX_=pathconf(".",_PC_NAME_MAX);	//	File name max of current directory.
		_OPEN_MAX_=sysconf(_SC_OPEN_MAX);	//	The file'maxinun process could to open.

		//	Get memory.
	
		Work_Home=new char[_PATH_MAX_];
		if (! Work_Home)
		{
			State_Of_Initialization_FID=false;
			syslog(LOG(LOG_ERR),"FTPR: Can not get mem for work path buffer.");	
		}
		else
			strncpy(Work_Home,New_Work_Home,_PATH_MAX_);
		
		Root_Home=new char[_PATH_MAX_];
		if (! Root_Home)
		{	
			delete[] Work_Home;
			State_Of_Initialization_FID=false;
			syslog(LOG(LOG_ERR),"FTPR: Can not get mem for root path buffer.");
		}
		else
			strncpy(Root_Home,New_Work_Home,_PATH_MAX_);	// Root home always is work home at the time of building.

		// Try to change work home.

		if (_CHDIR_(Work_Home))	// _CHDIR_ Will copy new path to work home.
		{
			syslog(LOG(LOG_INFO),"FTPR: Changed work home.");
		}
		else
		{
			delete[] Work_Home;
			delete[] Root_Home;
			State_Of_Initialization_FID=false;	// Error.
			syslog(LOG(LOG_ERR),"FTPR: Can not change work home.");
		}


		if (State_Of_Initialization_FID)
		{	
			FDES_OPENED=new int[_OPEN_MAX_];
			if (! FDES_OPENED)
			{
				delete[] Work_Home;
				delete[] Root_Home;	
				State_Of_Initialization_FID=false;
				syslog(LOG(LOG_ERR),"FTPR: Can not get mem for file descriptor array.");
			}
			else;
		}
		else;

	}

	FID_class::~FID_class()	//	Clear method.
	{

		if (State_Of_Initialization_FID)	
		{
		/*	Recycle resource.	*/
			
			if (Work_Home)
			{
				delete[] Work_Home;
				syslog(LOG(LOG_INFO),"FTPR: Work directory path deleted.");
			}
			else;	
			
			if (Root_Home)
			{	
				syslog(LOG(LOG_INFO),"FTPR: Root directory path deleted.");
				delete[] Root_Home;
			}
			else;

			if (FDES_OPENED)
			{
				for (int Index=0; Index < Current_Length_FDES; ++Index)
					(void)_CLOSE_(FDES_OPENED[Index]);	// Close files them had been opened.
				delete[] FDES_OPENED;	// Delete array.
				syslog(LOG(LOG_INFO),"FTPR: FDES Deleted.");
			}
			else;

			if (Directory)
			{
				_CLOSE_DIR_();
				syslog(LOG(LOG_INFO),"FTPR: Close directory.");
			}
			else;

			if (List_Head)
			{
				syslog(LOG(LOG_INFO),"FTPR: Clear linked-list.");
				_CLEAR_LIST_(List_Head);	
			}
			else;

			_CLOSE_(FID_TempFile);	// Closing tempfile.

			syslog(LOG(LOG_NOTICE),"FTPR: Destroied file class.");
		}
		else
			syslog(LOG(LOG_ERR),"FTPR: Failed to delete File class because it had not created.");
		
	}

	int FID_class::_OPEN_(const char* FILENAME,int FLAG,...)
	{

		int Tempolarly_Fdes=-1;	// Temp variable.
		FLAG|=O_NOCTTY;	// If pathname link to a tty file,don't assignment it as control tty for this process.

		if (FLAG & O_CREAT)
		{
			//	T -> Have flag O_CREAT.
			va_list MODE;	//	va_list object.
			va_start(MODE,FLAG);	//	Initialize.
			mode_t File_Mode=va_arg(MODE,mode_t);	//	Get mode
			va_end(MODE);	//	Recycle va_list object.
			
			Tempolarly_Fdes=open(FILENAME,FLAG|O_NOCTTY,File_Mode);	// Open file.And when process open a tty file,do not assignment control tty.
			if (Tempolarly_Fdes < 0)
				return -1;
			else
				FDES_OPENED[Current_Length_FDES++]=Tempolarly_Fdes;	// Save file descriptor which had just to opened.	
		}
		else
		{
			// Doesn't to create file.
			Tempolarly_Fdes=open(FILENAME,FLAG);	// Open file.
			if (Tempolarly_Fdes < 0)
				return -1;
			else
				FDES_OPENED[Current_Length_FDES++]=Tempolarly_Fdes;			
		}
	}

	//	File control

	inline void FID_class::_CLOSE_(const int CLOSE_DES)
	{
		(void)close(CLOSE_DES);	//	close file.
	}

	inline void FID_class::_UNLINK_(const char *UNLINK_FILE)
	{
		unlink(UNLINK_FILE);	//	unlink file.
	}

	int FID_class::_MKSTEMP_(char *TEMPFILE)
	{
		int TempFile=mkstemp(TEMPFILE);	//	mkstemp would open the tempfile and return the descriptor is own its.
		if (TempFile > 0)
			FID_TempFile=TempFile;
		else
			FID_TempFile=-1;

		return TempFile;
	}

	inline int FID_class::_LSEEK_(const int SEEK_DES,const off_t NEW_POS,const int WHENCE)
	{
		return lseek(SEEK_DES,NEW_POS,WHENCE);	//	Seek position.
	}

	inline int FID_class::_LINK_(const char *OLD,const char *NEW)
	{
		return link(OLD,NEW);	//	Create hard link.
	}

	inline int FID_class::_FTRUNCATE_(const int FDES,const off_t LENGTH)
	{
		return ftruncate(FDES,LENGTH);	//	Set file length.
	}	

	struct stat FID_class::_LSTAT_(const char *FILENAME)
	{
		struct stat Stat_BUF;	//	Create a auto variable of struct stat.
		if (lstat(FILENAME,&Stat_BUF) < 0)
		{
			Stat_BUF.st_size=0;
			syslog(LOG(LOG_NOTICE),"FTPR: The file to open is possible not be existed - %s.",FILENAME);
		}
		else
		{
			//	exclude the symboil link.
			if ((Stat_BUF.st_mode & S_IFMT) == S_IFLNK)
			{
				Stat_BUF.st_size=0;
				syslog(LOG(LOG_ERR),"FTPR: Try to open a symbol link - %s.",FILENAME);
			}
			else;
		}

		return Stat_BUF;
	}

	inline int FID_class::_RENAME_(const char *OLD_NAME,const char *NEW_NAME)
	{
		return rename(OLD_NAME,NEW_NAME);	// File rename.
	}


	//	Read from file.
	inline ssize_t FID_class::_READ_FILE_(const int FDES,void *BUFF,const size_t LEN)
	{
		return read(FDES,BUFF,LEN);
	}

	//	Write to file.
	inline ssize_t FID_class::_WRITE_FILE_(const int FDES,const void *BUFF,const size_t LEN)
	{
		return write(FDES,BUFF,LEN);
	}

	//	File lock
	
	int FID_class::_LOCK_FILE_(const int LOCKFDES,const int CMD,const struct flock *LOCK)
	{
		//	Lock file or unlock file.

		switch (CMD)
		{
			case F_GETLK:
			//	Get lock infomation.

				return fcntl(LOCKFDES,F_GETLK,LOCK);

			case F_SETLK:
			//	Don't block.
				return fcntl(LOCKFDES,F_SETLK,LOCK);

			case F_SETLKW:
			//	F_SETLKW is a block operation.
				return fcntl(LOCKFDES,F_SETLKW,LOCK);
				

			default:
				syslog(LOG(LOG_ERR),"FTPR: Lock type none defined.");
				return -2;	// Differentiate to return value.
		}
	}

	//	Directory
	
	bool FID_class::_OPEN_DIR_(const char* DIRPATH)
	{
		//	Open directory.
		Directory=opendir(DIRPATH);	
		return Directory?true:false;	
	}

	inline struct dirent* FID_class::_READ_DIR_(void)
	{
		return readdir(Directory);	//	Return static memory.
	}

	inline void FID_class::_REWIND_DIR_(void)
	{
		rewinddir(Directory);	//	Reset dir position.
	}

	inline void FID_class::_CLOSE_DIR_(void)
	{
		if (Directory)
			if (0 == closedir(Directory))	//	Close directory.
				Directory=NULL;
			else
				syslog(LOG(LOG_ERR),"FTPR: Close directory was fault.");
			
		else;
	}
	
	bool FID_class::_CHDIR_(const char *New_Directory)
	{
		if (chdir(New_Directory) < 0)
			return false;
		else
		{
			
			//	Get new directory length.
			size_t New_Dir_Len=strlen(New_Directory);
		
			_Copy_String_(Work_Home,New_Directory,New_Dir_Len);

			_CLEAR_LIST_(List_Head);		//	Recycle the older list.

			Current_Length_List=_MAKE_DIR_LIST_();	//	When process entry a new dir,then create a new directory items list.
			
			if (-1 == Current_Length_List)
			{
				syslog(LOG(LOG_ERR),"FTPR: Creating new list was fault.");	// Log.
				return false;
			}
			else
			{
				syslog(LOG(LOG_INFO),"FTPR: Created list,length is %d",Current_Length_List);	// Log.
				return true;
			}
		}
	}

	void FID_class::_GETCWD_(char *Buffer,const size_t Buffer_Len)
	{
		const int The_Finally_Len=(Buffer_Len < _PATH_MAX_)?Buffer_Len:_PATH_MAX_;	// Between two for value take the less.

		_Copy_String_(Buffer,Work_Home,The_Finally_Len);
		
	}

	int FID_class::_MAKE_DIR_LIST_(void)
	{

		if (Directory)		// At fist to close dir.
		{
			_CLOSE_DIR_();
		}
		else;

		if (! _OPEN_DIR_("."))	// Open dir failed.
			return -1;
		else;

		struct dirent* DIR_BUF(NULL);	//	Temp pointer.
		List_Head=new DIL;	//	Get memory for head.
		if (List_Head)
		{
			List_Head->Next=NULL;	//	Next is NULL in initalization.
			List_Head->Item_Name=NULL;
		}
		else
			return -1;

		int Links(0);	//	Counter.
		size_t D_NAME_LEN(0);	//	d_name length.

		DIL *Temp_ptr(List_Head);

		while ((DIR_BUF=_READ_DIR_()))
		{
			if (*DIR_BUF->d_name == '.' && (*(DIR_BUF->d_name+1) == '.' || *(DIR_BUF->d_name+1) == '\0'))	// Jump across to . and .. ;
				continue;
			else
			{
			
				//	Get string length.
				D_NAME_LEN=strlen(DIR_BUF->d_name);

				Temp_ptr->Item_Name=new char[_NAME_MAX_];	//	Get memory for name array.

				if (Temp_ptr->Item_Name)
				{

					_Copy_String_(Temp_ptr->Item_Name,DIR_BUF->d_name,D_NAME_LEN);
					//	Copy item name.				

					++Links;
				}
				else
					return Links;	//	Had been linked number.

				Temp_ptr->Next=new DIL;	//	New node.
				if (Temp_ptr->Next)
				{
					Temp_ptr=Temp_ptr->Next;	//	To new node.
				
					//	Set pointer to NULL.
					Temp_ptr->Next=NULL;
					Temp_ptr->Item_Name=NULL;
				}
				else
					return Links;

			}
		}

		_CLOSE_DIR_();	//	Close directory.

		return Links;
	}

	void FID_class::_TRAVERSE_(char *DNAME)
	{
		static DIL* Next_Pos(List_Head);	//	Head start.
		static short int Node_Num(0);

		if (Next_Pos && Node_Num < Current_Length_List)	//	Otherwise is a null list or list around.
		{
			//	Get length.
			size_t Name_Len=strlen(Next_Pos->Item_Name);

			_Copy_String_(DNAME,Next_Pos->Item_Name,Name_Len);

			Next_Pos=Next_Pos->Next;	//	Update,before next calling entry the next node.
			++Node_Num;			//	Next number.

		}
		else
		{
			Next_Pos=List_Head;	//	End to Head.
			Node_Num=0;		//	To head.
			*DNAME='\0';		//	NULL.
		}
	}

	void FID_class::_CLEAR_LIST_(DIL *NODE)
	{

		if (NODE)
		{
			_CLEAR_LIST_(NODE->Next);	//	Recursive.
			delete[] NODE->Item_Name;	//	Data in the list.
			delete NODE;	//	Delete Node.
		}
		else
			;
	}	

	short int FID_class::_WORKHOME_(char *Path_Array,const size_t Array_Len)
	{
		const size_t Length(Array_Len);		//	I don't know that why the variable 'Array_Len' had be changed in the assembly.
							//	So I declare a new variable to save that value to repeat copy.
		
		if (NULL != Path_Array)
		{
			_Copy_String_(Path_Array,Work_Home,Array_Len);
			return 0;
		}
		else
			return 1;
	}
}
