#include<iostream>
using namespace std;

#include "HeadTable.h"
#include "pardhp.h"

HeadTable::HeadTable(const double *d){
	int count = 0;
	int i;
	for (i = 0; i < maxitem; i++){
		if (d[i] >= MIN_UTILITY){
			count++;
                        //cout<<"large item no is "<<i<<endl;
		}
	}
	size = count;
	
	count = 0;
	HeadNode = new item_2[size];
	for (i = 0; i<maxitem; i++) {
		if (d[i] >= MIN_UTILITY){
                    HeadNode[count].item2 = i;//设置其物品编号
                    HeadNode[count].t_utility = d[i];//设置其物品的TWU值
                    HeadNode[count].tf =0;//设置初始的tf次数
                    HeadNode[count].linknode = NULL;//设置指向IHUP节点的link
                    count++;
 		}
	}
	
}

HeadTable::HeadTable(const double *d,int c){
        int count = 0;
	size = c;
	HeadNode = (item_2 *)calloc(size, sizeof(item_2));
	for (int i = 0; i < maxitem; i++){
		if (d[i] >= MIN_UTILITY){
			HeadNode[count].item2 = i;//设置其物品编号
			HeadNode[count].t_utility = d[i];//设置其物品的TWU值
                        HeadNode[count].linknode = NULL;//设置指向IHUP节点的link
			count++;
		}
	}
}

HeadTable::~HeadTable(){
	clear();
}

int compare(const void *a, const void*b){//从大到小排列
	item_2 arg1 = *reinterpret_cast<const item_2*>(a);
	item_2 arg2 = *reinterpret_cast<const item_2*>(b);
	if (arg1.t_utility < arg2.t_utility) { return 1; }
	if (arg1.t_utility == arg2.t_utility) { return 0; }
	if (arg1.t_utility > arg2.t_utility) { return -1; }
}

void HeadTable::TWU_sort(){
	qsort(HeadNode, size, sizeof(item_2), compare);
}

void HeadTable::clear(){
	delete[] HeadNode;
	size = 0;
}
