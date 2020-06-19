//	_Copy_String_.h
//	Version : 0.1
//	Date : Mon Jun  8 12:36:39 2020
//	Last revise : Mon Jun  8 12:36:39 2020
//	Symbol-Constraint :
//			_Xx..._ - Function Symbol
//			Xx...|x|X - Variable Symbol
//	Description :
//		This is the header of function '_Copy_String_'.It is an assembly function,processing apply to the SystemV ABI.
//	Functions :
//		void _Copy_String_(char *des,const char *src,const size_t Copy_Size);
//	Header :

#ifndef _COPY_STRING__H_
#define _COPY_STRING__H_

#if defined __ISOC__

	void _Copy_String_(char *,const char *,const size_t);	/*	ISO C Program.	*/

#else
	extern "C"{
		void _Copy_String_(char *,const char *,const size_t);	// ISO C++ Program.	*/
	}

#endif

#endif
