#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));
        std::cout << "Server is running on port 1234...\n";

        tcp::socket socket(io_context);
        acceptor.accept(socket);  // blocks until a client connects
        std::cout << "Client connected!\n";

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
