#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> dataQueue;
bool finished = false;

void producer() {
    for (int i = 1; i <= 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            dataQueue.push(i);
            std::cout << "Produced: " << i << "\n";
        }
        cv.notify_one(); // notify the consumer
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate delay
    }

    // signal that production is done
    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all(); // wake up all consumers
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !dataQueue.empty() || finished; });

        while (!dataQueue.empty()) {
            int value = dataQueue.front();
            dataQueue.pop();
            lock.unlock(); // release lock while processing
            std::cout << "Consumed: " << value << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(300)); // simulate work
            lock.lock(); // re-acquire lock before checking again
        }

        if (finished) break;
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    std::cout << "All data processed.\n";
    return 0;
}
