#include <lock.h>
#include <kernel.h>
#include <proc.h>
#include <sleep.h>

int lock(int ldes1, int type, int priority)
{
	STATWORD ps;
	struct pentry *pptr;
	struct lentry *lptr;
	
	disable (ps);
	int ldes = ldes1 %50;
	pptr = &proctab[currpid];
	lptr = &locks[ldes];
	pptr->ltype = type;
	proctab[currpid].pwaitret = OK;
	
/*	if (locks[ldes].status == KILLED)
	{
		kprintf("status in the beginnign is %d\n", locks[ldes].status);
		restore (ps);
		return SYSERR;
	}
*/	if (ldes1 != ldes + (locks[ldes].count * NLOCKS) || locks[ldes].status == KILLED)
	{
		restore (ps);
		return SYSERR;
	}
	
	int k;
	
	for (k=0;k<5;k++)
			{
				if (proctab[currpid].locks[k] == 100)
				{
					proctab[currpid].locks[k] = ldes;
					//kprintf("put the value %d, %d\n",proctab[currpid].locks[k], ldes);
					break;
				}
			}

	
//	kprintf("writer %d, reader %d, writerp %d, readerp %d, activer %d, lock %d\n", locks[ldes].writer, locks[ldes].reader, locks[ldes].writerp, locks[ldes].readerp, locks[ldes].activer, ldes);
	if (lptr->currop == 0)
	{
		lptr->currop = type;
		lenqueue(ldes, currpid, priority, type, ctr1000);
		if (type == READ)
		{
			locks[ldes].reader++;
			locks[ldes].activer++;
		}
		else if (type == WRITE)
			locks[ldes].writer++;
		
	}
	//if read is going on and a write is in queue
	else if (type == READ && locks[ldes].writer > 0 && lptr->currop == READ && locks[ldes].writerp > priority)
	{	
//		kprintf("in 1st read\n");
		lenqueue(ldes, currpid, priority, type, ctr1000);
		locks[ldes].reader = locks[ldes].reader + 1;
		if (priority > lptr->readerp)
		{
			lptr->readerp = priority;
			lptr->readerid = currpid;
		}
		pptr->pstate = PRWAIT;
		resched();
		if (locks[ldes].status == KILLED || ldes1 != ldes + (locks[ldes].count * NLOCKS))
		{
			restore (ps);
			return DELETED;
		}
		else
		{
		restore (ps);
		return (OK);
		}
	}
	else if(type == READ && locks[ldes].writer > 0 && lptr->currop == READ)
	{
//		kprintf("in 2nd read\n");
		lenqueue(ldes, currpid, priority, type, ctr1000);
		locks[ldes].reader = locks[ldes].reader + 1;
		locks[ldes].activer++;
		if (priority > lptr->readerp)
		{
			lptr->readerp = priority;
			lptr->readerid = currpid;
		}
		restore (ps);
		return (OK);
	}
	//if lock is being used and write comes
	else if (type == WRITE)
	{
		lenqueue(ldes, currpid, priority, type, ctr1000);
		lptr->writer = lptr->writer + 1;
		if (priority > lptr->writerp)
		{
			lptr->writerp = priority;
			lptr->writerid = currpid;
		}
		pptr->pstate = PRWAIT;
		resched();
//		kprintf("status %d locks %d\n", locks[ldes].status, ldes);
		if (locks[ldes].status == KILLED || ldes1 != ldes + (locks[ldes].count * NLOCKS))
		{
			restore (ps);
			return DELETED;
		}
		else
		{
		restore (ps);
		return (OK);
		}
	}
	else if(type == READ && locks[ldes].currop == WRITE)
	{
//		kprintf("in read of write\n");
		lenqueue(ldes, currpid, priority, type, ctr1000);
		locks[ldes].reader = locks[ldes].reader + 1;
//		kprintf("in read %d\n", locks[ldes].reader);
		if (priority > locks[ldes].readerp)
		{
			locks[ldes].readerp = priority;
			locks[ldes].readerid = currpid;
		}
//		kprintf("slept\n");
		pptr->pstate = PRWAIT;
		resched();
		if (locks[ldes].status == KILLED || ldes1 != ldes + (locks[ldes].count * NLOCKS))
		{
			restore (ps);
			return DELETED;
		}
		else
		{
		restore (ps);
		return (OK);
		}
	}
	else if(type == READ)
	{
		lenqueue(ldes, currpid, priority, type, ctr1000);
		locks[ldes].reader = locks[ldes].reader + 1;
		locks[ldes].activer++;
//		kprintf("in here\n");
//		kprintf("in read %d\n", locks[ldes].reader);
		if (priority > locks[ldes].readerp)
		{
			locks[ldes].readerp = priority;
			locks[ldes].readerid = currpid;
		}
		restore (ps);
		return OK;
	}
	
//	kprintf("currop %d = %d\n", locks[ldes].currop, type);
	restore(ps);
	return(OK);
}