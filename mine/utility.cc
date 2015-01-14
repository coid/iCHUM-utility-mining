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
int *hash_pos, *start, *enda;

int *item_index = NULL;

int transaction_file;
Dbase_Ctrl_Blk DCB;

void form_index(const HeadTable &ht){

	for (int i = 0; i < maxitem; i++){
		item_index[i] = -1;
	}

	for (int i = 0; i < ht.size; i++){
		int indx = ht.HeadNode[i].item2;
		item_index[indx] = i;
	}

}




void main_proc(){
   itemset2 *t_utilitycnt;
   Itemset *trans=NULL;
   int i, j, idx,k, vist, *buf, numitem, tid, hash_function;
   char *trans_bitvec = new char[maxitem];
   itemset2 *cnt_ary;
   HashTree *oldCand=NULL; 
   Dbase_Ctrl_Blk DCB;
   int last_tot_cand;

   max_trans_sz = Database_readfrom(transaction_file_name, t_utilitycnt);  
   trans = new Itemset(max_trans_sz);
 
   transaction_file = open(transaction_file_name, O_RDONLY);
   init_DCB(DCB, transaction_file);
   for (i=0; i < maxitem; i++){
       trans_bitvec[i] = 0;
   }

//   fprintf(summary, "Database %s utility_threshold %f num_trans %d min_utility %d thr %d\n", transaction_file_name, MIN_UTILITY_PER, num_trans,MIN_UTILITY, threshold);
  
   hash_indx = new int [maxitem];
   cnt_ary = t_utilitycnt;
 
   for(i=0; i < maxitem; i++)
      hash_indx[i] = -1;
   for(i=0; i < maxitem-1; i++){
      for (j=0; j < cnt_ary[i].count; j++){
         if (cnt_ary[i].t2[j].t_utility >= MIN_UTILITY){
            hash_indx[i] = 0;
            hash_indx[cnt_ary[i].t2[j].item2] = 0;
            Itemset *it = new Itemset(2);
            it->set_numitems(2);
            it->add_item(0,i);
            it->add_item(1,cnt_ary[i].t2[j].item2);
            it->set_t_utility(cnt_ary[i].t2[j].t_utility);
            Largelist->append(*it);
         }
      }
   }
     
   NUM_INSERT = choose(Largelist->numitems(),2);
//   fprintf(summary, "(2)it = %d ", Largelist->numitems());
printf("k=%d, Largelist->numitems=%d\n", 2, Largelist->numitems());

   if (Largelist->numitems() > 0) max_pattern_length = 2;

//ListElement *L1iter = Largelist->node(0);
//for (int j=0; j <  Largelist->numitems(); j++, L1iter = L1iter->next())
//  printf("i=%d, j=%d\n",L1iter->item()->item(0), L1iter->item()->item(1));

Itemset *temp_2 = new Itemset(2);
ListElement *L1iter = Largelist->node(0);
for (int j=0; j <  Largelist->numitems(); j++, L1iter = L1iter->next()){
  Itemset *temp = L1iter->item();
  Itemset *temp_2 = new Itemset(2);
  temp_2->copy(temp);
  Total_Cand_Largelist->append(*temp_2);
}
                                   
   hash_function = get_hash_function(NUM_INSERT,2);
   Candidate = new HashTree(0, hash_function, threshold);

#ifdef BALT
   more = (NUM_INSERT > 0);
   form_hash_indx(hash_function);
#endif
//   fprintf (summary," %f ", te-ts);

   int blk = num_trans;
   int lb = 0;
   int ub = num_trans;

   hash_function = get_hash_function(NUM_INSERT,2);

   more = (Largelist->numitems() > 1);
   last_tot_cand = tot_cand;
   
   for(k=3;more;k++){
      if (hash_pos) delete [] hash_pos;
      hash_pos = (int *) calloc((k+1)*hash_function, sizeof(int));
      vist = 1;

      //form new candidates

      NUM_ACTUAL_INSERT = apriori_gen(k);

      printf("k=%d, NUM_ACTUAL_INSERT=%d\n", k, NUM_ACTUAL_INSERT);
#ifndef EQ
      Largelist->clearlist();
#endif
      if (oldCand) delete oldCand;
      
      more = (last_tot_cand < tot_cand);
printf("more =%d, last_tot_cand=%d, tot_cand=%d\n", more, last_tot_cand, tot_cand);
      if (more){
         num_db_scan++;
         reset_database(DCB);
         get_first_blk(DCB);
         for(i=lb; i < ub;i++){
            get_next_trans(DCB, buf, numitem, tid);
            make_Itemset(trans, buf, numitem, tid);
            for (j=0; j < trans->numitems(); j++)
                trans_bitvec[trans->item(j)] = 1;
            subset(trans, 0, trans->numitems()-k+1, trans->numitems(),
                   Candidate,
                   k, 0, trans_bitvec, &vist, hash_function);
            for (j=0; j < trans->numitems(); j++)
               trans_bitvec[trans->item(j)] = 0;
         }
      }
      for (i=0; i < maxitem; i++) hash_indx[i] = -1;
      form_large(Candidate,k);
printf("k=%d, Largelist->numitems=%d\n", k, Largelist->numitems());
      
      Itemset *temp_2 = new Itemset(k);
      ListElement *L1iter = Largelist->node(0);
      for (int j=0; j <  Largelist->numitems(); j++, L1iter = L1iter->next()){
          Itemset *temp = L1iter->item();
          Itemset *temp_2 = new Itemset(k);
          temp_2->copy(temp);
          Total_Cand_Largelist->append(*temp_2);
      }
//      fprintf(summary, "(%d)it = %d ", k, Largelist->numitems());
      
      more = (Largelist->numitems() > 1);
      last_tot_cand = tot_cand;

      if (Largelist->numitems() > 0) max_pattern_length = k;

      NUM_INSERT = choose(Largelist->numitems(),2);
#ifdef BALT
      form_hash_indx(get_hash_function(NUM_INSERT,k+1));
#endif
      oldCand = Candidate;
      Candidate = new HashTree(0, get_hash_function(NUM_INSERT,k+1),
                               threshold);
      hash_function = get_hash_function(NUM_INSERT,k+1);
//      fprintf(summary," %f ", t4-t3);
   }
 
   delete [] trans_bitvec;
   delete trans;
   for (i=0; i<maxitem; i++)
      free(t_utilitycnt[i].t2);
   free(t_utilitycnt);
//   close_DCB(DCB);
//   fprintf(summary,"MAIN %f ", t2-t1);
}

int main(int argc, char **argv)
{
    int transaction_file, profit_file;
    summary = fopen("out", "a+");
    int indx, i, k, j, m, n, *buf, numitem, tid;
    float val;
    ListElement *L1iter;
    int total_real_high;
    Itemset *trans=NULL;
    char *tbitvec;
    double t1, t2, t3;

    if (argc < 4){
        cout << "usage: <transaction_file> <profit_file> <utility_threshold>\n";
        exit(3);
    }
                      
     strcpy(transaction_file_name, argv[1]);
     strcpy(profit_file_name, argv[2]);
    
     transaction_file = open(transaction_file_name, O_RDONLY);
seconds(t1); 
//phase I begin
//建立HeadTable，IHUP，并挖掘
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
     

     Total_Cand_Largelist = new ListItemset;
     
     more =1;

     start = new int [MAXITER];
     enda = new int [MAXITER];
     main_proc();
seconds(t2);     
//phase II begin
//进行验证工作

    
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


