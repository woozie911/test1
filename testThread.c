#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>


long long a = 0;
long long b = 0;

// 获取调度策略的函数
static int get_thread_policy(pthread_attr_t *attr)
{
    int policy;
    int rs = pthread_attr_getschedpolicy(attr, &policy);
    // assert(rs == 0);
    switch (policy)
    {
    case SCHED_FIFO:
        printf("policy= SCHED_FIFO\n");
        break;
    case SCHED_RR:
        printf("policy= SCHED_RR");
        break;
    case SCHED_OTHER:
        printf("policy=SCHED_OTHER\n");
        break;
    default:
        printf("policy=UNKNOWN\n");
        break;
    }
    return policy;
}

// 将线程绑定到某个cpu核上
int attach_cpu(int cpu_index)
{
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    if (cpu_index < 0 || cpu_index >= cpu_num)
    {
        printf("cpu index ERROR!\n");
        return -1;
    }
    
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_index, &mask);

    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
        printf("set affinity np ERROR!\n");
        return -1;
    }

    return 0;
}

// 线程函数1
void *thread1(void *param)
{
    attach_cpu(0);

    long long i;
    for (i = 0; i < 10000000000; i++)
    {
        a++;
    }
}

// 线程函数2
void *thread2(void *param)
{
    attach_cpu(0);

    long long i;
    for (i = 0; i < 10000000000; i++)
    {
        b++;
    }
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    pthread_attr_t attr1;
    pthread_attr_t attr2;
    struct sched_param param1;
    struct sched_param param2;

    //1、初始化
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);

    //2、自己决定调度策略
    pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);

    //3、设置调度策略
    pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);
    pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
    // 获取调度策略
    get_thread_policy(&attr1);
    get_thread_policy(&attr2);

    //4、设置优先级
    param1.sched_priority = 10;// 线程1设置优先级为10
    param2.sched_priority = 12;// 线程2设置优先级为12
    pthread_attr_setschedparam(&attr1, &param1);
    pthread_attr_setschedparam(&attr2, &param2);

    //5、创建线程
    int i=pthread_create(&t1, &attr1, thread1, NULL);
    if (i!=0)
    {
        printf("%d\n",i);
        perror("thread created failed");
    }
    
    sleep(1);
    pthread_create(&t2, &attr2, thread2, NULL);

    while (1)
    {
        printf("a=%lld, b=%lld\n", a, b);
        sleep(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}