#include<iostream>
#include<algorithm>
#include<cstdlib>
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
    HeadNode = (item_2 *)calloc(size, sizeof(item_2));
    for (i = 0; i<maxitem; i++) {
        if(d[i] >= MIN_UTILITY){
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
                        HeadNode[count].tf =0;//设置初始的tf次数
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

void HeadTable::swap_ht(int m, int n){
   
//    item_2* tmp = new item_2;
//    tmp->item2 = HeadNode[m].item2;
//    tmp->t_utility = HeadNode[m].t_utility;
//    tmp->tf = HeadNode[m].tf;
//    tmp->linknode = HeadNode[m].linknode;
//    
//    HeadNode[m].item2 = HeadNode[n].item2;
//    HeadNode[m].t_utility = HeadNode[n].t_utility;
//    HeadNode[m].tf = HeadNode[n].tf;
//    HeadNode[m].linknode = HeadNode[n].linknode;
//    
//    HeadNode[n].item2 = tmp->item2;
//    HeadNode[n].t_utility = tmp->t_utility;
//    HeadNode[n].tf = tmp->tf;
//    HeadNode[n].linknode = tmp->linknode;
//    
    //delete tmp;
    
    std::swap(HeadNode[m],HeadNode[n]);
    
}

void HeadTable::resize(int c){
    if(c > 0){
        cout<<"we resize headtable."<<endl;
        int newsize = size+c;
        HeadNode = (item_2*) realloc(HeadNode, newsize*sizeof(item_2));
        size = newsize;
    }
}
