#include <iostream>
#include <asio.hpp>
using namespace std;

int count = 0 ;


void tick(asio::steady_timer& timer, asio::io_context& io){
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait([&](const asio::error_code& error) {
        if (!error) {
            std::cout << "Tick " << ++::count << std::endl;
            
                tick(timer, io); // Schedule the next tick
        
        } 
    });
}


int main() { 
    asio::io_context io_context;

    asio::steady_timer timer(io_context);

    tick(timer, io_context); // Start the first tick
    io_context.run(); // Run the io_context to process events

    std::cout << "Timer stopped." << std::endl;
    return 0;


}