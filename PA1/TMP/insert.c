/* insert.c  -  insert */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * insert.c  --  insert an process into a q list in key order
 *------------------------------------------------------------------------
 */
int insert(int proc, int head, int key)
{
	int	next;			/* runs through list		*/
	int	prev;

	/*if (proctab[proc].isrealtime == 1)
	{
	next = rtq[head].qnext;
	while (rtq[next].qkey < key)	/* tail has maxint as key	*/
		/*next = rtq[next].qnext;
	rtq[proc].qnext = next;
	rtq[proc].qprev = prev = rtq[next].qprev;
	rtq[proc].qkey  = key;
	rtq[prev].qnext = proc;
	rtq[next].qprev = proc;
	}
	else
	{*/
	next = q[head].qnext;
	while (q[next].qkey < key)	/* tail has maxint as key	*/
		next = q[next].qnext;
	q[proc].qnext = next;
	q[proc].qprev = prev = q[next].qprev;
	q[proc].qkey  = key;
	q[prev].qnext = proc;
	q[next].qprev = proc;
	//}
	return(OK);
}
