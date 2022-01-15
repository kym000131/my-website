#include "FlatHash.h"
#include "HierarchyHash.h"
#include "NRKFlat.h"

#include <iostream>
#include <crtdbg.h>

int main()
{
	std::cout << "tombstone test" << std::endl;
	HierarchyHash ts(LINEAR_PROBING, 0.8);
	std::cout << ts.insert(98) << std::endl;//1
	std::cout << ts.insert(99) << std::endl; //1
	std::cout << ts.insert(1099) << std::endl; //2
	std::cout << ts.remove(99) << std::endl; //1
	std::cout << ts.getAllocatedSize() << std::endl; //100
	ts.print(); // 0:(98:98,99:1099)
 
	std::cout << "HierarchyHash test" << std::endl;
	HierarchyHash he(QUADRATIC_PROBING, 0.8);
	std::cout << he.insert(3) << std::endl;    // 1
	std::cout << he.insert(3) << std::endl;     // -1
	std::cout << he.insert(7) << std::endl;    // 1
	std::cout << he.insert(103) << std::endl;    // 1
	std::cout << he.insert(903) << std::endl;    // 1
	std::cout << he.insert(99) << std::endl;    // 1
	std::cout << he.insert(1099) << std::endl;    // 2
	std::cout << he.insert(98) << std::endl;    // 1
	std::cout << he.insert(1098) << std::endl; //3
	std::cout << he.insert(2098) << std::endl;    // 4
	std::cout << he.getNumofKeys() << std::endl; //9
	std::cout << he.search(100) << std::endl;    //-2
	he.print();

	std::cout << he.remove(1098) << std::endl;    // 3

	he.print();

	std::cout << "------" << std::endl;

	//std::cout << hh.remove(2098) << std::endl;    // 4 

	//hh.print();

	he.clearTombstones();

	std::cout << "------" << std::endl;

	he.print();

	std::cout << he.insert(1903) << std::endl;    // 
	std::cout << he.remove(903) << std::endl;    // 
	for (int i = 500; i < 1300; i++) {
		he.insert(i);
	}
	std::cout << he.getTableSize() << std::endl;      // 
	std::cout << he.getNumofKeys() << std::endl;      // 
	std::cout << he.getAllocatedSize() << std::endl;  // 1100
	//hh.~HierarchyHash();
	std::cout << std::endl;

	std::cout << "print test" << std::endl; // memory leaks
	HierarchyHash h1(LINEAR_PROBING, 0.8);
	std::cout << h1.insert(12) << std::endl;
	h1.print();
	std::cout << h1.remove(12) << std::endl;
	h1.print();
	std::cout << h1.getAllocatedSize();
	std::cout << std::endl;

	std::cout << "resizing test" << std::endl; // memory leaks
	HierarchyHash hs(LINEAR_PROBING, 0.002);
	std::cout << hs.insert(2) << std::endl;
	hs.print();
	std::cout << hs.insert(1002) << std::endl;
	hs.print();
	std::cout << hs.insert(2002) << std::endl;
	hs.print();
	std::cout << hs.insert(4002) << std::endl;
	hs.print();
	std::cout << hs.remove(1002) << std::endl;
	hs.print();
	std::cout << hs.getAllocatedSize() << std::endl;

	std::cout << "removing test" << std::endl;
	HierarchyHash hs2(LINEAR_PROBING, 0.9);
	std::cout << hs2.insert(98) << std::endl;
	std::cout << hs2.insert(1098) << std::endl;
	std::cout << hs2.insert(101) << std::endl;
	std::cout << hs2.insert(3098) << std::endl;
	std::cout << hs2.insert(99) << std::endl;
	std::cout << hs2.insert(100) << std::endl;
	std::cout << hs2.insert(2098) << std::endl;
	hs2.print();
	std::cout << hs2.remove(1098) << std::endl;
	hs2.print();
	std::cout << std::endl;

	std::cout << "removing test2" << std::endl;
	HierarchyHash hs3(LINEAR_PROBING, 0.9);
	std::cout << hs3.insert(1000) << std::endl;
	std::cout << hs3.insert(998) << std::endl;
	std::cout << hs3.insert(1) << std::endl;
	std::cout << hs3.insert(1998) << std::endl;
	std::cout << hs3.insert(2) << std::endl;
	std::cout << hs3.insert(2998) << std::endl;
	hs3.print();
	std::cout << hs3.remove(998) << std::endl;
	hs3.print();
	std::cout << std::endl;
	
	// ########################################
	// # FlatHash test example: Linear probing
	// ########################################
	std::cout << "FlatHash test: Linear" << std::endl;
	FlatHash fh(LINEAR_PROBING, 0.8);
	fh.print();
	std::cout << fh.insert(3)    << std::endl;    // 1
	std::cout << fh.insert(7)    << std::endl;    // 1
	std::cout << fh.insert(7)    << std::endl;    // -1
	std::cout << fh.insert(1003) << std::endl;    // 2
	std::cout << fh.insert(2006) << std::endl;    // 1
	std::cout << fh.insert(3006) << std::endl;    // 3
	fh.print();   // [3:3,4:1003,6:2006,7:7,8:3006] 
	std::cout << fh.insert(6)    << std::endl;    // 4
	std::cout << fh.remove(4003) << std::endl;    // -3
	std::cout << fh.insert(5003) << std::endl;    // 3
	fh.print();   // [3:3,4:1003,5:5003,6:2006,7:7,8:3006,9:6]
	//fh.~FlatHash();
	std::cout << std::endl;


	// ###########################################
	// # FlatHash test example: Quadratic probing
	// ###########################################
	std::cout << "FlatHash test: Quadratic" << std::endl;
	FlatHash fh1(QUADRATIC_PROBING, 0.8);
	std::cout << fh1.insert(1008) << std::endl;    // 1
	std::cout << fh1.insert(8009) << std::endl;    // 1
	std::cout << fh1.insert(5008) << std::endl;    // 3
	std::cout << fh1.insert(6008) << std::endl;    // 4
	std::cout << fh1.search(9008) << std::endl;    // -5
	std::cout << fh1.insert(4009) << std::endl;    // 2
	std::cout << fh1.insert(9)    << std::endl;    // 3
	fh1.print();    // [8:1008,9:8009,10:4009,12:5008,13:9,17:6008]

	FlatHash fh2(QUADRATIC_PROBING, 0.8);
	int a = 1;
	for(int i = 0; i < 157; i++){
		fh2.insert(a);
		a+=1000;
	}
	std::cout << fh2.insert(a+=1000) << std::endl;  // 247
	std::cout << fh2.insert(a+=1000) << std::endl;  // 249
	std::cout << fh2.insert(a+=1000) << std::endl;  // 1003
	std::cout << fh2.insert(a+=1000) << std::endl;  // 1004
	//fh1.~FlatHash();
	//fh2.~FlatHash();
	std::cout << std::endl;


	// #############################
	// # HierarchyHash test example
	// #############################
	std::cout << "HierarchyHash test" << std::endl;
	HierarchyHash hh(LINEAR_PROBING, 0.8);
	std::cout << hh.insert(3)    << std::endl;    // 1
	std::cout << hh.insert(7)    << std::endl;    // 1
	std::cout << hh.insert(103)  << std::endl;    // 1
	std::cout << hh.insert(903)  << std::endl;    // 1
	std::cout << hh.insert(99)   << std::endl;    // 1
	std::cout << hh.insert(1099) << std::endl;    // 2
	std::cout << hh.insert(98)   << std::endl;    // 1
	std::cout << hh.insert(1098) << std::endl;    // 4
	std::cout << hh.search(100)  << std::endl;    // -3
	hh.print();   // 0:[3:3,7:7,98:98,99:99]
	              // 1:[100:1099,101:1098,103:103]
	              // 9:[903:903]

	std::cout << hh.remove(1099) << std::endl;    // 2
	std::cout << hh.remove(2098) << std::endl;    // -4
	hh.print();   // 0:[3:3,7:7,98:98,99:99]
	              // 1:[100:1098,103:103]
	              // 9:[903:903]

	hh.clearTombstones();

	std::cout << hh.insert(1903) << std::endl;    // 2
	std::cout << hh.remove(903)  << std::endl;    // 1
	hh.print();
	std::cout << std::endl;

	hh.clearTombstones();

	for (int i = 500; i < 1300; i++) {
		hh.insert(i);
	}// memory leaks
	std::cout << hh.getTableSize() << std::endl;      // 2000
	std::cout << hh.getNumofKeys() << std::endl;      // 806
	std::cout << hh.getAllocatedSize() << std::endl;  // 1100
	//hh.~HierarchyHash();
	std::cout << std::endl;


	// #######################
	// # NRKFlat test example
	// #######################
	std::cout << "NRKFlat test" << std::endl;
	NRKFlat nf(LINEAR_PROBING, 0.8, 1000);
	std::cout << nf.insert(2019) << std::endl;    // 1
	std::cout << nf.search(3019) << std::endl;    // 0
	for (int i = 1; i < 800; i++){    // UPDATED 191111
		nf.insert(i);
	}
	std::cout << nf.insert(365) << std::endl;     // -2
	std::cout << nf.search(222) << std::endl;     // 2
	std::cout << nf.search(802) << std::endl;     // 0
	//nf.~NRKFlat();
	double pros = 0;
	for (unsigned int i = 801; i < 3000; i++) {
		if (nf.search(i) == 0) {
			pros++;
		}
	}
	double k = pros / (double)(3000 - 801);
	std::cout << k*100 << std::endl;//64.0746
	
	std::cout << nf.getTableSize() << std::endl;//2000
	std::cout << nf.getNumofKeys() << std::endl;//800

	std::cout << std::endl;

	std::cout << "done" << std::endl;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}
