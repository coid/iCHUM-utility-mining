// Compiler options:-
// -DBALT     = make trees balanced

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <math.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <list>
using namespace std;
#include "Database.h"
#include "HeadTable.h"
#include "IHUP.h"
#include "pardhp.h"

#define MAXITER 30

struct timeval tp;
double MIN_UTILITY;
char transaction_file_name0[100];
char transaction_file_name1[100], transaction_file_name2[100], profit_file_name[100];
float *profit_array;
double *transaction_utility_array;
double total_tran_utility;
int num_trans;
int maxitem;
int avg_trans_sz;

int max_trans_sz;

float MIN_UTILITY_PER;
FILE *summary;

double *item_t_utility;

//int more =1, threshold = 2, tot_cand =0, num_db_scan=2, max_pattern_length;
int*  item_index;
int*  item_rank_index;


int transaction_file;
Dbase_Ctrl_Blk DCB;
//HeadTable ht = HeadTable(item_t_utility);
//IHUP ihup= IHUP(ht);
vector <candidate*> cand;    

void form_index(const HeadTable &ht){
    
    for(int i =0 ;i<maxitem;i++){
        item_index[i] = -1;
    }
        for (int i = 0; i < maxitem; i++){
		int indx = ht.HeadNode[i].item2;
		item_index[indx] = i;
	}

}

void form_index1(const HeadTable &ht){
	for(int i =0 ;i<maxitem;i++){
        item_index[i] = -1;
    }
	int j = 0;
    for(int itmp=0;itmp<maxitem;itmp++){
        if(ht.HeadNode[itmp].t_utility < MIN_UTILITY){
            break;
        }
        j = itmp+1;
    }

        for (int i = 0; i < j; i++){
		int indx = ht.HeadNode[i].item2;
		item_index[indx] = i;
	}
	
}

void init_rank(){
	for (int i = 0; i < maxitem; i++){
		item_rank_index[i] = -1;
	}
}

void init_tmp_twu(item_3* list,int count,const HeadTable &ht){
    for(int k =0; k < count; k++){
        list[k].item = ht.HeadNode[k].item2;
        list[k].twu = 0.0;
    }    
}

void mine_proc(HeadTable &ht);

void main_proc(HeadTable &ht, IHUP &ihup, char* transaction_file_name,int offset){
   
   int i, j, k, *buf, numitem, tid;
   
//   int * item_index = new int[maxitem];//记录物品在headtable中的标号
//	int * item_rank_index = new int[maxitem];//记录物品是否在headtable中
   
   //Dbase_Ctrl_Blk DCB;
     
   //max_trans_sz = Database_readfrom(transaction_file_name);  
   
   
   //第二遍去读数据库：先排序，再插入到IHUP中
   int blk = num_trans;
   int lb = 0;
   int ub = blk;
   init_rank();
   
   item_1* ihuproot = ihup.getRoot();

   transaction_file = open(transaction_file_name, O_RDONLY);
   init_DCB(DCB, transaction_file);
   reset_database(DCB);
   get_first_blk(DCB);
   for (i = lb; i < ub;i++){
	   get_next_trans(DCB, buf, numitem, tid);
	   for (j = 0; j < numitem * 2 - 1; j = j + 2){
		   k = item_index[buf[j]];
                   if (k != -1)//实现1：做一个大数组，如果一笔交易出现HeadTable内物品，记录之
			   item_rank_index[k] = 1;
                   
	   }
	   //TODO：插入IHUP树操作
	   //插入操作;
           tid = tid+offset-1;
           ihup.insert(ihuproot, tid, item_rank_index, ht);
           //cout<<"trans "<<tid<<" complete"<<endl;
           init_rank();
   }
   cout<<"complete"<<endl;
   //cout<<"2-1 level num children :"<<ihup.getRoot()->children[0]->children.size()<<endl;
//   item_1* tmp = ht.HeadNode[3].linknode;
//   while(tmp!=NULL){
//       printf("no is %d twu per is %f\n",tmp->item1,tmp->t_utility);
//       tmp=tmp->brother;
//   }
 
   
}

