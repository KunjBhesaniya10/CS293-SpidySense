#include <boost/asio.hpp>
#include <iostream>

void handleClient(boost::asio::ip::tcp::socket &socket) {
    boost::asio::write(socket, boost::asio::buffer("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!\n"));
}

void server() {
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::acceptor acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

    std::cout << "Server running on port 8080...\n";
    for (;;) {
        boost::asio::ip::tcp::socket socket(ioService);
        acceptor.accept(socket);
        handleClient(socket);
    }
}

int main() {
    server();
    return 0;
}
