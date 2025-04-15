#include <iostream>
#include <thread>
#include <asio.hpp>

using asio::ip::tcp;

void handle_client(tcp::socket socket) {
    try {
        std::cout << "Client connected on thread: " << std::this_thread::get_id() << "\n";

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, world!";

        asio::write(socket, asio::buffer(response));
        socket.shutdown(tcp::socket::shutdown_send);
    } catch (std::exception& e) {
        std::cerr << "Error handling client: " << e.what() << "\n";
    }
}

int main() {
    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));
        std::cout << "Server is running on port 1234...\n";

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::thread(handle_client, std::move(socket)).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }

    return 0;
}
