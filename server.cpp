#include <iostream>
#include <asio.hpp>

using namespace std;
int main() {
    try{
        asio::io_context io_context;
        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 12345));
        std::cout << "Server is running on port 12345..." << std::endl;

        asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Client connected!" << std::endl;

        while(true){
            char data[1024];
            size_t length = socket.read_some(asio::buffer(data));
            std::cout << "Received: " << std::string(data, length) << std::endl;
            asio::write(socket, asio::buffer(data, length));

        }
    }
    catch(std::exception& e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}