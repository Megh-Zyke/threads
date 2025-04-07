#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>

class BadThreadPool {
public:
    BadThreadPool(size_t threads) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        if (!tasks.empty()) {
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                    }

                    if (task) {
                        task();
                    }

                    // Sleep a bit to simulate idle time
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            });
        }
    }

    void enqueue(std::function<void()> f) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(f);
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
};


int main() {
    BadThreadPool pool(2);

    pool.enqueue([]() {
        std::cout << "Task A running\n";
    });

    pool.enqueue([]() {
        std::cout << "Task B running\n";
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Main thread exiting...\n";

    return 0; 
}
