#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
using namespace std;

/*
Instead of creating a new thread for every task, a thread pool:

Creates a fixed number of worker threads ahead of time.
Accepts tasks (jobs) into a task queue.
Worker threads pick up tasks and execute them.
Saves overhead of thread creation/destruction for each task.
*/

//creating a thread pool class

class ThreadPool {
    public:
        ThreadPool(size_t threads);
        ~ThreadPool(); // Destructor to join all threads

        void enqueue(function<void()> task); // Add a task to the queue

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
};

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all(); // Notify all threads to wake up and check the stop condition

    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join(); // Wait for all threads to finish
        }
    }
}

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    
                    // Wait until there's a task or we're stopping
                    this->condition.wait(lock, [this]() {
                        return this->stop || !this->tasks.empty();
                    });

                    // Exit if we're stopping and no tasks left
                    if (this->stop && this->tasks.empty())
                        return;

                    // Grab the task from the queue
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                // Execute the task
                task();
            }
        });
    }
}


void ThreadPool::enqueue(function<void()> task) { 
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.push(std::move(task)); // Add the task to the queue
    }

    condition.notify_one(); // Notify one waiting thread to wake up and process the task
}


int main() {
    ThreadPool pool(3);  // 3 worker threads

    for (int i = 1; i <= 10; ++i) {
        pool.enqueue([i]() {
            std::cout << "Task " << i << " is running on thread " 
                      << std::this_thread::get_id() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        });
    }

    // ThreadPool destructor will automatically wait for all tasks
    return 0;
}
