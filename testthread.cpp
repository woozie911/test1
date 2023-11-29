#include<iostream>
#include<cstdlib>
#include<thread>

long long a=0;
long long b=0;

void handler1()
{
    long long i;
    for (i = 0; i < 10000000000; i++)
    {
        a++;
    }
    
}

void handler2()
{
    long long i;
    for (i = 0; i < 10000000000; i++)
    {
        b++;
    }
    
}


int main()
{
    std::thread t1(handler1);
    
    std::thread t2(handler2);
    
    int priority1=10;
    int priority2=13;
    // 在设置线程函数之前设置线程优先级
    if (pthread_setschedprio(t1.native_handle(), priority1) != 0) {
        std::cerr << "Failed to set priority for thread 1.\n";
    }
    if (pthread_setschedprio(t1.native_handle(), priority2) != 0) {
        std::cerr << "Failed to set priority for thread 2.\n";
    }


    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout<<"a:"<<a<<" b:"<<b<<std::endl;
    }
    t1.join();
    t2.join();
    return 0;
    
}