void mine_proc(HeadTable &ht){
    int j = 0;
    for(int itmp=0;itmp<maxitem;itmp++){
        if(ht.HeadNode[itmp].t_utility < MIN_UTILITY){
            break;
        }
        j = itmp+1;
    }
    
    
    cout<<"j is "<<j<<endl;
    int i;
    for(i = j-1 ;i>=0;--i){
        item_rank_1* cur = ht.HeadNode[i].linknode;
        if(cur == NULL){
            cout<<"oh, no"<<endl;
            continue;
        }
        
       // item_3* tmp_twu = (item_3 *)calloc(i, sizeof(item_3));
        item_3* tmp_twu = new item_3[i];
        init_tmp_twu(tmp_twu,i,ht);

        candidate* newcand = new candidate;
            
        newcand->item = ht.HeadNode[i].item2;
        
        itemset* newitemset =new itemset;
        newitemset->count = ht.HeadNode[i].tf;
        (newitemset->items).push_back(ht.HeadNode[i].item2);
        newitemset->twu = ht.HeadNode[i].t_utility;
        newitemset->utility = 0.0;
        (newcand->itemslist).push_back(newitemset);
        
 //       ct_head ct;//条件树vector
 //       ct.first = NULL;
        
        list<ct_node*> ct;
        while(cur!=NULL){
            item_rank_1* tmp = cur;
            ct_node* ctnode = new ct_node;
            ctnode->tu_per = cur->t_utility;
            //ctnode->next = NULL;
            
            while(!tmp->parent->isRoot){
                int itemno = tmp->parent->item1 ;
                tmp_twu[item_index[itemno]].twu += cur->t_utility;
                (ctnode->items).push_back(itemno);
                tmp = tmp->parent;               
            }
            ct.push_back(ctnode);
            cur = cur->brother;
        }
        
//        while(cur!=NULL){
//            item_rank_1* tmp = cur; 
//            ct_node* ctnode = new ct_node;
//            ct_node* ct_cur = ct.first;
//            
//            if(ct.first==NULL){
//                ctnode->tu_per = cur->t_utility;
//                ctnode->next = NULL;
//                ct.first = ctnode; 
//                ct_cur = ct.first;
//            }else{                
//                while(ct_cur->next!=NULL){
//                    ct_cur = ct_cur->next;
//                }
//                ctnode->tu_per = cur->t_utility;
//                ctnode->next = NULL;
//                ct_cur->next = ctnode;
//                ct_cur = ct_cur->next;
//            }
//            
//            while(!tmp->parent->isRoot){
//                int itemno = tmp->parent->item1 ;
//                tmp_twu[item_index[itemno]].twu += cur->t_utility;
//                ctnode->items.push_back(itemno);
//                tmp = tmp->parent;               
//            }
//            
//            cur = cur->brother;
//        }//遍历条件树
        
//        printf("contitional tree completed\n");
//        ct_node* tst = ct.first; 
//        while(tst!=NULL){
//            printf("ct %d %f\n",i,tst->tu_per);
//            tst = tst->next;
//        }
        
        bool goon = false;//记录是否还有潜在大的itemset
        int count =0;//记录新增加的有多少个
        for(int k=i-1;k>=0;--k){
            if(tmp_twu[k].twu>=MIN_UTILITY){
                goon = true;
                count++;
                itemset* newitemset =new itemset;
                (newitemset->items).push_back(ht.HeadNode[i].item2);
                newitemset->count = ht.HeadNode[k].tf;
                (newitemset->items).push_back(ht.HeadNode[k].item2);
                newitemset->twu =  tmp_twu[k].twu;
                newitemset->utility = 0.0;
                (newcand->itemslist).push_back(newitemset);               
            }
        }
        
        while(goon){
            int tmp_count = count;
            int tmp_begin = newcand->itemslist.size();
            goon = false;
            count = 0;
            for(int m = tmp_begin - tmp_count;m < tmp_begin; ++m ){
                init_tmp_twu(tmp_twu,i,ht);
                //newcand->itemslist[m]->items[1],...size()-1
//                for(int n =0; n < ct.size(); n++){
//                    //ct[n]->items[]
//                    for(int s =0 , t = 1 ; s < ct[n]->items.size(); s++){
//                        if(ct[n]->items[s] == newcand->itemslist[m]->items[t]) {t++;}
//                        if(t == newcand->itemslist[m]->items.size()) {
//                            tmp_twu[item_index[ct[n]->items[s]]].twu += ct[n]->tu_per;                                                                
//                        }                            
//                    }
//                }//更新tmp_twu 
                list<ct_node*>::iterator ctIterator;
                for(ctIterator = ct.begin();ctIterator!=ct.end();ctIterator++){
                    int t =1;
                    for(int s=0;s<(*ctIterator)->items.size();++s){
                        if(t<newcand->itemslist[m]->items.size() && (*ctIterator)->items[s] == newcand->itemslist[m]->items[t]) {
                            t = t+1;
                            continue;
                            }
                        if(t == newcand->itemslist[m]->items.size()) {
                            tmp_twu[item_index[(*ctIterator)->items[s]]].twu += (*ctIterator)->tu_per;
                        }
                    }        
                }
                
                for(int k=i-1;k>=0;--k){
                    if(tmp_twu[k].twu >= MIN_UTILITY){
                    goon = true;
                    ++count;
                    itemset* newitemset =new itemset;
                    //int tmp_num = newcand->itemslist[m]->items.size()
                    for(int l = 0;l < newcand->itemslist[m]->items.size();++l){
                        (newitemset->items).push_back(newcand->itemslist[m]->items[l]);
                    }
                    newitemset->count = ht.HeadNode[k].tf;                    
                    newitemset->twu =  tmp_twu[k].twu;
                    (newitemset->items).push_back(ht.HeadNode[k].item2);
                    newitemset->utility = 0.0;
                    (newcand->itemslist).push_back(newitemset);               
                    }
                }//如果仍然有潜在的high组合,添加进newcand
                
            }
            
            
            
        }
        
        cand.push_back(newcand);
        
        //cout<<"cand "<< cand[cand.size()-1]->item <<" finished"<<endl;
        delete[] tmp_twu;
//        while(ct.first!=NULL){
//            ct_node* tmp  =ct.first;
//            ct.first = tmp->next;
//            while(tmp->items.size()>0){tmp->items.pop_back();}            
//            delete tmp;
//            tmp = NULL;
//        }
    }
    int total_cand =0;
    for(int m = 0;m<cand.size();m++){
        total_cand += cand[m]->itemslist.size();
    }
    printf( "total cand is %d\n",total_cand);
    
//    for(int m = 0;m<cand.size();m++){
//        
//            for(int n=0;n<cand[m]->itemslist.size();n++){
//                for(int nn=0;nn<cand[m]->itemslist[n]->items.size();nn++){
//                printf("%d ",cand[m]->itemslist[n]->items[nn]);
//                }
//                printf("%f ",cand[m]->itemslist[n]->twu);
//                printf("%f\n",cand[m]->itemslist[n]->utility);
//            }
//        
//    }
        
}

