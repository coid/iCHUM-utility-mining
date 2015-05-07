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
extern char transaction_file_name0[100], transaction_file_name1[100], transaction_file_name2[100], profit_file_name[100];//���׼�¼�ļ�����������ļ���
extern float *profit_array;
extern double *transaction_utility_array;//��Ʒ��ֵ��ÿ�����׼�ֵ��
extern int num_trans;//��������
extern int maxitem;//��Ʒ����
extern int avg_trans_sz;//ƽ�����׳���

extern double *item_t_utility;//��Ʒ��TWUֵ
extern double total_tran_utility;//ȫ���ܹ���utilityֵ
extern float MIN_UTILITY_PER;
extern FILE *summary;
extern int tot_cand;

#define seconds(tm) gettimeofday(&tp,(struct timezone *)0);\
   tm=tp.tv_sec+tp.tv_usec/1000000.0
      
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifndef ITEM_RANK
#define ITEM_RANK

typedef struct item_rank_1{
	bool isRoot;//�Ƿ�Ϊ���ڵ�
	double t_utility;//�ڵ��Ӧ��twuֵ
	int item1;//�ڵ���
        int tf;//�ڵ����
	item_rank_1* parent;//ָ�򸸽ڵ�
	vector< item_rank_1* > children;//ָ����ӽڵ�
        item_rank_1* brother;//��һ������ͬ��ŵĽڵ�
}item_1;//��ΪIHUP�Ľڵ�

typedef struct item_rank_2{
   int item2;//�ڵ���Ʒ���
   double t_utility; //�ڵ��Ӧ��twuֵ
   int tf;//�ڵ���ִ���
   item_rank_1 *linknode;//��Ӧ�����ڵ��һ��λ��
}item_2;//��ΪHeadTable�Ľڵ�

typedef struct item_rank_3{
    double twu;
    int item;
    
}item_3;//����������������ʵ�ּ�¼



struct ct_node{
    double tu_per;
    vector<int> items;//��Ӧ����Ŀ���
    //ct_node* next;
};//�������Ľڵ�

struct ct_head{
    ct_node* first;
    //ct_node* last;
};//������ͷ

 struct itemset {
   int count;//��itemset���ִ���
   vector<int> items;//��Ӧ����Ŀ���
   double twu;//itemset��twuֵ
   double utility;   //��Ӧ��������utilityֵ
} ;

 struct candidate{
    vector<itemset*> itemslist;//��¼��ϣ����itemset
    int item;//�����غ�����Ʒ���
    //int count;//���и���Ʒ��itemset�ĸ���
    //double twu;//����Ʒ��twuֵ
};
#endif

//#ifdef __cplusplus   
//   }
//#endif

