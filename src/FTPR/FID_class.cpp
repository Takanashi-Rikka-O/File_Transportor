//	FID_class.cpp
//	Version : 0.2.1
//	Date : Sat May 23 15:12:12 2020
//	Last revise : Sat Jul 17 11:03:? 2020
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
//		2> Adding remake list function.


#include"FID_class.h"
#include<iostream>


namespace FID{


	// Client must use this function init FID class.
	FID_class::FID_class():State_Of_Initialization_FID(true)
	{
		// Because _CHDIR_ will make a linked-list,but client need not use that.
		// This method for preparation of client to init FID.
		// Server never should not to use this function.
	
		/*	Initialize.	*/
		Work_Home=NULL;
		Root_Home=NULL;
		Directory=NULL;
		FDES_OPENED=NULL;
		List_Head=NULL;
		FID_TempFile=-1;
		Current_Length_List=-1;
		Current_Length_FDES=1;
		_OPEN_MAX_=-1;	
		/*	Get configre.	*/
		_PATH_MAX_=pathconf(".",_PC_PATH_MAX);	// Get path max.
		_NAME_MAX_=pathconf(".",_PC_NAME_MAX);	// Get name length limit, this value will limits the file name max length.
		
		/*	Set variables.	*/

		Work_Home=new char[_PATH_MAX_];	// For currently work path.
		if (NULL == Work_Home)
		{
			syslog(LOG(LOG_ERR),"FTPR_FID: Can not allocs memory for work home.");
			State_Of_Initialization_FID=false;
			return;
		}
		else;

		FDES_OPENED=new int[Current_Length_FDES];	// A pointer point to a int type mem.
		if (NULL == FDES_OPENED)
		{
			syslog(LOG(LOG_NOTICE),"FTPR_FID: Can not allocs memory for fdes.");
			State_Of_Initialization_FID=false;
			return;
		}
		else
			*FDES_OPENED=-1;	// Initialize.

		syslog(LOG(LOG_NOTICE),"FTPR_FID: Limits -");
		syslog(LOG(LOG_NOTICE),"FTPR_FID: Path max : %ld.",_PATH_MAX_);
		syslog(LOG(LOG_NOTICE),"FTPR_FID: Name max : %ld.",_NAME_MAX_);


	}

	
	FID_class::FID_class(const char *New_Work_Home)	//	Build method.
	{

		/*	Before change root path,have to change currently dir path to the target path.	*/
		/*	After change root path,then get currently path will return '/'.	*/
		
		/*	Memories from apply by this function all will be destroying at clear time.	*/


		//	State

		State_Of_Initialization_FID=true;	//	Suppose it's succeed.

		// Initialize.
		Directory=NULL;
		List_Head=NULL;
		Current_Length_List=0;
		Current_Length_FDES=0;
		Work_Home=NULL;
		Root_Home=NULL;
		FDES_OPENED=NULL;

		if (New_Work_Home)	// Parameter must is not NULL.
			;
		else
		{
			State_Of_Initialization_FID=false;
			return;
		}

		FID_TempFile=-1;	// This variable will save the tmpfile descriptor.

		//	Get os limits.

		_PATH_MAX_=pathconf(".",_PC_PATH_MAX);	//	Path name of current directory.
		_NAME_MAX_=pathconf(".",_PC_NAME_MAX);	//	File name max of current directory.
		_OPEN_MAX_=sysconf(_SC_OPEN_MAX);	//	The file'maxinun process could to open.

		//	Get memory.
	
		Work_Home=new char[_PATH_MAX_];
		if (! Work_Home)
		{
			State_Of_Initialization_FID=false;
			syslog(LOG(LOG_ERR),"FTPR_FID: Can not get mem for work path buffer.");	
			return;
		}
		else
		{
			// If succeed to get work home memory,will invoke chdir to change directory path at first.
			_Copy_String_(Work_Home,New_Work_Home,strlen(New_Work_Home));
			if (! _CHDIR_(Work_Home))	// This function will keep a linked-list in memory.
			{
				// Failed to change directory path.
				syslog(LOG(LOG_ERR),"FTPR_FID: Can not change directory.");
				State_Of_Initialization_FID=false;
				return;
			}
			else;
		}

		
		Root_Home=new char[_PATH_MAX_];
		if (! Root_Home)
		{	
			State_Of_Initialization_FID=false;
			syslog(LOG(LOG_ERR),"FTPR_FID: Can not get mem for root path buffer.");
			return;
		}
		else
		{
			// If succeed to get root home memory,will invoke chroot to change root path at first.
			_Copy_String_(Root_Home,Work_Home,strlen(Work_Home));
			if (chroot(Root_Home) < 0)
			{
				State_Of_Initialization_FID=false;
				syslog(LOG(LOG_ERR),"FTPR_FID: Failed to change root path.");
				return;
			}
			else;
		}


		FDES_OPENED=new int[_OPEN_MAX_];
		if (! FDES_OPENED)
		{
			State_Of_Initialization_FID=false;
			syslog(LOG(LOG_ERR),"FTPR_FID: Can not get mem for file descriptor array.");
		}
		else;

		syslog(LOG(LOG_NOTICE),"FTPR_FID: Limits -");
		syslog(LOG(LOG_NOTICE),"FTPR_FID: Path max : %ld.",_PATH_MAX_);
		syslog(LOG(LOG_NOTICE),"FTPR_FID: Name max : %ld.",_NAME_MAX_);
		syslog(LOG(LOG_NOTICE),"FTPR_FID: Open max : %ld.",_OPEN_MAX_);

	}

