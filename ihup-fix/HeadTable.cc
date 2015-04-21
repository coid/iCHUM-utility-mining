#include<iostream>
using namespace std;

#include "HeadTable.h"
#include "pardhp.h"

HeadTable::HeadTable(const double *d){
    int i;
	HeadNode = new item_2[maxitem];
	for (i = 0; i<maxitem; i++) {
		    HeadNode[i].item2 = i;//��������Ʒ���
                    HeadNode[i].t_utility = d[i];//��������Ʒ��TWUֵ
                    HeadNode[i].tf =0;//���ó�ʼ��tf����
                    HeadNode[i].linknode = NULL;//����ָ��IHUP�ڵ��link
                }
	}




HeadTable::~HeadTable(){
	clear();
}

int compare(const void *a, const void*b){//�Ӵ�С����
	item_2 arg1 = *reinterpret_cast<const item_2*>(a);
	item_2 arg2 = *reinterpret_cast<const item_2*>(b);
	if (arg1.t_utility < arg2.t_utility) { return 1; }
	if (arg1.t_utility == arg2.t_utility) { return 0; }
	if (arg1.t_utility > arg2.t_utility) { return -1; }
}

void HeadTable::TWU_sort(){
	qsort(HeadNode, maxitem, sizeof(item_2), compare);
}

void HeadTable::clear(){
	delete[] HeadNode;
	//size = 0;
}
