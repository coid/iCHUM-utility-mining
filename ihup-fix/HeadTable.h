#ifndef __HEADTABLE_H
#define __HEADTABLE_H

#include<iostream>
using namespace std;

#include "pardhp.h"

class HeadTable{
public:
	HeadTable(const double *d);
	//HeadTable(const double *d, int c);
	~HeadTable();

	
	void TWU_sort();
	void clear();
	void merge(HeadTable ht);//TODO:合并两个HeadTable
        
        item_2* HeadNode;
        //int size;
	
};

#endif //__HEADTABLE_H
