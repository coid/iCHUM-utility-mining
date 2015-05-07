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
extern char transaction_file_name0[100], transaction_file_name1[100], transaction_file_name2[100], profit_file_name[100];//交易记录文件名，利润表文件名
extern float *profit_array;
extern double *transaction_utility_array;//物品价值表，每条交易价值表
extern int num_trans;//交易总数
extern int maxitem;//物品总数
extern int avg_trans_sz;//平均交易长度

extern double *item_t_utility;//物品的TWU值
extern double total_tran_utility;//全部总共的utility值
extern float MIN_UTILITY_PER;
extern FILE *summary;
extern int tot_cand;

#define seconds(tm) gettimeofday(&tp,(struct timezone *)0);\
   tm=tp.tv_sec+tp.tv_usec/1000000.0
      
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifndef ITEM_RANK
#define ITEM_RANK

typedef struct item_rank_1{
	bool isRoot;//是否为根节点
	double t_utility;//节点对应的twu值
	int item1;//节点编号
        int tf;//节点次数
	item_rank_1* parent;//指向父节点
	vector< item_rank_1* > children;//指向儿子节点
        item_rank_1* brother;//下一个与它同编号的节点
}item_1;//作为IHUP的节点

typedef struct item_rank_2{
   int item2;//节点物品编号
   double t_utility; //节点对应的twu值
   int tf;//节点出现次数
   item_rank_1 *linknode;//对应的树节点第一个位置
}item_2;//作为HeadTable的节点

typedef struct item_rank_3{
    double twu;
    int item;
    
}item_3;//条件树中用来快速实现记录



struct ct_node{
    double tu_per;
    vector<int> items;//对应的数目编号
    //ct_node* next;
};//条件树的节点

struct ct_head{
    ct_node* first;
    //ct_node* last;
};//条件树头

 struct itemset {
   int count;//该itemset出现次数
   vector<int> items;//对应的数目编号
   double twu;//itemset的twu值
   double utility;   //对应的真正的utility值
} ;

 struct candidate{
    vector<itemset*> itemslist;//记录有希望的itemset
    int item;//该栏必含的物品标号
    //int count;//含有该物品的itemset的个数
    //double twu;//该物品的twu值
};
#endif

//#ifdef __cplusplus   
//   }
//#endif

