/*
A race condition happens when two or more threads access shared data at the same time, and at least one of them writes to it.
This can cause unpredictable results.

function threads called using pass by reference 
```
    std::thread t1(increment, std::ref(num));
```

passing without reference will create a copy of the variable in the thread function
```
    std::thread t1(increment, num);
```
This will not work as expected because the thread will be working on a copy of the variable, not the original one. 
C++'s std::thread internally copies the arguments and passes them to the function. When it sees num, it passes a copy. When it sees &num, it treats it like a pointer.
*/ 

#include <iostream>
#include <thread>
#include <mutex>

int balance = 100;
std::mutex mtx;

/*
Mutex allows only one thread to access the critical section at a time. use std::lock_guard to lock the mutex when entering the critical section and automatically unlock it when leaving the scope.
*/

void withdraw(const std::string& name, int amount) {
    for (int i = 0; i < 3; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        // Critical section - only one thread can access this at a time
        // lock_guard will automatically unlock the mutex when it goes out of scope
        if (balance >= amount) {
            std::cout << name << " is withdrawing " << amount << "\n";
            balance -= amount;
        } else {
            std::cout << name << " tried to withdraw but insufficient balance.\n";
        }
        std::cout << "Remaining balance: " << balance << "\n";
    }
}

// int main() {
//     std::thread t1(withdraw, "Alice", 30);
//     std::thread t2(withdraw, "Bob", 50);

//     t1.join();
//     t2.join();

//     return 0;
// }


std::mutex logMutex;

void logMessage(const std::string& message) {
    std::cout << "[LOG]: " << message << "\n";
}

void doWork(const std::string& name) {
    for (int i = 0; i < 5; ++i) {
        logMessage(name + " is working on task " + std::to_string(i));
    }
}

int main() {
    std::thread t1(doWork, "Thread 1");
    std::thread t2(doWork, "Thread 2");

    t1.join();
    t2.join();

    return 0;
}
/*
the output when i do not use mutex is :
[LOG]: Thread 1 is working on task 0
[LOG]: Thread 1 is working on task 1
[LOG]: Thread 1 is working on task 2
[LOG]: Thread 2 is working on task 0
[LOG]: Thread 2 is working on task 1
[LOG]: Thread 2 is working on task 2
[LOG]: Thread 2 is working on task 3
[LOG]: Thread 2 is working on task 4
[LOG]: Thread 1 is working on task 3
[LOG]: Thread 1 is working on task 4

Vectors are not thread safe.
*/