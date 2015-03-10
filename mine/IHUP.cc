#include<iostream>
using namespace std;

#include "IHUP.h"
#include "HeadTable.h"
#include "pardhp.h"

#define SAFE_DELETE(p) {if(p){delete p; p=NULL;}}

IHUP::IHUP(HeadTable &ht){
//ָ������ָ��ĳ�ʼ��
//��ʼ��root����root���ӽڵ�
	root->isRoot = true;
	root->t_utility = -1;
	root->item1 = -1;
	root->parent = NULL;
        root->brother = NULL;
	//root->children(ht.size);
		
}

IHUP::~IHUP(){
        item_1* r = getRoot();
	IHUP::clear(r);
}


void IHUP::clear(item_1* root){
	for (int i = 0; i < root->children.size();i++){
		clear(root->children[i]);
	}
	SAFE_DELETE(root);
}

void IHUP::insert(item_1* root, int tid, const int* item_list, const HeadTable &ht){
	for (int i = 0; i < maxitem; i++){
		if (item_list[i] == 1){
			if (root->children.empty()){
			//�����½ڵ�
				item_1* newitem_1 ;
				newitem_1->isRoot = false;
				newitem_1->t_utility = transaction_utility_array[tid];
				newitem_1->item1 = ht.HeadNode[i].item2;
                                newitem_1->parent = root;
				root->children.push_back(newitem_1);
				int k = root->children.size();
				root = root->children[k-1];
                                item_1* tmp = ht.HeadNode[i].linknode;
                                while(tmp != NULL){
                                    tmp = tmp->brother;
                                }
                                tmp = newitem_1;
			}
			else{
			//�����ҵ��Ƿ��иýڵ�
				bool isIn = false;
				for (int j = 0; j < root->children.size();j++){
					if (root->children[j]->item1 == ht.HeadNode[i].item2){
						//������ڣ����нڵ����ӵĲ���
						root->children[j]->t_utility += transaction_utility_array[tid];
						isIn = true;
						root = root->children[j];
						break;
					}
				}
				if (!isIn){
				//�����½ڵ�
					item_1* newitem_1 ;
					newitem_1->isRoot = false;
					newitem_1->t_utility = transaction_utility_array[tid];
					newitem_1->item1 = ht.HeadNode[i].item2;
					newitem_1->parent = root;
					root->children.push_back(newitem_1);
					int k = root->children.size();
					root = root->children[k-1];
                                        item_1* tmp = ht.HeadNode[i].linknode;
                                        while(tmp != NULL){
                                        tmp = tmp->brother;
                                        }
                                        tmp = newitem_1;
				}
			}
			
		}
	}
}

item_1* IHUP::getRoot(){
	return root;
}