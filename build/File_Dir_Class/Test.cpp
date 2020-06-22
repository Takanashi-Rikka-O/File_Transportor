#include"FID_class.h"

#include<iostream>

using namespace std;
using namespace FID;

int main(void)
{
	FID_class Test(".");

	char* Item_Name=new char[125];
	if (Item_Name)
	{

		if (Test._CHDIR_(".."))
		{
			cout<<"Succeed to change directory."<<endl;
			if (-1 != Test._WORKHOME_(Item_Name,125))
				cout<<"Parent directory : "<<Item_Name<<endl;
			else
				cout<<"Get parent directory was fault!"<<endl;
			cout<<"List length = "<<Test._Had_Linked_()<<endl;
			while (1)
			{
				Test._TRAVERSE_(Item_Name);
				if (*Item_Name != '\0')
					cout<<"Item : "<<Item_Name<<endl;
				else
					break;
			}
		}
		else
			cout<<"Can not change directory!"<<endl;
	}
	else
		cerr<<"Can not get memory for Item_Name!\n";	//	error

	cerr<<"Process quti!"<<endl;
	delete[] Item_Name;
	
	cout<<"Delete array succeed"<<endl;
	
	return 0;
}
