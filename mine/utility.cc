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
#include "Itemset.h"
#include "ListItemset.h"
#include "HashTree.h"
#include "Database.h"
#include "pardhp.h"

#define MAXITER 30

struct timeval tp;
double MIN_UTILITY;
int NUM_INSERT;
int NUM_ACTUAL_INSERT;

char transaction_file_name[100], profit_file_name[100];
float *profit_array, *transaction_utility_array;
int num_trans;
int maxitem;
int avg_trans_sz;

int max_trans_sz;

float MIN_UTILITY_PER;
FILE *summary;

double *item_t_utility;

int more =1, threshold = 2, tot_cand =0, num_db_scan=2, max_pattern_length;

int *item_index = NULL;
int *item_rank_index = NULL;

int transaction_file;
Dbase_Ctrl_Blk DCB;
HeadTable ht;
IHUP ihup;

void form_index(const HeadTable &ht){

	for (int i = 0; i < maxitem; i++){
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



void main_proc(){
   
   int i, j, k, *buf, numitem, tid;
   
   Dbase_Ctrl_Blk DCB;
   HeadTable ht;
   
   max_trans_sz = Database_readfrom(transaction_file_name);  
   
   ht = HeadTable(item_t_utility);
   ht.TWU_sort();//����HeadTable��������
   form_index(ht);
   
   //�ڶ���ȥ�����ݿ⣺�������ٲ��뵽IHUP��
   int blk = num_trans;
   int lb = 0;
   int ub = num_trans;
   init_rank();
   ihup = new IHUP(ht);
   ihuproot = ihup.getRoot();

   transaction_file = open(transaction_file_name, O_RDONLY);
   init_DCB(DCB, transaction_file);
   reset_database(DCB);
   get_first_blk(DCB);
   for (i = lb; i < ub;i++){
	   get_next_trans(DCB, buf, numitem, tid);
	   for (j = 0; j < numitem * 2 - 1; j = j + 2){
		   k = item_index[buf[j]];
		   if (k != -1)//ʵ��1����һ�������飬���һ�ʽ��׳���HeadTable����Ʒ����¼֮
			   item_rank_index[k] = 1;
	   }
	   //TODO������IHUP������
	   //�������;
	   insert(ihuproot, tid, item_rank_index, ht);
	   
	   init_rank();
   }
   
   //   close_DCB(DCB);
//   fprintf(summary,"MAIN %f ", t2-t1);
}

int main(int argc, char **argv)
{
    int transaction_file, profit_file;
    summary = fopen("out", "a+");
    int indx, i, k, j, m, n, *buf, numitem, tid;
    
    int total_real_high;
    

    if (argc < 4){
        cout << "usage: <transaction_file> <profit_file> <utility_threshold>\n";
        exit(3);
    }
                      
     strcpy(transaction_file_name, argv[1]);
     strcpy(profit_file_name, argv[2]);
    
     transaction_file = open(transaction_file_name, O_RDONLY);
seconds(t1); 
//phase I begin
//����HeadTable��IHUP�����ھ�
     read(transaction_file, &num_trans, ITSZ);
     read(transaction_file, &maxitem, ITSZ);
     read(transaction_file, &avg_trans_sz, ITSZ);
     
     transaction_utility_array = (float *)calloc(num_trans, sizeof(float));
     profit_array = (float *)calloc(maxitem, sizeof(float));

     profit_file = open(profit_file_name, O_RDONLY);
     
     read(profit_file, profit_array,sizeof(float)*maxitem);
     close(profit_file);
     MIN_UTILITY_PER = atof(argv[3]);
     
	 item_index = new int[maxitem];
	 item_rank_index = new int[maxitem];
     
     main_proc();//����ɨ�����ݿ�+����IHUP��
seconds(t2);     
//phase II begin
//������֤����

    
seconds(t3);    
//     fprintf(summary, "Cands %d ", tot_cand);
printf("Final tot_cand=%d, total_real_high=%d, num_db_scan=%d\n", tot_cand, total_real_high, num_db_scan);
printf("Phase I time=%f, total time = %f\n", t2-t1, t3-t1); 
//     fprintf(summary, "Cands %d Total = %f ", tot_cand, te-ts);
     fprintf(summary, "\n");
     //printf("testing\n");
     fflush(summary);
     fflush(stdout);
     close_DCB(DCB);
     delete Candidate;
     if (Largelist) delete Largelist;
        delete [] hash_indx;

     delete [] start;
     delete [] enda;
     delete [] hash_pos;
     return(0);
}


