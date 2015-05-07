#include<iostream>
using namespace std;

#include "IHUP.h"
#include "HeadTable.h"
#include "pardhp.h"

#define SAFE_DELETE(p) {if(p){delete p; p=NULL;}}

IHUP::IHUP(HeadTable &ht){
//ָ������ָ��ĳ�ʼ��
//��ʼ��root����root���ӽڵ�
    root = new item_1;
    root->isRoot = true;
    root->t_utility = 0.0;
    root->item1 = -1;
    root->tf = -1;
    root->parent = NULL;
    root->brother = NULL;
    root->children;
    //cout<<root->children.size()<<endl;
		
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

void IHUP::insert(item_1* root, int &tid,  int* item_list, const HeadTable &ht){
	for (int i = 0; i < ht.size; i++){
            //cout<<"root now"<<root->item1<<endl;
            if (item_list[i] == 1 ){
                //cout<<root->children.size()<<endl;
                    if (root->children.empty()){
			//�����½ڵ�
                            	item_1* newitem_1 = new item_1;
				newitem_1->isRoot = false;
				newitem_1->t_utility = transaction_utility_array[tid];
                                newitem_1->tf = 1;
				newitem_1->item1 = ht.HeadNode[i].item2;
                                newitem_1->parent = root;
                                newitem_1->brother = NULL;
                                ht.HeadNode[i].tf++;
				root->children.push_back(newitem_1);
				int k = root->children.size();
				root = root->children[k-1];
                                if(ht.HeadNode[i].linknode == NULL){
                                    ht.HeadNode[i].linknode = newitem_1;
                                }else{
                                    item_1* tmp = ht.HeadNode[i].linknode;
                                    while(tmp->brother != NULL){
                                        tmp = tmp->brother;
                                    }
                                    tmp->brother = newitem_1;
                                }
                                
			}
			else{
			//�����ҵ��Ƿ��иýڵ�
                        	bool isIn = false;
				for (int j = 0; j < root->children.size();j++){
					if (root->children[j]->item1 == ht.HeadNode[i].item2){
						//������ڣ����нڵ����ӵĲ���
						root->children[j]->t_utility += transaction_utility_array[tid];
                                                root->children[j]->tf += 1;
                                                ht.HeadNode[i].tf++;
						isIn = true;
						root = root->children[j];
						break;
					}
				}
				if (!isIn){
				//�����½ڵ�
					item_1* newitem_1 = new item_1;
					newitem_1->isRoot = false;
					newitem_1->t_utility = transaction_utility_array[tid];
					newitem_1->item1 = ht.HeadNode[i].item2;
                                        newitem_1->tf = 1;
					newitem_1->parent = root;
                                        newitem_1->brother = NULL;
					root->children.push_back(newitem_1);
					int k = root->children.size();
					root = root->children[k-1];
                                        ht.HeadNode[i].tf++;
                                       if(ht.HeadNode[i].linknode == NULL){
                                            ht.HeadNode[i].linknode = newitem_1;
                                       }else{
                                            item_1* tmp = ht.HeadNode[i].linknode;
                                
                                            while(tmp->brother != NULL){
                                            tmp = tmp->brother;
                                            }
                                            tmp->brother = newitem_1;
                                        }
				}
			}
			
		}
	}
        
}

item_1* IHUP::getRoot(){
	return root;
}

void IHUP::reinsert(item_1* root, int &tid,  int* item_list, const HeadTable &ht,int offset){
	for (int i = 0; i < ht.size; i++){
            //cout<<"root now"<<root->item1<<endl;
            if(item_list[i] == 1 && i < ht.size-offset){
                for (int j = 0; j < root->children.size();j++){
                    if (root->children[j]->item1 == ht.HeadNode[i].item2){
			//������ڣ�					
			root = root->children[j];
			break;
                    }
		}
                continue;
            }
            if (item_list[i] == 1 && i >= ht.size-offset ){
                //cout<<root->children.size()<<endl;
                    if (root->children.empty()){
			//�����½ڵ�
                            	item_1* newitem_1 = new item_1;
				newitem_1->isRoot = false;
				newitem_1->t_utility = transaction_utility_array[tid];
                                newitem_1->tf = 1;
				newitem_1->item1 = ht.HeadNode[i].item2;
                                newitem_1->parent = root;
                                newitem_1->brother = NULL;
                                ht.HeadNode[i].tf++;
				root->children.push_back(newitem_1);
				int k = root->children.size();
				root = root->children[k-1];
                                if(ht.HeadNode[i].linknode == NULL){
                                    ht.HeadNode[i].linknode = newitem_1;
                                }else{
                                    item_1* tmp = ht.HeadNode[i].linknode;
                                    while(tmp->brother != NULL){
                                        tmp = tmp->brother;
                                    }
                                    tmp->brother = newitem_1;
                                }
                                
			}
			else{
			//�����ҵ��Ƿ��иýڵ�
                        	bool isIn = false;
				for (int j = 0; j < root->children.size();j++){
					if (root->children[j]->item1 == ht.HeadNode[i].item2){
						//������ڣ����нڵ����ӵĲ���
						root->children[j]->t_utility += transaction_utility_array[tid];
                                                root->children[j]->tf += 1;
                                                ht.HeadNode[i].tf++;
						isIn = true;
						root = root->children[j];
						break;
					}
				}
				if (!isIn){
				//�����½ڵ�
					item_1* newitem_1 = new item_1;
					newitem_1->isRoot = false;
					newitem_1->t_utility = transaction_utility_array[tid];
					newitem_1->item1 = ht.HeadNode[i].item2;
                                        newitem_1->tf = 1;
					newitem_1->parent = root;
                                        newitem_1->brother = NULL;
					root->children.push_back(newitem_1);
					int k = root->children.size();
					root = root->children[k-1];
                                        ht.HeadNode[i].tf++;
                                       if(ht.HeadNode[i].linknode == NULL){
                                            ht.HeadNode[i].linknode = newitem_1;
                                       }else{
                                            item_1* tmp = ht.HeadNode[i].linknode;
                                
                                            while(tmp->brother != NULL){
                                            tmp = tmp->brother;
                                            }
                                            tmp->brother = newitem_1;
                                        }
				}
			}
			
		}            
            
	}
        
}