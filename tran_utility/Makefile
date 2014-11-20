CC  =  g++ -fopenmp
CFLAGS  =  -O2 -g  
OBJS	= ListItemset.o
OBJS2   = Database.o Itemset.o HashTree.o
OPTS	= -DBALT -DOPTIMAL 
LIBS = -lm 

TARGET  = utility_mine 

default:	$(TARGET)

clean:
	rm -rf *~ *.o $(TARGET)

utility_mine: utility.cc $(OBJS) $(OBJS2) 
	$(CC) $(CFLAGS) $(OPTS)\
	-o utility_mine utility.cc $(OBJS) $(OBJS2) $(LIBS)

Database.o: Database.cc Database.h
	$(CC) $(CFLAGS) $(OPTS) -c Database.cc

Itemset.o: Itemset.cc Itemset.h 
	$(CC) $(CFLAGS) $(OPTS) -c Itemset.cc

ListItemset.o: ListItemset.cc ListItemset.h
	$(CC) $(CFLAGS) -c ListItemset.cc

HashTree.o: HashTree.cc HashTree.h
	$(CC) $(CFLAGS) $(OPTS) -c HashTree.cc

