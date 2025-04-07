#include <iostream>
#include <thread>
using namespace std;

void run(int count) {
    while(count --> 0)
        cout << "CppNuts" << endl;
    std::this_thread::sleep_for(std::chrono::seconds(3)); 
}

int main() {
    std:: thread t1(run, 5); // Create a thread that runs the run function with count 5cout <<
    cout <<"main() is running" << endl;

    t1.detach();
    // use joinable to check if the thread is joinable
    //if (t1.joinable())
    //    t1.join(); // Wait for the thread to finish
    // The moment the code hits the join() function, the main thread will block until t1 is finished.
    // If you want to detach the thread instead of joining it, you can use t1.detach() instead of t1.join()
    cout << "Thread has finished" << endl;
    return 0;

}

//Join can not be used two times on the same thread. It will throw an error if you try to do so.