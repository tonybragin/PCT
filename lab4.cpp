#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <signal.h>
using namespace std;

thread cl, se;
mutex mut;
int f;

void server(int dos[])
{
    lock_guard<mutex> lock(mut);
    
    fstream keys;
    string val;
    int key;
    
    std::map<int, string> kv;
    keys.open("key", ios::out | ios::in);
    
    while (keys >> key) {
        keys >> val;
        kv[key] = val;
    }

    keys.close();
    
    keys.open("key", ios::trunc | ios::out | ios::in);

    switch (dos[0]) {
        case 0: {
            cout << "looking " << dos[1] << "..\n";
            if (kv.find(dos[1]) != kv.end()) {
                cout << dos[1] << " - " << kv[dos[1]] << "\n";
            }
            else cout << "not found\n";
            break;
        }
            
        case 1: {
            cout << "adding " << dos[1] << " - default..\n";
            kv[dos[1]] = "default";
            cout << "added\n";
            break;
        }
            
        case 2: {
            cout << "deliting " << dos[1] << "..\n";
            if (kv.find(dos[1]) != kv.end()) {
                kv.erase(dos[1]);
                cout << "delited\n";
            }
            else cout << "not found\n";
            break;
        }
            
        case 3: {
            for (std::map<int, string>::iterator it = kv.begin(); it != kv.end(); ++it)
                keys << it->first << " " << it->second << '\n';
            keys.close();
            cout << "exit\n";
            exit(0);
            break;
        }
    }
    for (std::map<int, string>::iterator it = kv.begin(); it != kv.end(); ++it)
        keys << it->first << " " << it->second << '\n';
    f = 1;
    keys.close();
}

void client()
{
    int dos[2];
    
    dos[0] = rand() % 3;
    dos[1] = rand() % 10;
    f = 0;
    
    cout << "\nRequest\n...\n";
    se = thread(server, dos);
    se.join();
    
    if (f == 1) {
        cout << "...\nDone\n\n";
    }
    else cout << "Something went wrong=(\n\n";
}

static void out_handler(int signo) {
    cout << "\nWait\n";
    sleep(4);
    cout << "Exit\n";
    exit(EXIT_SUCCESS);
}

static void int_handler(int signo) {
    cout << "Exit" << endl;
    exit(EXIT_FAILURE);
}

int main()
{
    signal (SIGQUIT, out_handler);
    signal (SIGINT, int_handler);
    signal (SIGTERM, int_handler);
    for (int i = 0; i < 10; i++) {
        cl = thread(client);
        cl.join();
        sleep(2);
    }
}