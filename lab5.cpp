#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex qu, s;
int mas[100] = {1,2,4,5,56,37,3764,4674,3545};
int f[20] = {0};
int q_size = 16;
int command[16] = {0};
int key[16] = {0};
int cli[16] = {0};
atomic <int> id, id_out;

void findid(){
    int f = 0;
    for (int i = 0; i <= q_size ; i++) {
        if (command[i] == 0) {
            id = i;
            f = 1;
            }
    }
    if (f == 0)
        sleep(2);
}

void findout()
{
    int f = 0;
    for (int i = 0; i <= q_size ; i++) {
        if (command[i] != 0) {
            id_out = i;
            f = 1;
        }
    }
    if (f == 0)
        sleep(2);
}

void queue (int com, int ke, int c)
{
    lock_guard<mutex> lock(qu);
        
    command[id] = com;
    key[id] = ke;
    cli[id] = c;
    if (id + 1 == q_size)
        findid();
    else id++;
}
    
    
    
void server()
{
    lock_guard<mutex> lock(s);
        
    if (command[id_out] == 0)
        findout();
    f[cli[id_out]] = 100;
    
    switch (command[id_out]) {
        case 1: {
            cout << "\nlooking " << key[id_out] << "..\n";
            int f = 0;
            for (int i = 0; mas[i] != 0; i++) {
                if (mas[i] == key[id_out]) {
                    cout << key[id_out] << " is " << i << " \n";
                    f = 1;
                    break;
                }
            }
            
            if (f == 0)
                cout << "not found\n";
            break;
        }
            
        case 2: {
            cout << "\nadding " << key[id_out] << "\n";
            int i = 0;
            for (; mas[i] != 0; i++);
            mas[i] = key[id_out];
            cout << "added\n";
            break;
        }
            
        case 3: {
            cout << "\ndeliting " << key[id_out] << "..\n";
            int f = 0;
            for (int i = 0; mas[i] != 0; i++) {
                if (mas[i] == key[id_out]) {
                    mas[i] = 0;
                    cout << "delited\n";
                    f = 1;
                    break;
                }
            }
            
            if (f == 0)
                cout << "not found\n";
            
            break;
        }
    }
    f[cli[id_out]] = 200;
    if (id_out + 1 == q_size || id_out + 1 == 0)
        findout();
    else id_out++;
}

void client(int i)
{
    queue(rand() % 3 + 1, rand(), i);
    if (f[i] < 100) this_thread::sleep_for(chrono::seconds(3));
    if (f[i] == 100) this_thread::sleep_for(chrono::seconds(2));
    if (f[i] == 200) cout << i;
}



int main()
{
    thread cl[20], se[20];
    
    int n = 10;
    for (int i = 0; i < n; i++) {
        cl[i] = thread(client, i);
        se[i] = thread(server);
    }
        
    for (int i = 0; i < n; i++) {
        if (cl[i].joinable()) {
            cl[i].join();
        }
        if (se[i].joinable()) {
            se[i].join();
        }
    }
    return 0;
}