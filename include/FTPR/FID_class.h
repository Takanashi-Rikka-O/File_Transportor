//	FID_class.h
//	Version : 0.1
//	Date : Sat May 23 15:14:05 2020
//	Last revise : Sat May 23 15:14:05 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This header define FID_class.
//		FID_class contained the file control and directory control and file lock interfaces.
//
//	Functions :
//
//		Some file and directory and lock operation functions.
//
//	Header :
//
//		<fcntl.h>		//	File control.
//		<unistd.h>		//	UNIX standard functions.
//		<sys/stat.h>		//	Stat.
//		<sys/types.h>		//	System types.
//		<dirent.h>		//	Directory operation.
//		<cstdbool>		//	ISO C bool type.
//		<cstdarg>		//	ISO C args.

#ifndef _FID_CLASS_H_
#define _FID_CLASS_H_




#include<sys/stat.h>
#include<limits.h>
#include<dirent.h>
#include<cstdio>
#include<cstdarg>

#include"Share_Headers.h"

namespace FID{

	typedef struct DIR_Item_List{

		char* Item_Name;	//	Item name.
		struct DIR_Item_List* Next;	//	Next node.

	}DIL;


	class FID_class{

		private:

			/*	Environment.	*/

			char* Work_Home;	//	Current work home.
			char* Root_Home;	//	Current root home.

			long int _PATH_MAX_;	//	Path name max.
			long int _NAME_MAX_;	//	File name max.
			long int _OPEN_MAX_;	//	File open max.

			/*	Directory.	*/

			DIR* Directory;		//	Directory pointer.
			DIL* List_Head;		//	Item list.
			int Current_Length_List;	//	Record paresent list length.

			/*	File.	*/

			int* FDES_OPENED;	//	File descriptors array.
			int Current_Length_FDES;	// Length of FDES in parent.

			void _CLEAR_LIST_(DIL* NODE);	//	Clear list.
		public:


			FID_class();
			~FID_class();

			bool State_Of_Initialization_FID;	// The class object state in the meantime of initialization.

			int _Had_Opened_(void)
			{
				return Current_Length_FDES;	// Return length of file descriptors array.
			}

			int _Had_Linked_(void)
			{
				return Current_Length_List;	// Return length of directory items list.
			}


			//	File

			int _OPEN_(const char* FILENAME,int FLAG,...);	//	File open.
			void _CLOSE_(const int CLOSE_DES);			//	Close file.
			void _UNLINK_(const char* UNLINK_DES);			//	Unlink file.
			int _MKSTEMP_(char* TEMPFILE);			//	Create tempfile.
			int _LSEEK_(const int SEEK_DES,const off_t NEW_POS,const int WHENCE);	//	File seek.

			int _LINK_(const char* OLD,const char* NEW);	//	Link file.

			ssize_t _READ_FILE_(const int FDES,void* BUFF,const size_t LEN);	//	Read file.

			ssize_t _WRITE_FILE_(const int FDES,const void* BUFF,const size_t LEN);	//	Write to file.

			struct stat _LSTAT_(const char* FILENAME);	//	Get file information.

			int _RENAME_(const char *OLD_NAME,const char *NEW_NAME);	// File rename.

			int _FTRUNCATE_(const int FDES,const off_t LENGTH);	//	Set file length.	

			//	Lock
			
			int _LOCK_FILE_(const int LOCKFDES,const int CMD,const struct flock* LOCK);	//	Lock file.

			

			//	Directory

			bool _OPEN_DIR_(const char* DIRPATH);	//	Open directory.
			void _CLOSE_DIR_(void);	//	Close dir.	

			struct dirent* _READ_DIR_(void);
			//	Read dir.

			void _REWIND_DIR_(void);	//	Reset directory position.

			bool _CHDIR_(const char* New_Directory);	//	Into a new directory.

			void _GETCWD_(char* Buffer,const size_t Buffer_Len);


			int _MAKE_DIR_LIST_(void);	//	Make a directory list.

			void _TRAVERSE_(char* DNAME);	//	Traverse directory list.

			short int _WORKHOME_(char *Path_Array,const size_t Array_Len);

			short int _ROOTHOME_(char *Path_Array,const size_t Array_Len)
			{
				if (NULL != Path_Array)
				{
					_Copy_String_(Path_Array,Root_Home,Array_Len);
					return 0;
				}
				else
					return -1;
			}

                        int _Get_Latest_Open_File_(void)        // Use this function to get the latest opened.
                        {
                                return FDES_OPENED[Current_Length_FDES-1];
                        }

	};




}



#endif
