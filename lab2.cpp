#include <iostream>
#include <thread>
#include <mutex>  
#include <ctime>
using namespace std;

class Buf {
private:
    int BUF_SIZE = 16;
    int BUF_MASK = BUF_SIZE - 1;
    int idxIN = 0;
    int idxOUT = 0;
    int *buffer;
    mutex mut;
    
public:
    Buf()
    {
        buffer = new int[BUF_SIZE];
        for (int i = 0; i < BUF_SIZE; i++){
            buffer[i] = 0;
        }
    }
    ~Buf() { delete buffer; }

    void input(int value)
    {
        lock_guard<mutex> lock(mut);
        buffer[idxIN++] = value;
        idxIN = idxIN % BUF_MASK;
    }
    
    int lookup(int value)
    {        
        int i = 0;
        
        for (; i < BUF_SIZE; i++) {
            if (buffer[i] == value) {
                return i;
            }
        }
        
        return -1;
    }
    
    void del ()
    {
        lock_guard<mutex> lock(mut);
        buffer[idxOUT++] = 0;
        idxOUT = idxOUT % BUF_MASK;
    }
    
    void print()
    {
        for (int i = 0; i < BUF_SIZE; i++) {
            cout << buffer[i] << " ";
        }
        cout <<  "\n\n";
    }
};

int n = 100;
int *a = new int[n];
Buf b;

enum funs {
    input,
    print,
    lookup,
    del
};

void do_something(enum funs f)
{
    switch(f) {
        case input: {
            for (int i = 0; i < n; i++) {
                b.input(a[i]);
            }
            cout << "1";
            break;
        }
        case print:{
            b.print();
            break;
        }
        case lookup: {
            b.lookup(a[n/2]);
            cout << "2";
            break;
        }
        case del: {
            b.del();
            cout << "3";
            break;
        }
    }
            
}


int main()
{
    thread s[5];
    
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
    s[0] = thread(do_something, input);
    s[0].join();
    
    unsigned int start_time =  clock();
    
    s[1] = thread(do_something, lookup);
    s[2] = thread(do_something, del);
    s[3] = thread(do_something, lookup);
    s[4] = thread(do_something, del);
    
    for (int i = 1; i < 5; i++) {
        s[i].join();
    }
    
    unsigned int end_time = clock();
    cout << "\n" << (end_time - start_time) / 1000.00000000 << "\n";
    
    return 0;
}