void vertify_proc(HeadTable &ht,char* transaction_file_name){
   //第三遍的数据库,逐条计算cand的真实utility value
   int* buf;
   int i,j,numitem, tid;
   
   int blk = num_trans;
   
   int lb = 0;
   int ub = blk;
   
   init_rank();
   
   transaction_file = open(transaction_file_name, O_RDONLY);
   init_DCB(DCB, transaction_file);
   reset_database(DCB);
   get_first_blk(DCB);
//   for(int tmp=0;tmp<maxitem;tmp++){
//       printf("item_index %d is %d\n",tmp,item_index[tmp]);
//   }
int count = 0;
    for(int itmp=0;itmp<maxitem;itmp++){
        if(ht.HeadNode[itmp].t_utility < MIN_UTILITY){
            break;
        }
        count = itmp+1;
    }

   double * utility_per_item = (double *)calloc(count, sizeof(double));
   for (i = lb; i < ub;i++){
	   get_next_trans(DCB, buf, numitem, tid);
           
           for(int tmp=0;tmp<count;tmp++){utility_per_item[tmp] =0.0;}
           for (j = 0; j < numitem * 2 - 1; j = j + 2){
		   if(item_index[buf[j]] != -1){
                       utility_per_item[item_index[buf[j]]] = buf[j+1] * profit_array[buf[j]];
                       item_rank_index[item_index[buf[j]]] = 1;
                   } 
           }
//           if(i==0){
//               for(int tmp=0;tmp<maxitem;tmp++){
//                   printf("this trans contain %d\n",item_rank_index[tmp]);
//                   printf("this trans contain %f\n",utility_per_item[tmp]);
//               }
//           }
	   
	   for(int m = 0;m<cand.size();m++){
               if(item_rank_index[item_index[cand[m]->item]]!=1){continue;}          
               //cout<<cand[m]->item<<" testing"<<endl; 
               for(int n=0;n<cand[m]->itemslist.size();n++){
                   bool isFull = true;
                   for(int s = 0;s<cand[m]->itemslist[n]->items.size();s++){
                       if(item_rank_index[item_index[cand[m]->itemslist[n]->items[s]]]!=1) {isFull = false; }
                   }
                   if(isFull){
                       double tmp_utility = 0.0;
                       
                       for(int s = 0;s<cand[m]->itemslist[n]->items.size();s++){
                           tmp_utility += utility_per_item[item_index[cand[m]->itemslist[n]->items[s]]];                        
                       }
                       
                       cand[m]->itemslist[n]->utility += tmp_utility;
                   }
               }
           }
           //
           init_rank();
   }
   delete[] utility_per_item;
   
   int real_high = 0;
    
    for(int ri=0; ri < cand.size(); ri++){
        //cout<<"cand with :"<< cand[ri]->item <<endl;        
        fprintf(summary,"cand with : %d \n",cand[ri]->item);
        
        for(int rj=0; rj < cand[ri]->itemslist.size(); rj++){
            if(cand[ri]->itemslist[rj]->utility >= MIN_UTILITY){
                real_high++;
                for(int rk=0;rk<cand[ri]->itemslist[rj]->items.size();rk++){
                //cout<<cand[ri]->itemslist[rj]->items[rk]<<" ";
                fprintf(summary,"%d ",cand[ri]->itemslist[rj]->items[rk]);
                }
                fprintf(summary,"%f\n",cand[ri]->itemslist[rj]->utility);
                //cout<<cand[i]->itemslist[j]->utility<<"\n";
            }            
            
           // printf("\n\ttwu \t %d\n",cand[i]->itemslist[j]->twu);
            //printf("\n\tutility \t %d\n",cand[i]->itemslist[j]->utility);
                    
            //cout<<"\n\ttwu"<<"\t"<<cand[i]->itemslist[j]->twu<<endl;
            //cout<<"\tutility"<<"\t"<<cand[i]->itemslist[j]->utility<<endl;
        }
    }
   printf("real high no is %d\n",real_high);
}

