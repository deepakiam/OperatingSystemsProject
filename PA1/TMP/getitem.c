/* getitem.c - getfirst, getlast, getfirstrt, getlastrt */

#include <conf.h>
#include <kernel.h>
#include <q.h>

/*------------------------------------------------------------------------
 * getfirst  --	 remove and return the first process on a list
 *------------------------------------------------------------------------
 */
int getfirst(int head)
{
	int	proc;			/* first process on the list	*/

	if ((proc=q[head].qnext) < NPROC)
		return( dequeue(proc) );
	else
		return(EMPTY);
}



/*------------------------------------------------------------------------
 * getlast  --  remove and return the last process from a list
 *------------------------------------------------------------------------
 */
int getlast(int tail)
{
	int	proc;			/* last process on the list	*/

	if ((proc=q[tail].qprev) < NPROC)
		return( dequeue(proc) );
	else
		return(EMPTY);
}

/*-----------------------------------------------------------------------------------
 * getfirstrt  --	 remove and return the first process on a real time list
 *-----------------------------------------------------------------------------------
 */
int getfirstrt(int head)
{
	int	proc;			/* first process on the list	*/

	if ((proc=rtq[head].qnext) < NPROC)
		return( dequeue(proc) );
	else
		return(EMPTY);
}



/*----------------------------------------------------------------------------------
 * getlastrt  --  remove and return the last process from a real time list
 *----------------------------------------------------------------------------------
 */
int getlastrt(int tail)
{
	int	proc;			/* last process on the list	*/

	if ((proc=rtq[tail].qprev) < NPROC)
		return( dequeue(proc) );
	else
		return(EMPTY);
}
