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
extern char transaction_file_name[100], profit_file_name[100];//���׼�¼�ļ�����������ļ���
extern float *profit_array, *transaction_utility_array;//��Ʒ��ֵ��ÿ�����׼�ֵ��
extern int num_trans;//��������
extern int maxitem;//��Ʒ����
extern int avg_trans_sz;//ƽ�����׳���

extern double *item_t_utility;//��Ʒ��TWUֵ
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
}item_1;//��ΪIHUP�Ľڵ�

typedef struct item_rank_2{
   double t_utility;
   int item2;
   item_rank_1 *linknode;
}item_2;//��ΪHeadTable�Ľڵ�

//typedef struct itemset_length_2 {
//   int count;
//   int size;
//   item_2 *t2;
//} itemset2;
#endif

//#ifdef __cplusplus   
//   }
//#endif

