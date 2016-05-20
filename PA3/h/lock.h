//#define DELETED  1
#define READ  1
#define WRITE  2

#define NLOCKS 50
#define ALIVE 1
#define KILLED 0

struct lqent{
	int pid;
	int prio;
	int type;
	long time;
	
	struct lqent * lqnext;
};

struct lentry{
	int status;
	//int desc;
	int count;		//to distinguish different instances of lock
	int reader;		//number of readers
	int writer;		//number of writers
	int currop;		//current operation of the lock
	
	int readerid;	//pid of reader with highest priority
	int readerp;	//highest priority of a reader
	int writerid;	//pid of writer with highest priority
	int writerp;	//highest priority of a writer
	
	int activer;
	
	int head;		//head
	int tail;		//tail
	
	int pid[50];	//array of pids
	int prio[50];	//array of priorities
	int type[50];	//array of types
	int time[50];	//array of time
	
	int index;
};

extern struct lentry locks[];
extern struct lqent lq[];
	
int releaseall(int, long, ...);
