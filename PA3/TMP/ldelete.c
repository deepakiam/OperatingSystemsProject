#include <lock.h>
#include <kernel.h>
#include <proc.h>

int ldelete(int lockdesc)
{
	int j;
	STATWORD ps;
	disable (ps);
	lockdesc = lockdesc % NLOCKS;
	locks[lockdesc].status = KILLED;
	//kprintf("made status %dof lock %d\n", locks[lockdesc].status, lockdesc);
	locks[lockdesc].reader=0;
	locks[lockdesc].writer=0;
	locks[lockdesc].currop=-1;
	
	locks[lockdesc].readerid=-1;	//pid of reader with highest priority
	locks[lockdesc].readerp=0;	//highest priority of a reader
	locks[lockdesc].writerid=-1;	//pid of writer with highest priority
	locks[lockdesc].writerp=0;	//highest priority of a writer
	
	locks[lockdesc].head=0;		//head
	locks[lockdesc].tail=0;		//tail
	
	
	for (j=0;j<5; j++)
			{
//		if (locks[lockdesc].pid[j] > 0)
//			kprintf("state %d pid %d\n",proctab[locks[lockdesc].pid[j]].pstate, locks[lockdesc].pid[j]);
				if (locks[lockdesc].pid[j] != -1  && proctab[locks[lockdesc].pid[j]].pstate == 7)
				{
					//kprintf("state %d\n",proctab[locks[lockdesc].pid[j]].pstate);
					//kprintf("pid %d\n",locks[lockdesc].pid[j]);
					proctab[locks[lockdesc].pid[j]].pwaitret = DELETED;
					ready(locks[lockdesc].pid[j], RESCHNO);
				}
				locks[lockdesc].pid[j]=-1;
				locks[lockdesc].prio[j]=-1;
				locks[lockdesc].time[j]=-1;
				locks[lockdesc].type[j]=-1;
			}
	restore(ps);
	return OK;
}