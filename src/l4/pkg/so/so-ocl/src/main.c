/******************************************************************/
/*  SOLAR Orbiter DPU System under CCC Framework                  */
/* -------------------------------------------------------------- */
/*  Application   SO-OCL                                          */
/*  Author        Min Xu                                          */
/*  Date          01.05.2014                                      */
/*  Version       1.0                                             */
/* -------------------------------------------------------------- */
/*  Technical University of Braunschweig, Germany                 */
/*  Institute for Computer and Network Engineering (IDA)          */
/* -------------------------------------------------------------- */
/*  Revision History:                                             */
/*  1.0 01052014        File Created                              */
/******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread-l4.h>
#include <l4/sys/ipc.h>
#include <time.h>

sem_t		sema;
int		err_no;

static pthread_t	t1;
static pthread_t	t2;

static void* producer(void *arg)
{
  int sabuff = 0;
  while(1)
  {
    if(sem_getvalue(&sema, &sabuff))
      return(3);
    printf("Produced. Available: %d\n", sabuff+1);
    sleep(2);
    if(sem_post(&sema))
      return(4);
  }
}

static void* customer(void *arg)
{
  int sabuff = 0;
  while(1)
  {
    if(sem_getvalue(&sema, &sabuff))
      return(3);
    printf("Sold out. Available: %d\n", sabuff);
    sleep(1);
    if(sem_wait(&sema))
      return(4);
  }
}

int main(void)
{
  struct timeval	tvalstr;

  if(sem_init(&sema,0,3) != 0)
    return(1);
  if(pthread_create(&t1, NULL, producer, NULL))
    return(2);
  if(pthread_create(&t2, NULL, customer, NULL))
    return(2);

  while(1);
}
