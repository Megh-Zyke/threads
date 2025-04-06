#include <iostream>
#include <thread>
#include <mutex>

std::mutex ticketMtx;

/*
There are two booking agents (threads) trying to book tickets from a shared ticket counter.
You start with 10 tickets available.
Each agent tries to book 1 ticket at a time, 10 times in total.
*/


/*
RAII Resource Acquisition Is Initialization

*/


int ticketsAvaliable = 10;

void bookTicket(const std::string& name){
    for (int i =0 ; i < 10 ; i++){
        std::lock_guard<std::mutex> lock(ticketMtx);
        if (ticketsAvaliable > 0){
            std::cout << name << " booked a ticket. Tickets left: " << --ticketsAvaliable << "\n";
        } else {
            std::cout << name << " tried to book a ticket but none are available.\n";
            break;
        }
    }

}

int main(){
    std::thread agent1(bookTicket, "Agent 1");
    std::thread agent2(bookTicket, "Agent 2");

    agent1.join();
    agent2.join();
    
    return 0;
}