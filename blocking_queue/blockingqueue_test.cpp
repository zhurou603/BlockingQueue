#include "blockingqueue.h"

#include<iostream>
#include <unistd.h>
#include <thread>

using namespace std;

//global queue
BlockingQueue<int> q(3);
float speed_produce = 1;
float speed_consumer = speed_produce * 5;

void produce(){
    int id = 0;
    while(true){
        q.Push(id);
        cout << "Push:" << id << endl;
        id++;
        sleep(speed_produce);        
    }
}

void consume(){
    while(true){
        int data = q.Pop();
        cout << "Pop:" << data << endl;
        sleep(speed_consumer);
    }
}

int main(int argc, char **argv){
    thread t1(produce);
    thread t2(consume);
    t1.join();
    t2.join();
    return 0;
}