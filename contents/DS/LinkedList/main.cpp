#include <iostream>
#include "LinkedList.h"

using namespace std;

int main()
{
	LinkedList<int> LL;
	for(int i=1; i<11; i++)
		LL.AddAtHead(i);
	LL.Print(); // (10,9,8,7,6,5,4,3,2,1)
	
	cout << LL.Get(2) << endl; // 8
	
	LL.AddAtIndex(2,8);
	LL.AddAtIndex(12, 9);
	LL.Print(); // (10,9,8,8,7,6,5,4,3,2,1)
	
	LL.DeleteAtIndex(3);
	LL.DeleteAtIndex(13);
	LL.Print(); // (10,9,8,7,6,5,4,3,2,1)
	
	LL.DeleteValue(9);
	LL.DeleteValue(12);
	LL.Print(); // (10,8,7,6,5,4,3,2,1)

	LL.MoveToHead(2);
	LL.MoveToHead(23);
	LL.Print(); // (2,10,8,7,6,5,4,3,1)

	//LL.Rotate(0);
	//LL.Print();

	//LL.Rotate(-1);
	//LL.Print();
	
	LL.Rotate(2);
	LL.Print(); // (3,1,2,10,8,7,6,5,4)
	
	LL.AddAtHead(4);
	LL.Print(); // (4,3,1,2,10,8,7,6,5,4)

	LL.Reduce();
	LL.Print(); // (4,3,1,2,10,8,7,6,5)
	
	LL.AddAtIndex(7,10); 
	LL.Print(); // (4,3,1,2,10,8,7,10,6,5)

	LL.K_Reverse(3);
	LL.Print(); // (1,3,4,8,10,2,6,10,7,5)
	cout<<LL.Size()<< endl; // 10

	LL.AddAtHead(-1);
	LL.AddAtHead(-6);
	LL.AddAtHead(-7);
	LL.AddAtHead(-4);
	LL.Print();
	LL.EvenOddSeparateSort();	
	LL.Print(); // (2,4,6,8,10,10,7,5,3,1)
	cout<<LL.Size()<< endl; // 10

	LL.CleanUp(); 
	cout<<LL.Size()<<endl; // 0

	cout << "One list test---------------------" << endl;
	LinkedList<int> OL;
	OL.AddAtHead(19);
	OL.AddAtHead(18);
	OL.Print();
	OL.DeleteValue(18);
	OL.Print();
	cout << "-----------------------------" << endl;

	cout << "Deleting test--------------------" << endl;
	LinkedList<int> SL;
	SL.AddAtHead(55);
	SL.AddAtHead(44);
	SL.AddAtHead(22);
	SL.AddAtIndex(3, 11);
	SL.AddAtIndex(0, 33);
	SL.Print();
	SL.DeleteAtIndex(2);
	SL.Print();
	SL.DeleteValue(44);
	SL.Print();
	cout << "-----------------------------" << endl;

	cout << "Reduce Test-------------------------" << endl;
	LinkedList<int> RT;
	RT.AddAtHead(11);
	RT.AddAtHead(11);
	RT.AddAtHead(13);
	RT.AddAtHead(13);
	RT.AddAtHead(11);
	RT.Print();
	RT.Reduce();
	RT.Print();
	cout << "-----------------------------------" << endl;
}