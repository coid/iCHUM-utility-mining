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
using namespace std;
#include "Database.h"
#include "HeadTable.h"
#include "IHUP.h"
#include "pardhp.h"

#define MAXITER 30

struct timeval tp;
double MIN_UTILITY;

char transaction_file_name[100], profit_file_name[100];
float *profit_array;
double *transaction_utility_array;
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

        for (int i = 0; i < ht.size; i++){
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

void main_proc(HeadTable &ht, IHUP &ihup){
   
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
           ihup.insert(ihuproot, tid, item_rank_index, ht);
           init_rank();
   }
   cout<<"complete"<<endl;
   cout<<"2-1 level num children :"<<ihup.getRoot()->children[0]->children.size()<<endl;
//   item_1* tmp = ht.HeadNode[3].linknode;
//   while(tmp!=NULL){
//       printf("no is %d twu per is %f\n",tmp->item1,tmp->t_utility);
//       tmp=tmp->brother;
//   }
 
   
}

void mine_proc(HeadTable &ht){
    
    int j = ht.size;
    cout<<"j is "<<j<<endl;
    int i;
    for(i = j-1 ;i>=0;i--){
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
        newitemset->items.push_back(ht.HeadNode[i].item2);
        newitemset->twu = ht.HeadNode[i].t_utility;
        newitemset->utility = 0.0;
        newcand->itemslist.push_back(newitemset);
        
        ct_head ct;//条件树vector
        ct.first = NULL;
        
        while(cur!=NULL){
            item_rank_1* tmp = cur; 
            ct_node* ctnode = new ct_node;
            ct_node* ct_cur = ct.first;
            
            if(ct.first==NULL){
                ctnode->tu_per = cur->t_utility;
                ctnode->next = NULL;
                ct.first = ctnode; 
                ct_cur = ct.first;
            }else{                
                while(ct_cur->next!=NULL){
                    ct_cur = ct_cur->next;
                }
                ctnode->tu_per = cur->t_utility;
                ctnode->next = NULL;
                ct_cur->next = ctnode;
                ct_cur = ct_cur->next;
            }
            
            while(!tmp->parent->isRoot){
                int itemno = tmp->parent->item1 ;
                tmp_twu[item_index[itemno]].twu += cur->t_utility;
                ctnode->items.push_back(itemno);
                tmp = tmp->parent;               
            }
            
            cur = cur->brother;
        }//遍历条件树
        
//        printf("contitional tree completed\n");
//        ct_node* tst = ct.first; 
//        while(tst!=NULL){
//            printf("ct %d %f\n",i,tst->tu_per);
//            tst = tst->next;
//        }
        
        bool goon = false;//记录是否还有潜在大的itemset
        int count =0;//记录新增加的有多少个
        for(int k=i-1;k>=0;k--){
            if(tmp_twu[k].twu>=MIN_UTILITY){
                goon = true;
                count++;
                itemset* newitemset =new itemset;
                newitemset->items.push_back(ht.HeadNode[i].item2);
                newitemset->count = ht.HeadNode[k].tf;
                newitemset->items.push_back(ht.HeadNode[k].item2);
                newitemset->twu =  tmp_twu[k].twu;
                newitemset->utility = 0.0;
                newcand->itemslist.push_back(newitemset);               
            }
        }
        
        while(goon){
            int tmp_count = count;
            int tmp_begin = newcand->itemslist.size();
            goon = false;
            count = 0;
            for(int m = tmp_begin - tmp_count;m < tmp_begin; m++ ){
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
                ct_node* ct_cur = ct.first;
                while(ct_cur!=NULL){
                    
                    for(int s=0,t=1;s<ct_cur->items.size();s++){
                        if(ct_cur->items[s] == newcand->itemslist[m]->items[t]) {
                            t = t+1;
                            continue;
                            }
                        if(t == newcand->itemslist[m]->items.size()) {
                            tmp_twu[item_index[ct_cur->items[s]]].twu += ct_cur->tu_per;
                        }
                    }        
                    ct_cur = ct_cur->next;
                }
                
                for(int k=i-1;k>=0;k--){
                    if(tmp_twu[k].twu >= MIN_UTILITY){
                    goon = true;
                    count++;
                    itemset* newitemset =new itemset;
                    //int tmp_num = newcand->itemslist[m]->items.size()
                    for(int l = 0;l < newcand->itemslist[m]->items.size();l++){
                        newitemset->items.push_back(newcand->itemslist[m]->items[l]);
                    }
                    newitemset->count = ht.HeadNode[k].tf;                    
                    newitemset->twu =  tmp_twu[k].twu;
                    newitemset->items.push_back(ht.HeadNode[k].item2);
                    newitemset->utility = 0.0;
                    newcand->itemslist.push_back(newitemset);               
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
    printf("total cand is %d\n",total_cand);
        
}

void vertify_proc(HeadTable &ht){
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
   double * utility_per_item = (double *)calloc(ht.size, sizeof(double));
   for (i = lb; i < ub;i++){
	   get_next_trans(DCB, buf, numitem, tid);
           
           for(int tmp=0;tmp<ht.size;tmp++){utility_per_item[tmp] =0.0;}
           for (j = 0; j < numitem * 2 - 1; j = j + 2){
		   if(item_index[buf[j]] != -1){
                       utility_per_item[item_index[buf[j]]] = buf[j+1] * profit_array[buf[j]];
                       item_rank_index[item_index[buf[j]]] = 1;
                   } 
           }
//           if(i==0){
//               for(int tmp=0;tmp<ht.size;tmp++){
//                   printf("this trans contain %d\n",item_rank_index[tmp]);
//                   printf("this trans contain %f\n",utility_per_item[tmp]);
//               }
//           }
	   
	   for(int m = 0;m<cand.size();m++){
               if(item_rank_index[item_index[cand[m]->item]]!=1){continue;}          
                              
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
        cout<<"cand with :"<< cand[ri]->item <<endl;        
        
        for(int rj=0; rj < cand[ri]->itemslist.size(); rj++){
            if(cand[ri]->itemslist[rj]->utility >= MIN_UTILITY){
                real_high++;
                for(int rk=0;rk<cand[ri]->itemslist[rj]->items.size();rk++){
                cout<<cand[ri]->itemslist[rj]->items[rk]<<" ";
                }
                printf("%f\n",cand[ri]->itemslist[rj]->utility);
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

int main(int argc, char **argv)
{
    int transaction_file, profit_file;
    summary = fopen("out", "a+");
    //int indx, i, k, j, m, n, *buf, numitem, tid;
    double t1,t2,t3,t4;
    //int total_real_high;
    

    if (argc < 4){
        cout << "usage: <transaction_file> <profit_file> <utility_threshold>\n";
        exit(3);
    }
                      
     strcpy(transaction_file_name, argv[1]);
     strcpy(profit_file_name, argv[2]);
    
     transaction_file = open(transaction_file_name, O_RDONLY);
seconds(t1); 
//phase I begins
//建立HeadTable，IHUP
     read(transaction_file, &num_trans, ITSZ);
     read(transaction_file, &maxitem, ITSZ);
     read(transaction_file, &avg_trans_sz, ITSZ);
     
     transaction_utility_array = (double *)calloc(num_trans, sizeof(double));
     profit_array = (float *)calloc(maxitem, sizeof(float));

     profit_file = open(profit_file_name, O_RDONLY);
     
     read(profit_file, profit_array,sizeof(float)*maxitem);
     close(profit_file);
//     for(int tmp=0;tmp<maxitem;tmp++){
//        printf("item %d profit is %f \n",tmp,profit_array[tmp]);
//     }
     MIN_UTILITY_PER = atof(argv[3]);
     item_index = (int *)calloc(maxitem, sizeof(int));//记录物品在headtable中的标号
     item_rank_index = (int *)calloc(maxitem, sizeof(int));//记录物品是否在headtable中
     
    max_trans_sz = Database_readfrom(transaction_file_name); 
    HeadTable ht = HeadTable(item_t_utility);   
    ht.TWU_sort();//生成HeadTable，并排序
//    for(int i =0;i<ht.size;i++){
//        printf("ht %d is %d , which %f\n",i,ht.HeadNode[i].item2,ht.HeadNode[i].t_utility);
//       //cout<<"ht no "<<i <<"is "<<ht.HeadNode[i].item2<<",which "<<ht.HeadNode[i].t_utility<<endl;
//    }
    form_index(ht);
    
    IHUP ihup= IHUP(ht);
    main_proc(ht,ihup);//两次扫描数据库+构建IHUP树
        
seconds(t2);
//phase I continues
//进行挖掘算法,生成候选集
    
    mine_proc(ht);
    
seconds(t3);    
//phase II begins
//进行验证工作

    vertify_proc(ht);
    

seconds(t4);
//     fprintf(summary, "Cands %d ", tot_cand);
//printf("Final tot_cand=%d, total_real_high=%d\n", tot_cand, total_real_high);
printf("Phase I-I time=%f,Phase I-II time=%f, Phase II time=%f, total time = %f\n", t2-t1,t3-t2,t4-t3, t4-t1); 
//     fprintf(summary, "Cands %d Total = %f ", tot_cand, te-ts);

     fprintf(summary, "\n");
     //printf("testing\n");
     fflush(summary);
     fflush(stdout);
     close_DCB(DCB);
     
     return(0);
}


