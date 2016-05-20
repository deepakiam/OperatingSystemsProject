/* sched.h */

/*Scheduler Constants*/
#define DEFAULT		0		/*Default XINU scheduler	*/
#define LINUXSCHED	1		/*Linux Scheduler associated with 1*/
#define MULTIQSCHED	2		/*Multi Q Scheduler associated with 2*/

extern int epoch;
extern int scheduler;
