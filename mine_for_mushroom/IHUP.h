#ifndef __IHUP_H
#define __IHUP_H
#include <malloc.h>
#include <iostream>
using namespace std;

#include "pardhp.h"
#include "HeadTable.h"

class IHUP {
public:
   
   IHUP (HeadTable& ht);
   ~IHUP();

   void clear(item_1* root);
   void insert(item_1* root, int tid,  int* item_list, const HeadTable &ht);
   item_1* getRoot();
     item_1* root;
private:
  
    

};

#endif //__IHUP_H

