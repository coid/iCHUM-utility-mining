CC  =  g++
CFLAGS  =  -O2 -g  
OBJS	= IHUP.o
OBJS2   = Database.o HeadTable.o
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

HeadTable.o: HeadTable.cc HeadTable.h 
	$(CC) $(CFLAGS) $(OPTS) -c HeadTable.cc

IHUP.o: IHUP.cc IHUP.h
	$(CC) $(CFLAGS) -c IHUP.cc

