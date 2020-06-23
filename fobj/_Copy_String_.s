#	_Copy_String_.s
#	Version : 0.2
#	Date : Sun Jun  7 19:27:31 2020
#	Last revise : Sun Jun  7 19:27:31 2020
#	Symbol-Constraint :
#			_Xx..._ - Function Symbol
#			Xx... - Normaly Symbol
#	Description :
#		An assembly function _Copy_String.
#		Use the C style like this : void _Copy_String_(char *Destination,const char *Source,const size_t Copy_Size);
#		This assembly was written for optimized processing speed up.Call method like C functions.And stack restore would be responed by caller.
#		Function use 64bit push/pop,it's mean that 8B/unit.And function have nothing local variables so it only have the segement .text ; But in the
#	some time,buffer length is longer than Assignment to copy,so must append a '\0' to the end.
#	Functions :
#		void _Copy_String_(char *Destination,const char *Source,const size_t Copy_Size);
#		While parameters are passing,Des would be sending to RDI,Src would be sending to RSI,Copy_Size would be sending to RDX.
#	
#	Header :
#		None.
#
#	Fixs :
#		> Append a null byte to end.

.text
.code64
.global _Copy_String_

#	Function definition.

	.type _Copy_String_,@function	# AT&T Function declare.
	_Copy_String_:
		
		#	Function verbose -
		#	Return : void ;
		#	Arguments : Two character pointers for source and destination ; A number for copy size ;
		#		Arguments use C function constraintion : R->L stack ;
		#	Registers would be using : RSI,RDI,RDX,RCX,RBP ;
		#	Because currently system ABI is SystemV ABI x86-64,so the parameters passing use RDI,RSI,RDX,RCX,R8,R9 (L->R).Now don't use stack to
		# get the number for three of parameters as well.

		_Env_Set_Of_Copy_String_:

			# Do not use any stack space to save arguments,gcc/g++ would use x86-64 ABI constraint to send parameters.
	
			# NO.1->RDI,NO.2->RSI,NO.3->RDX ;

			push	%RCX		# Save RCX.
			push	%RAX

			movq	%RDX,%RCX
			movb	$0x00,(%RDI,%RCX,0x01)	# End zero
			movq	$0x00,%RDX	# Index
			
		_Work_Of_Copy_String_:

			_Copy_Cycle_:

				cmp	$0x00,(%RSI,%RDX,0x01)

				movb	(%RSI,%RDX,0x01),%AL	# Move to al.
				movb	%AL,(%RDI,%RDX,0x01)	# Move to des.

				je	_Return_To_Caller_	# A null byte.

				incq	%RDX	# Index++
					
				loop	_Copy_Cycle_


		_Return_To_Caller_:


			pop	%RAX	# Restory RAX.
			pop	%RCX	# Restore RCX.
	
			ret	# Return.

#	Function defintion over.