	FID_class::~FID_class()	//	Clear method.
	{

		/*	Recycle resource.	*/
			
		if (Work_Home)
		{
			delete[] Work_Home;
			syslog(LOG(LOG_INFO),"FTPR_FID: Work directory path deleted.");
		}
		else;	
		
		if (Root_Home)
		{	
			delete[] Root_Home;
			syslog(LOG(LOG_INFO),"FTPR_FID: Root directory path deleted.");
		}
		else;

		if (FDES_OPENED)
		{
			for (int Index=0; Index < Current_Length_FDES; ++Index)
				(void)_CLOSE_(FDES_OPENED[Index]);	// Close files them had been opened.
			delete[] FDES_OPENED;	// Delete array.
			syslog(LOG(LOG_INFO),"FTPR_FID: FDES Deleted.");
		}
		else;

		if (Directory)
		{
			_CLOSE_DIR_();
			syslog(LOG(LOG_INFO),"FTPR_FID: Close directory.");
		}
		else;

		if (List_Head)
		{
			syslog(LOG(LOG_INFO),"FTPR_FID: Clear linked-list.");
			_CLEAR_LIST_(List_Head);
			List_Head=NULL;	
		}
		else;

		_CLOSE_(FID_TempFile);	// Closing tempfile.

		syslog(LOG(LOG_NOTICE),"FTPR_FID: Destroied file&directory interface.");

		
	}

	int FID_class::_OPEN_(const char* FILENAME,int FLAG,...)
	{

		if (strlen(FILENAME) > _NAME_MAX_)
		{
			syslog(LOG(LOG_ERR),"FTPR_FID: File name too long.");
			return -1;
		}
		else;



		int Temporaryly_Fdes=-1;	// Temp variable.
		FLAG|=O_NOCTTY;	// If pathname link to a tty file,don't assignment it as control tty for this process.

		if (FLAG & O_CREAT)
		{
			//	T -> Have flag O_CREAT.
			va_list MODE;	//	va_list object.
			va_start(MODE,FLAG);	//	Initialize.
			mode_t File_Mode=va_arg(MODE,mode_t);	//	Get mode
			va_end(MODE);	//	Recycle va_list object.
			
			Temporaryly_Fdes=open(FILENAME,FLAG|O_NOCTTY,File_Mode); // Open file.And when process open a tty file,do not assignment control tty.
			if (Temporaryly_Fdes < 0)
				return -1;
			else
				FDES_OPENED[Current_Length_FDES++]=Temporaryly_Fdes;	// Save file descriptor which had just to opened.	
		}
		else
		{
			// Doesn't to create file.
			Temporaryly_Fdes=open(FILENAME,FLAG);	// Open file.
			if (Temporaryly_Fdes < 0)
				return -1;
			else
				FDES_OPENED[Current_Length_FDES++]=Temporaryly_Fdes;			
		}
		return Temporaryly_Fdes;
	}

	//	File control

	void FID_class::_CLOSE_(const int CLOSE_DES)
	{
		(void)close(CLOSE_DES);	//	close file.
	}

	void FID_class::_UNLINK_(const char *UNLINK_FILE)
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

	int FID_class::_LSEEK_(const int SEEK_DES,const off_t NEW_POS,const int WHENCE)
	{
		return lseek(SEEK_DES,NEW_POS,WHENCE);	//	Seek position.
	}