void update_proc(HeadTable &ht){
    bool exchange = false;
    do{
    exchange = false;
    for(int i = maxitem-1;i>0;i--){
        int j = i-1;
        if(item_t_utility[ht.HeadNode[i].item2] <= item_t_utility[ht.HeadNode[j].item2]){
            continue;
        }
        exchange = true;
        //swap headnode
                
        ht.swap_ht(i,j);
        //cout<<i<<" and "<<j<<" exchanged"<<endl;
        
        //swap all node in the tree
        item_rank_1* cur = ht.HeadNode[j].linknode;
        item_rank_1* cur_p = NULL;
//        item_rank_1* curu = ht.HeadNode[j].linknode;
//        item_rank_1* curu_p = ht.HeadNode[j].linknode;
                        
        while(cur!=NULL){
            if(cur->parent->item1 != ht.HeadNode[i].item2  ){
                cur_p = cur;
                cur = cur->brother;
                continue;
            } 
                        
            if(cur->tf == cur->parent->tf){
                item_rank_1* tmp_t = cur->parent; //new item_rank_1;
//                tmp_t->isRoot = cur->parent->isRoot;
//                tmp_t->item1 = cur->parent->item1;
//                tmp_t->t_utility = cur->parent->t_utility;
//                tmp_t->tf = cur->parent->tf;//j节点缺少初始化 儿子、父亲和兄弟节点
                
                item_rank_1* tmp_pa = cur->parent->parent;
                 
                
//                while(curu!=NULL){//找到j节点的前兄弟节点
//                    if(curu->tf == cur->tf && curu->t_utility == cur->t_utility && curu->children.size() == 1){
//                        break;
//                    }
//                    curu_p = curu;
//                    curu = curu->brother;
//                }
                
                bool isIn = false;
                vector< item_rank_1* >::iterator cIterator = tmp_pa->children.begin();
                while(cIterator != tmp_pa->children.end()){
                    if((*cIterator)->item1 == tmp_t->item1){
                        //tmp_t->brother = (*cIterator)->brother;//j节点的兄弟,目前看似乎不受影响
                        cIterator = tmp_pa->children.erase(cIterator);//从祖先节点解锁b节点
                        continue;
                    }
                    if((*cIterator)->item1 == cur->item1){//祖先节点已经有a
                        isIn = true;
                        (*cIterator)->tf += cur->tf;
                        (*cIterator)->t_utility += cur->t_utility;
                        (*cIterator)->children.push_back(tmp_t);
                        //*cIterator->brother = cur->brother;
                        tmp_t->parent = (*cIterator);//j节点的父亲                        
                        if(cur_p == NULL){
                            //cur_p = cur;
                            //cur = cur->brother;
                            ht.HeadNode[j].linknode = cur->brother;
                            //cur_p = cur->brother;
                        }else{
                            cur_p->brother = cur->brother;
                        }
                    }
                    ++cIterator;
                }
                
                if(!isIn) {//祖先节点没有a
                    tmp_pa->children.push_back(cur);
                    cur->children.push_back(tmp_t);
                    cur->parent = tmp_pa;
                    tmp_t->parent = cur;
                    if(cur_p == NULL){
                        cur_p = ht.HeadNode[j].linknode;
                    }else{
                        cur_p = cur_p->brother;
                    }
                    
                }
                tmp_t->children.pop_back();
                cIterator = cur->children.begin();
                while(cIterator!=cur->children.end()){
                    if((*cIterator)->item1 != tmp_t->item1){
                        item_rank_1* tmp_tt = (*cIterator);
                        tmp_tt->parent = tmp_t;
                        tmp_t->children.push_back(tmp_tt);//j节点的儿子
                        cIterator = cur->children.erase(cIterator);
                    }else{
                    ++cIterator;
                    }
                }
                if(cur_p!=NULL){
                    cur = cur_p->brother;     
                }else{
                    cur = ht.HeadNode[j].linknode;
                }
                
            }else{
                item_rank_1* tmp_t = cur->parent;
                item_rank_1* tmp_pa = tmp_t->parent;
                item_rank_1* tmp_x = new item_rank_1;
                
                
                bool isIn = false;
                vector< item_rank_1* >::iterator cIterator;
                for(cIterator = tmp_pa->children.begin(); cIterator!=tmp_pa->children.end();cIterator++){
                    if((*cIterator)->item1 == tmp_t->item1){
                        (*cIterator)->t_utility -= cur->t_utility;//从祖先节点减去a节点的值
                        (*cIterator)->tf -= cur->tf;
                        
                        tmp_x->isRoot = false;
                        tmp_x->item1 = tmp_t->item1;
                        tmp_x->t_utility = cur->t_utility;
                        tmp_x->tf = cur->tf;
                        tmp_x->brother = (*cIterator)->brother;
                        (*cIterator)->brother = tmp_x;                        
                    }
                    if((*cIterator)->item1 == cur->item1){
                        isIn = true;
                        (*cIterator)->tf += cur->tf;
                        (*cIterator)->t_utility += cur->t_utility;
                        (*cIterator)->children.push_back(tmp_x);
                        //*cIterator->brother = cur->brother;
                        tmp_x->parent = (*cIterator); 
                        if(cur_p == NULL){
                            //cur_p = cur;
                            //cur = cur->brother;
                            ht.HeadNode[j].linknode = cur->brother;
                            //cur_p = cur->brother;
                        }else{
                            cur_p->brother = cur->brother;
                        }
                    }
                    
                }
                if(!isIn){
                    tmp_pa->children.push_back(cur);
                    cur->children.push_back(tmp_x);
                    tmp_x->parent = cur; 
                    cur->parent = tmp_pa;
                    if(cur_p == NULL){
                        cur_p = ht.HeadNode[j].linknode;
                    }else{
                        cur_p = cur_p->brother;
                    }
                }
                cIterator = tmp_t->children.begin();
                while(cIterator!=tmp_t->children.end()){
                    if((*cIterator)->item1 == cur->item1){
                        cIterator = tmp_t->children.erase(cIterator);
                        continue;
                    }
                    ++cIterator;
                }
                
                cIterator = cur->children.begin();
                while(cIterator!=cur->children.end()){
                    if((*cIterator)->item1 != tmp_t->item1){
                        item_rank_1* tmp_tt = (*cIterator);
                        tmp_x->children.push_back(tmp_tt);
                        tmp_tt->parent = tmp_x;
                        cIterator = cur->children.erase(cIterator);
                        continue;
                    }
                    ++cIterator;
                }
                if(cur_p!=NULL){
                    cur = cur_p->brother;     
                }else{
                    cur = ht.HeadNode[j].linknode;
                }
                        
            }
                        
        }
               
        
    }}while(exchange);
}

