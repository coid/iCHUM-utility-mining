#ifndef __HEADTABLE_H
#define __HEADTABLE_H

#include<iostream>
using namespace std;

#include "pardhp.h"

class HeadTable{
public:
	HeadTable(const double *d);
	HeadTable(const double *d, int c);
        
	~HeadTable();

	
	void TWU_sort();
	void clear();
	void resize(int c);//TODO:ΪHeadTable����
        void swap_ht(int m, int n);//TODO:���������ڵ�λ��
        item_2* HeadNode;
        int size;
	
};

#endif //__HEADTABLE_H