	int FID_class::_LINK_(const char *OLD,const char *NEW)
	{
		return link(OLD,NEW);	//	Create hard link.
	}

	int FID_class::_FTRUNCATE_(const int FDES,const off_t LENGTH)
	{
		return ftruncate(FDES,LENGTH);	//	Set file length.
	}	

	struct stat FID_class::_LSTAT_(const char *FILENAME)
	{
		struct stat Stat_BUF;	//	Create a auto variable of struct stat.
		// File name must less than _NAME_MAX_
		if (strlen(FILENAME) > _NAME_MAX_)
		{
			syslog(LOG(LOG_ERR),"FTPR_FID: File name too long.");
			Stat_BUF.st_size=0;
			return Stat_BUF;
		}
		else;

		if (lstat(FILENAME,&Stat_BUF) < 0)
		{
			Stat_BUF.st_size=0;
			syslog(LOG(LOG_NOTICE),"FTPR_FID: The file to open is possible not be existed - %s.",FILENAME);
		}
		else
		{
			//	exclude the symboil link.
			if ((Stat_BUF.st_mode & S_IFMT) == S_IFLNK)
			{
				Stat_BUF.st_size=0;
				syslog(LOG(LOG_ERR),"FTPR_FID: Try to open a symbol link - %s.",FILENAME);
			}
			else;
		}

		return Stat_BUF;
	}

	int FID_class::_RENAME_(const char *OLD_NAME,const char *NEW_NAME)
	{
		return rename(OLD_NAME,NEW_NAME);	// File rename.
	}


	//	Read from file.
	ssize_t FID_class::_READ_FILE_(const int FDES,void *BUFF,const size_t LEN)
	{
		return read(FDES,BUFF,LEN);
	}

	//	Write to file.
	ssize_t FID_class::_WRITE_FILE_(const int FDES,const void *BUFF,const size_t LEN)
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
				syslog(LOG(LOG_ERR),"FTPR_FID: Lock type none defined.");
				return -2;	// Differentiate to return value.
		}
	}

	//	Directory
	
	bool FID_class::_OPEN_DIR_(const char* DIRPATH)
	{
		if (strlen(DIRPATH) > _NAME_MAX_)
		{
			syslog(LOG(LOG_ERR),"FTPR_FID: Directory name too long.");
			return false;
		}
		else;

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
				syslog(LOG(LOG_ERR),"FTPR_FID: Close directory was fault.");	
		else;
	}
	
	bool FID_class::_CHDIR_(const char *New_Directory)
	{

		if (strlen(New_Directory) > _PATH_MAX_)
		{
			syslog(LOG(LOG_ERR),"FTPR_FID: New directory path length too long.");
			return false;
		}
		else;

		if (chdir(New_Directory) < 0)
			return false;
		else
		{
			
			//	Get new directory length.
			size_t New_Dir_Len=strlen(New_Directory);
		
			_Copy_String_(Work_Home,New_Directory,New_Dir_Len);

			_CLEAR_LIST_(List_Head);		//	Recycle the older list.
			List_Head=NULL;	// Comply with constraction to reset this pointer.

			Current_Length_List=_MAKE_DIR_LIST_();	//	When process entry a new dir,then create a new directory items list.
			
			if (-1 == Current_Length_List)
			{
				syslog(LOG(LOG_ERR),"FTPR_FID: Creating new list was fault.");	// Log.
				return false;
			}
			else
			{
				syslog(LOG(LOG_INFO),"FTPR_FID: Created list,length is %d",Current_Length_List);	// Log.
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

	void FID_class::_REMAKE_LIST_()
	{

		/*	Nothings need by this function.	*/

		//	Env

		/*	Check if had create list.	*/
		if (List_Head)
		{
			_CLEAR_LIST_(List_Head);
			List_Head=NULL;
		}
		else;

		if (0 == (Current_Length_List=_MAKE_DIR_LIST_()))
			syslog(LOG(LOG_ERR),"FTPR_FID: Failed to remake linked-list for items.");
		else
			syslog(LOG(LOG_ERR),"FTPR_FID: Remade linked-list.");

		//	Primary


		//	Return

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
			--Current_Length_List;	// Subtract the length of list on parent.
		}
		else;
	}	

	short int FID_class::_WORKHOME_(char *Path_Array,const size_t Array_Len)
	{
		
		if (NULL != Path_Array)
		{
			_Copy_String_(Path_Array,Work_Home,Array_Len);
			return 0;
		}
		else
			return 1;
	}
}