int main(int argc, char **argv)
{
    int transaction_file, transaction_file2, profit_file;
    summary = fopen("out", "w");
    //int indx, i, k, j, m, n, *buf, numitem, tid;
    double t1,t2,t3,t4,t5,t6,t7;
    //int total_real_high;
    

    if (argc < 6){
        cout << "usage: <transaction_file1> <transaction_file2> <profit_file> <utility_threshold> <whole transaction>\n";
        exit(3);
    }
     strcpy(transaction_file_name0, argv[5]);                 
     strcpy(transaction_file_name1, argv[1]);
     strcpy(transaction_file_name2, argv[2]);
     strcpy(profit_file_name, argv[3]);
    
     transaction_file = open(transaction_file_name1, O_RDONLY);
     transaction_file2 = open(transaction_file_name2, O_RDONLY);
seconds(t1); 
//phase I begins
//建立HeadTable，IHUP
     read(transaction_file, &num_trans, ITSZ);
     read(transaction_file, &maxitem, ITSZ);
     read(transaction_file, &avg_trans_sz, ITSZ);
//      fprintf(summary,"%d\n",num_trans);
//      fprintf(summary,"%d\n",maxitem);
//      fprintf(summary,"%d\n",avg_trans_sz);
      printf("transactions  is %d \n",num_trans);
      printf("items  is %d \n",maxitem);
     transaction_utility_array = (double *)calloc(num_trans, sizeof(double));
     item_t_utility = (double *)calloc(maxitem, sizeof(double));
     profit_array = (float *)calloc(maxitem, sizeof(float));

     profit_file = open(profit_file_name, O_RDONLY);
     
     read(profit_file, profit_array,sizeof(float)*maxitem);
     close(profit_file);
//     for(int tmp=0;tmp<maxitem;tmp++){
//        printf("item %d profit is %f \n",tmp,profit_array[tmp]);
//     }
     MIN_UTILITY_PER = atof(argv[4]);
     item_index = (int *)calloc(maxitem, sizeof(int));//记录物品在headtable中的标号
     item_rank_index = (int *)calloc(maxitem, sizeof(int));//记录物品是否在headtable中
     total_tran_utility = 0.0;
    max_trans_sz = Database_readfrom(transaction_file_name1);     
         
    int count=0;
   for (int  i=0; i<maxitem; i++) {
      if (item_t_utility[i] >= MIN_UTILITY) 
          count++;
   }
    if(count==0){
    fprintf(summary, "\n");
     //printf("testing\n");
     fflush(summary);
     fflush(stdout);
     close_DCB(DCB);
     
     return(0);
    }
    printf("ok", "\n");
    HeadTable ht = HeadTable(item_t_utility); 
    ht.TWU_sort();//生成HeadTable，并排序
//    for(int i =0;i<maxitem;i++){
//        printf("ht %d is %d , which %f\n",i,ht.HeadNode[i].item2,ht.HeadNode[i].t_utility);
//       //cout<<"ht no "<<i <<"is "<<ht.HeadNode[i].item2<<",which "<<ht.HeadNode[i].t_utility<<endl;
//    }
    form_index(ht);
    
    IHUP ihup= IHUP(ht);
    main_proc(ht,ihup,transaction_file_name1,0);//两次扫描数据库+构建IHUP树
        
seconds(t2);
//phase I continues
//进行挖掘算法,生成候选集
    
    mine_proc(ht);
    
seconds(t3);    
//phase II begins
//进行验证工作
form_index1(ht);
    vertify_proc(ht,transaction_file_name1);
    

seconds(t4);
//开始挖掘新的数据库
cand.clear();
printf("old transactions  is %d \n",num_trans);

int offset = num_trans;
read(transaction_file2, &num_trans, ITSZ);
int tmp_max_trans_sz = max_trans_sz;
//transaction_utility_array需要重新分配
int tot_num_trans = num_trans+offset;
printf("transactions  is %d \n",tot_num_trans);
transaction_utility_array = (double *)realloc(transaction_utility_array, tot_num_trans*sizeof(double));

max_trans_sz = Database_readfrom(transaction_file_name2,offset);  
if (max_trans_sz < tmp_max_trans_sz){
    max_trans_sz = tmp_max_trans_sz;
}
for(int i =0;i<maxitem;i++){
    ht.HeadNode[i].t_utility = item_t_utility[ht.HeadNode[i].item2];
}
cout<<"update begins"<<endl;
update_proc(ht);

cout<<"update finished"<<endl;
form_index(ht);
//for(int i =0;i<maxitem;i++){
//    printf("ht %d is %d , which %f\n",i,ht.HeadNode[i].item2,ht.HeadNode[i].t_utility);
//       //cout<<"ht no "<<i <<"is "<<ht.HeadNode[i].item2<<",which "<<ht.HeadNode[i].t_utility<<endl;
//}

main_proc(ht,ihup,transaction_file_name2,offset);
seconds(t5);
cout<<"mine begins"<<endl;

num_trans = tot_num_trans;
mine_proc(ht);
cout<<"mine ends"<<endl;

seconds(t6);
cout<<"vertify begins"<<endl;
form_index1(ht);
vertify_proc(ht,transaction_file_name0);
cout<<"vertify finished"<<endl;
seconds(t7);


//     fprintf(summary, "Cands %d ", tot_cand);
//printf("Final tot_cand=%d, total_real_high=%d\n", tot_cand, total_real_high);
printf("Original: Phase I-I time=%f,Phase I-II time=%f, Phase II time=%f, total time = %f\n", t2-t1,t3-t2,t4-t3, t4-t1); 
//     fprintf(summary, "Cands %d Total = %f ", tot_cand, te-ts);
printf("Update: Phase I-I time=%f,Phase I-II time=%f, Phase II time=%f, total time = %f\n", t5-t4,t6-t5,t7-t6, t7-t1); 


     fprintf(summary, "\n");
     //printf("testing\n");
     fflush(summary);
     
     //fflush(tmp1);
     fflush(stdout);
     close_DCB(DCB);
     
     return(0);
}


