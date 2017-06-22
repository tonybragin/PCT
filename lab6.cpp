#include <iostream>
#include <thread>
#include <atomic>
#include <ctime>
using namespace std;

thread pot[20], pro[20];
int BUF_SIZE = 16;
int BUF_MASK = BUF_MASK - 1;
atomic<int> idxIN;
atomic<int> idxOUT;
atomic<double> buffer[100];
double sum = 0;

void at_init()
{
    idxIN = 0;
    idxOUT = 0;
    for (int i = 0; i < BUF_SIZE; i++) {
        buffer[i] = 0;
    }
}

void potr()
{
    if (buffer[idxOUT] == 0)
        sleep(1);
    sum += buffer[idxOUT];
    buffer[idxOUT++] = 0;
    idxOUT = idxOUT % BUF_MASK;
    
}

void proiz()
{
    double z = rand() + 0.13 * rand();
    buffer[idxIN++] = z;
    idxIN = idxIN % BUF_MASK;
}

int main()
{
    at_init();
    for (int i = 10; i < 21; i++) {
        int n = i;
        unsigned int start_time =  clock();
        for (int i = 0; i < n; i++) {
            pot[i] = thread(potr);
            pro[i] = thread(proiz);
        }
    
        for (int i = 0; i < n; i++) {
            if (pot[i].joinable()) {
                pot[i].join();
            }
            if (pro[i].joinable()) {
                pro[i].join();
            }
        }
        unsigned int end_time = clock();
        cout << "\n" << n << "\n" << sum << "\n" << (end_time - start_time) / 1000.00000000 << "\n";
        
    }
    
    return 0;
}