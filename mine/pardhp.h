//#ifdef __cplusplus
//extern "C" {
//#endif

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <vector>
using namespace std;

extern double MIN_UTILITY;
extern char transaction_file_name[100], profit_file_name[100];//交易记录文件名，利润表文件名
extern float *profit_array, *transaction_utility_array;//物品价值表，每条交易价值表
extern int num_trans;//交易总数
extern int maxitem;//物品总数
extern int avg_trans_sz;//平均交易长度

extern double *item_t_utility;//物品的TWU值
extern float MIN_UTILITY_PER;
extern FILE *summary;
extern int tot_cand;

#define seconds(tm) gettimeofday(&tp,(struct timezone *)0);\
   tm=tp.tv_sec+tp.tv_usec/1000000.0
      
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifndef ITEM_RANK
#define ITEM_RANK

typedef struct item_rank_1{
	bool isRoot;
	double t_utility;
	int item1;
	item_rank_1* parent;
	vector< item_rank_1* > children;
        item_rank_1* brother;
}item_1;//作为IHUP的节点

typedef struct item_rank_2{
   double t_utility;
   int item2;
   item_rank_1 *linknode;
}item_2;//作为HeadTable的节点

//typedef struct itemset_length_2 {
//   int count;
//   int size;
//   item_2 *t2;
//} itemset2;
#endif

//#ifdef __cplusplus   
//   }
//#endif

