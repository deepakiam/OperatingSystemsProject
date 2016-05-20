#include <kernel.h>
#include <lock.h>
#include <proc.h>

int releaseall(numlocks, ldes)
	int	numlocks;			/* number of args that follow	*/
	long	ldes;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	STATWORD ps;
	struct lentry *lptr;
	int i,reader,rprio=0,writer,wprio=0,running=0;
	unsigned long	*a;		/* points to list of args	*/
	int ret=OK;
	disable(ps);
	
//		kprintf("ldes %d\n", ldes);
		int array[numlocks];
		int narray[numlocks];
		a = (unsigned long *)(&ldes) + (numlocks - 1);
		int temp = numlocks;
		for ( ; temp > 0 ; temp--)	
		{
			narray[temp-1] =  *a--;
			array[temp-1] =  narray[temp-1] % NLOCKS;
		}
	
		for(temp = numlocks; temp>0; temp--)
		{
		int k, flag=0;
		for (k=0;k<5; k++)
		{
			if (proctab[currpid].locks[k] == array[temp-1])
				flag = 1;
		}
			if (flag == 0)
				ret = SYSERR;
		}
		for ( ; numlocks > 0 ; numlocks--)	
		{
			ldes = array[numlocks -1];
//			kprintf("ldes %d\n", ldes);
		ldequeue(ldes, currpid, proctab[currpid].ltype);
		if (proctab[currpid].ltype == READ)
		{
			int temp=0;
			locks[ldes].activer--;
			if (locks[ldes].activer < 0)
				locks[ldes].activer=0;
			locks[ldes].reader--;
			for(i=0;i<locks[ldes].index; i++)
			{
				if (locks[ldes].type[i] == READ && locks[ldes].activer > 0)
					running = 1;
				if (locks[ldes].type[i] == READ && locks[ldes].prio[i] > temp)
					temp = locks[ldes].prio[i];
			}
			locks[ldes].readerp = temp;
		}
		else if(proctab[currpid].ltype = WRITE)
		{
			int temp=0;
			locks[ldes].writer--;
			for(i=0;i<locks[ldes].index; i++)
			{
				
				if (locks[ldes].type[i] == WRITE && locks[ldes].prio[i] > temp)
					temp = locks[ldes].prio[i];
			}
			locks[ldes].writerp = temp;
		}
				int k;
			for (k=0;k<5;k++)
	{
		if (proctab[currpid].locks[k] == ldes)
		{
			proctab[currpid].locks[k] = 100;
			break;
		}
	}
		
//	kprintf("writer %d, reader %d, writerp %d, readerp %d, activer %d, running %d, lock %d\n", locks[ldes].writer, locks[ldes].reader, locks[ldes].writerp, locks[ldes].readerp, locks[ldes].activer, running, ldes);
	
	if (running == 0)
	if (locks[ldes].writer > 0 || locks[ldes].reader > 0)
	{
		//kprintf("reader or writer present\n");
		if (locks[ldes].writerp >= locks[ldes].readerp)
		{
			for(i=0;i<locks[ldes].index; i++)
			{
				if(locks[ldes].prio[i] == locks[ldes].writerp && locks[ldes].type[i] == WRITE)
				{
					ready(locks[ldes].pid[i], RESCHNO);
					//locks[ldes].writer =locks[ldes].writer - 1;
					break;
				}
			}
		}
		else
		{
			int start=0, end=0, count=0,flag=0;
//			kprintf("in complicated loop %d\n", locks[ldes].index);
			for(i=0; i<=locks[ldes].index; i++)
			{

				if (locks[ldes].type[i] == READ && count == 0)
				{
					start = i;
					count++;
				}
				else if(locks[ldes].type[i] == READ)
				{
					count++;
				}
				else if (locks[ldes].type[i] == WRITE)
				{
					end = i-1;
					count = 0;
				}
				else if (locks[ldes].type[i] != READ || locks[ldes].type[i] != WRITE)
				{
					end = i-1;
				}
			}

			for(i=start; i<= end; i++)
			{
				proctab[locks[ldes].pid[i]].pwaitret = OK;
				ready(locks[ldes].pid[i], RESCHNO);
			}
		}
	}
			else
			{
				//kprintf("idle lock\n");
				locks[ldes].currop = 0;
			}
		}

	restore(ps);
	return ret;
}