/* insertd.c - insertd */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  insertd  --  insert process pid in delta list "head", given its key
 *------------------------------------------------------------------------
 */
int insertd(int pid, int head, int key)
{
	int	next;			/* runs through list		*/
	int	prev;			/* follows next through list	*/

/*	if (proctab[pid].isrealtime == 1)
	{
	for(prev=head,next=rtq[head].qnext ;
	    rtq[next].qkey < key ; prev=next,next=rtq[next].qnext)
		key -= rtq[next].qkey;
	rtq[pid].qnext = next;
	rtq[pid].qprev = prev;
	rtq[pid].qkey  = key;
	rtq[prev].qnext = pid;
	rtq[next].qprev = pid;
	if (next < NPROC)
		rtq[next].qkey -= key;
	}
	else
	{*/
	for(prev=head,next=q[head].qnext ;
	    q[next].qkey < key ; prev=next,next=q[next].qnext)
		key -= q[next].qkey;
	q[pid].qnext = next;
	q[pid].qprev = prev;
	q[pid].qkey  = key;
	q[prev].qnext = pid;
	q[next].qprev = pid;
	if (next < NPROC)
		q[next].qkey -= key;
	//}
	return(OK);
}
