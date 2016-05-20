/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sched.h>
#include <stdio.h>
//#include <time.h>

unsigned long currSP;   /* REAL sp of current process */
extern int scheduler;
extern int epoch;
extern int bestproc;
extern int goodness[NPROC];
extern int urgency = 0;
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:       Upon entry, currpid gives current process id.
 *              Proctab[currpid].pstate gives correct NEXT state for
 *                      current process if other than PRREADY.
 *-----------------------------------------------------------------------
 */
int resched()
{
        register struct pentry  *optr;  /* pointer to old process entry */
        register struct pentry  *nptr;  /* pointer to new process entry */
        register struct pentry  *aptr;  /* general pointer to a process */
                
			//preepmt = 0;
        if (scheduler == LINUXSCHED)
        {

        int i;
        if (epoch <= 0)
        {
                epoch = 0;
                /*int runproc = 0;
                (optr = &proctab[0])->pprio = 0;
                (optr = &proctab[0])->counter = 0;
                (optr = &proctab[0])->inepoch = 0;*/
                for (i=1;i<NPROC;i++)
                {
                        /*if ((aptr = &proctab[i])->pstate == PRFREE)
                        {
                                (optr = &proctab[i])->pprio = 0;
                                (optr = &proctab[i])->counter = 0;
                                (optr = &proctab[i])->inepoch = 0;
                        }*/
					
                        if ((optr = &proctab[i])->pstate == PRCURR || (optr = &proctab[i])->pstate == PRREADY)
                        {
                                //kprintf("process, counter, priority, goodness, bestproc :- %d, %d, %d, %d, %d\n", i, (optr = &proctab[i])->counter, (optr = &proctab[i])->pprio, goodness[i], bestproc);
                                epoch = epoch + (optr = &proctab[i])->pprio;
                             // kprintf("epoch is %d",epoch);
                                (optr = &proctab[i])->counter += (optr = &proctab[i])->pprio;
                                goodness[i] = (optr = &proctab[i])->counter + (optr = &proctab[i])->pprio;

                                if (goodness[i] > goodness[bestproc])
                                        bestproc = i;
                                (optr = &proctab[i])->inepoch = 1;
					//		kprintf("process %d, ",i);
                                //runproc = 1;
                                //kprintf("process, counter, priority, goodness, bestproc :- %d, %d, %d, %d, %d\n", i, (optr = &proctab[i])->counter, (optr = &proctab[i])->pprio, goodness[i], bestproc);
                        }
                        else
                                goodness[i] = -1;
					
                }
			//kprintf("is in epoch\n");
                /*if (runproc == 0)
                        kill(0);*/
        }
        
        for (i=1; i<NPROC; i++)
        {
                if ((goodness[i]>goodness[bestproc]) && ((aptr = &proctab[i])->inepoch == 1))
                {
                        bestproc = i;
					
					//kprintf("process %d is in epoch\n",i);
//                      kprintf("reversed from %d to %d",currpid, bestproc);
                }
        }
//	kprintf("process, counter, priority, goodness, bestproc, epoch:- %d, %d, %d, %d, %d, %d\n", currpid, (optr = &proctab[currpid])->counter, (optr = &proctab[currpid])->pprio, goodness[currpid], bestproc, epoch);
        
        
        /* no switch needed if current process priority higher than next*/
        if ((currpid == bestproc) && ((optr = &proctab[currpid])->counter > 0)) {
                if ((optr = &proctab[currpid])->counter > QUANTUM)
                {
                        (optr = &proctab[currpid])->counter = (optr = &proctab[currpid])->counter - QUANTUM;
                        goodness[currpid] = goodness[currpid] - QUANTUM;
					#ifdef RTCLOCK
                        preempt = QUANTUM;
					#endif                        
                        epoch = epoch - QUANTUM;
                }
                else
                {
#ifdef RTCLOCK
                        preempt = (optr = &proctab[currpid])->counter;
#endif       
                        epoch = epoch - (optr = &proctab[currpid])->counter;
                        (optr = &proctab[currpid])->counter = 0;
                        goodness[currpid] = 0;
                        (optr = &proctab[currpid])->inepoch = 0;
                }
               // kprintf("process, counter, priority, goodness, bestproc, epoch, inepoch:- %d, %d, %d, %d, %d, %d, %d\n", currpid, (optr = &proctab[currpid])->counter, (optr = &proctab[currpid])->pprio, goodness[currpid], bestproc, epoch, (nptr = &proctab[bestproc])->inepoch);
//              kprintf("did not change reduced to %d\n", epoch);

                return(OK);
        }

                /* force context switch */
                
                if (optr->pstate == PRCURR) {
                optr->pstate = PRREADY;
                insert(currpid,rdyhead,goodness[currpid]);
        }
        /* remove highest priority process at end of ready list */
        optr = &proctab[ currpid ];
        nptr = &proctab[ bestproc ];
        if (nptr->counter > QUANTUM)
        {
                nptr->counter = nptr->counter - QUANTUM;
                goodness[bestproc] = goodness[bestproc] - QUANTUM;
#ifdef  RTCLOCK
                preempt = QUANTUM;              /* reset preemption counter     */
#endif
                epoch = epoch - QUANTUM;
        }
        else
        {
#ifdef  RTCLOCK
                preempt = nptr->counter;              /* reset preemption counter     */
#endif
                epoch = epoch - nptr->counter;
                nptr->counter = 0;
                goodness[bestproc] = 0;
                (nptr = &proctab[bestproc])->inepoch = 0;
        }
        currpid = bestproc;
        // if (currpid != 0)
        //      kprintf("currpid, goodness[currpid], nptr->counter, nptr->pstate :- %d, %d, %d, %s", currpid, goodness[currpid], nptr->counter, nptr->pstate);
        nptr->pstate = PRCURR;          /* mark it currently running    */


        nptr = &proctab[ (currpid = dequeue(bestproc)) ];
        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
//     kprintf("changed to %d reduced to %d\n", currpid, epoch);
//      kprintf("\nNEW PROCESS :- process, counter, priority, goodness, bestproc, epoch, inepoch :- %d, %d, %d, %d, %d, %d, %d\n", currpid, (nptr = &proctab[currpid])->counter, (optr = &proctab[currpid])->pprio, goodness[currpid], bestproc, epoch, (nptr = &proctab[bestproc])->inepoch);

        /* The OLD process returns here when resumed. */
        return OK;
}
        
        else if (scheduler == MULTIQSCHED)
        {
			
            int i=0, bestprocrt=0, bestprocn=0;
			int quantum;
        	if (epoch <= 0)
        	{
				epoch = 0;
				int random = rand() % 10;
				if (random<=3)
					urgency = 0;
				else if(random > 3 && random <10)
					urgency = 1;
				
				for (i = 1; i<NPROC; i++)
				{
					if (urgency)	
					{
						if ((aptr = &proctab[i])->isrealtime == 1 )
						{
							//kprintf("real time urgency %d for random %d",urgency, random);
							(aptr = &proctab[i])->counter = 100;
							(aptr = &proctab[i])->inepoch = 1;
							goodness[i] = 200;
							epoch = epoch + 100;
							bestproc = i;
							quantum = 100;
						}
					}
					else
					{
						if ((aptr = &proctab[i])->isrealtime == 0 )
						{
							//kprintf("normal urgency %d for random %d",urgency, random);
							(aptr = &proctab[i])->counter = (aptr = &proctab[i])->pprio;
							goodness[i] = ((aptr = &proctab[i])->counter * 2);
							epoch = epoch + (aptr = &proctab[i])->counter;
							(aptr = &proctab[i])->inepoch = 1;
							if (goodness[i] > goodness[bestproc])
								bestproc = i;
							quantum = 10;
						}
					}
					goodness[i] = -1;
				}
			}
			
			
			for (i =0 ; i < NPROC; i++)
			{
				if (urgency == 1)	
				{
					if (((aptr = &proctab[i])->inepoch == 1) && ((aptr = &proctab[i])->isrealtime == 1))
						bestproc = i;
				}
				else
				{
					if ((goodness[i]>goodness[bestproc]) && ((aptr = &proctab[i])->inepoch == 1) && ((aptr = &proctab[i])->isrealtime != 1))
                	{
                        bestproc = i;
                	}	
				}
			}
			
						        /* no switch needed if current process priority higher than next*/
	        if ((currpid == bestproc) && ((optr = &proctab[currpid])->counter > 0)) {
                if ((optr = &proctab[currpid])->counter > quantum)
                {
                        (optr = &proctab[currpid])->counter = (optr = &proctab[currpid])->counter - quantum;
                        goodness[currpid] = goodness[currpid] - quantum;
					#ifdef RTCLOCK
                        preempt = quantum;
					#endif                        
                        epoch = epoch - quantum;
                }
                else
                {
#ifdef RTCLOCK
                        preempt = (optr = &proctab[currpid])->counter;
#endif       
                        epoch = epoch - (optr = &proctab[currpid])->counter;
                        (optr = &proctab[currpid])->counter = 0;
                        goodness[currpid] = 0;
                        (optr = &proctab[currpid])->inepoch = 0;
                }
               // kprintf("process, counter, priority, goodness, bestproc, epoch, inepoch:- %d, %d, %d, %d, %d, %d, %d\n", currpid, (optr = &proctab[currpid])->counter, (optr = &proctab[currpid])->pprio, goodness[currpid], bestproc, epoch, (nptr = &proctab[bestproc])->inepoch);

                return(OK);
        }
			
					                if (optr->pstate == PRCURR) {
                optr->pstate = PRREADY;
                insert(currpid,rdyhead,goodness[currpid]);
        }
		
			        /* remove highest priority process at end of ready list */
        optr = &proctab[ currpid ];
        nptr = &proctab[ bestproc ];
        if (nptr->counter > quantum)
        {
                nptr->counter = nptr->counter - quantum;
                goodness[bestproc] = goodness[bestproc] - quantum;
#ifdef  RTCLOCK
                preempt = quantum;              /* reset preemption counter     */
#endif
                epoch = epoch - quantum;
        }
        else
        {
#ifdef  RTCLOCK
                preempt = nptr->counter;              /* reset preemption counter     */
#endif
                epoch = epoch - nptr->counter;
                nptr->counter = 0;
                goodness[bestproc] = 0;
                (nptr = &proctab[bestproc])->inepoch = 0;
        }
        currpid = bestproc;
        nptr->pstate = PRCURR;          /* mark it currently running    */

    // kprintf("\nNEW PROCESS :- process, counter, priority, goodness, bestproc, epoch, inepoch :- %d, %d, %d, %d, %d, %d, %d\n", currpid, (nptr = &proctab[currpid])->counter, (optr = &proctab[currpid])->pprio, goodness[currpid], bestproc, epoch, (nptr = &proctab[bestproc])->inepoch);


        nptr = &proctab[ (currpid = dequeue(bestproc)) ];
        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);


			return(OK);
        }
      
        else
        {
            /* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK; 
        }
}

void setschedclass(int sched_class)
{
        scheduler = sched_class;
}

int getschedclass()
{
        return scheduler;
}
