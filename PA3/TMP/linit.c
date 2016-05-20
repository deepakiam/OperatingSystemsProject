#include <lock.h>

struct	lentry	locks[NLOCKS]; /* process table			*/
struct	lqent	lq[NLOCKS];

void linit()
{
	int i;
	
	for (i=0;i<NLOCKS; i++)
	{
		
		locks[i].status = KILLED;
		//locks[i].desc=-1;
		locks[i].count=0;
		locks[i].reader=0;
		locks[i].writer=0;
		locks[i].currop=0;
		
		locks[i].readerid=0;
		locks[i].readerp=0;
		locks[i].writerid=0;
		locks[i].writerp=0;
		
		locks[i].activer = 0;

		
	}
	
}