#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

int n = 1000;
int *a = new int[n];
int step = n / 2;

void printa()
{
    for (int i = 0; i < n; i++) {
        cout << a[i] << ' ';
    }
    cout << "\n";
}

int change_temp(int j)
{
    int temp = a[j];
    a[j] = a[j + step];
    a[j + step] = temp;
    j--;
    
    return j;
}



void shell_temp()
{
    for (int i = 0; i < (n - step); i++) {
        int j = i;
        while (j >= 0 && a[j] > a[j + step]) {
            j = change_temp(j);
        }
    }
}

void shell()
{
    while (step > 0) {
        thread s1(shell_temp);
        if (step > 2){
            step /= 2;
            thread s2(shell_temp);
            s2.join();
        }
        s1.join();
        step /= 2;
    }
}

int main()
{
    
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
    
    printa();
    unsigned int start_time =  clock();
    shell();
    unsigned int end_time = clock();
    printa();
    cout << (end_time - start_time) / 1000.00000000 << "\n";
    return 0;
}
