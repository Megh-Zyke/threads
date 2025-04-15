#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>

using asio::ip::tcp;

std::string current_time() {
    std::time_t now = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", std::localtime(&now));
    return buf;
}

void log(const std::string& msg) {
    std::string entry = current_time() + " " + msg;
    std::cout << entry << std::endl;

    // Optional: log to a file
    static std::ofstream log_file("server.log", std::ios::app);
    log_file << entry << std::endl;
}

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        log("Client connected from " + socket_.remote_endpoint().address().to_string());
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, max_length),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string msg(data_, length);
                    log("Received: " + msg);
                    do_write(length);
                } else {
                    log("Client disconnected");
                }
            });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(data_, length),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    log("Echoed message back to client");
                    do_read();
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server {
public:
    Server(asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        log("Server started on port " + std::to_string(port));
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        asio::io_context io_context;
        Server server(io_context, 12345);
        io_context.run();
    } catch (std::exception& e) {
        log(std::string("Server error: ") + e.what());
    }
    return 0;
}
