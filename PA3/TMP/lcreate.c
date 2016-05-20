#include <lock.h>
#include <kernel.h>
#include <proc.h>

int lcreate()
{
	STATWORD ps;
	int i,j,k;
	disable (ps);
	
	for (i=0;i<NLOCKS;i++)
	{
		if (locks[i].status == KILLED)
		{
			locks[i].status = ALIVE;
			locks[i].count++;
			locks[i].currop=0;
			locks[i].index=0;
			for (j=0;j<NLOCKS; j++)
			{
				locks[i].pid[j]=-1;
				locks[i].prio[j]=-1;
				locks[i].type[j]=-1;
				locks[i].time[j]=-1;
			}
			
			
//			kprintf("returning %d\n", i);
			restore(ps);
			return (i + (locks[i].count * NLOCKS));
		}
	}
	restore(ps);
	return(SYSERR);
}