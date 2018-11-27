#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_THREAD 3
_Atomic unsigned long long main_counter, counter[MAX_THREAD];

void *thread_worker(void *);

int main(int argc, char *argv[]) {
  int i, rtn, ch;
  pthread_t pthread_id[MAX_THREAD] = {0};

  for (i = 0; i < MAX_THREAD; i++) {
    /*在这里填写代码，用pthread_create建一个普通的线程，
      线程id存入pthread_id[i]，线程执行函数是thread_worker并i作为参数传递给线程*/
    int *p = (int *)malloc(sizeof(int));
    *p = i;
    pthread_create(&pthread_id[i], NULL, thread_worker, p);
  }

  do { /*用户按一次回车执行下面的循环体一次。按q退出*/ 
    unsigned long long sum = 0;
    /*求所有线程的counter的和*/
    for (i = 0; i < MAX_THREAD; i++) {
      /*求所有counter的和*/
      sum += counter[i];
      printf("%llu ", counter[i]);
    }
    printf("%llu/%llu %s", main_counter, sum, (main_counter < sum) ? "smaller" : "bigger");
  } while ((ch = getchar()) != 'q');
  return 0;
}

void *thread_worker(void *p) {
  int thread_num;

  /*在这里填写代码，把main中的i的值传递给thread_num*/
  thread_num = *(int *)p;
  free(p);

  for (;;) {
    counter[thread_num]++;
    main_counter++;
  }
}