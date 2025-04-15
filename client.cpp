#include <iostream>
#include <asio.hpp>


using namespace std;    
int main() {
    try{
        asio::io_context io_context;
        asio::ip::tcp::resolver resolver(io_context);

        auto endpoints = resolver.resolve("127.0.0.1", "12345");
        asio::ip::tcp::socket socket(io_context);
        asio::connect(socket, endpoints);
        std::cout << "Connected to server!" << std::endl;   

        while(true){
            cout<< "Enter a message"<<endl;
            string message;
            getline(cin, message);

            asio::write(socket, asio::buffer(message));
            char data[1024];
            size_t length = socket.read_some(asio::buffer(data));
            std::cout << "Received: " << std::string(data, length) << std::endl;
        }
    }
    catch(asio::system_error& e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}