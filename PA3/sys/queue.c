/* queue.c - dequeue, enqueue */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <lock.h>

/*------------------------------------------------------------------------
 * enqueue  --	insert an item at the tail of a list
 *------------------------------------------------------------------------
 */
int enqueue(int item, int tail)
/*	int	item;			- item to enqueue on a list	*/
/*	int	tail;			- index in q of list tail	*/
{
	struct	qent	*tptr;		/* points to tail entry		*/
	struct	qent	*mptr;		/* points to item entry		*/

	tptr = &q[tail];
	mptr = &q[item];
	mptr->qnext = tail;
	mptr->qprev = tptr->qprev;
	q[tptr->qprev].qnext = item;
	tptr->qprev = item;
	return(item);
}


/*------------------------------------------------------------------------
 *  dequeue  --  remove an item from the head of a list and return it
 *------------------------------------------------------------------------
 */
int dequeue(int item)
{
	struct	qent	*mptr;		/* pointer to q entry for item	*/

	mptr = &q[item];
	q[mptr->qprev].qnext = mptr->qnext;
	q[mptr->qnext].qprev = mptr->qprev;
	return(item);
}


/*------------------------------------------------------------------------
 * enqueue  --	insert an item at the tail of a list
 *------------------------------------------------------------------------
 */

int lenqueue(int ldes, int pid, int priority, int type, long time)
{
	ldes = ldes % NLOCKS;
		int i = locks[ldes].index;
		locks[ldes].pid[i] = pid;
		locks[ldes].prio[i] = priority;
		locks[ldes].time[i] = time;
		locks[ldes].type[i] = type;
		locks[ldes].index = ++i;
//		kprintf("enqueued i %d, pid %d, priority %d, type %d, index %d\n", i, pid, priority, type, locks[ldes].index);
//		kprintf("enqueued time %d\n", time);
		return OK;
}


/*------------------------------------------------------------------------
 *  dequeue  --  remove an item from the head of a list and return it
 *------------------------------------------------------------------------
 */

int ldequeue(int ldes, int pid, int type)
{
	ldes = ldes % NLOCKS;
	int i;
	for (i=0; i<locks[ldes].index; i++)
	{
//		kprintf("enqueued i %d, pid %d, ldes %d, type %d, index %d\n", i, pid, ldes, type, locks[ldes].index);
		if (locks[ldes].pid[i] == pid)
		{
//			kprintf("dequeued %d\n", i);
			locks[ldes].pid[i] = -1;
			locks[ldes].prio[i] = 0;
			locks[ldes].time[i] = 0;
			locks[ldes].type[i] = 0;
			return OK;
		}
	}
	return SYSERR;
}

