#include <iostream>
#include <boost/asio.hpp>

std::string read(boost::asio::ip::tcp::socket& socket) {
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, "\n");
    std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
    return data;
}

void send(boost::asio::ip::tcp::socket& socket, const std::string& message) {
    write(socket, boost::asio::buffer(message + "\n"));
}

int main() {
    std::string ip = "127.0.0.1";
    auto port = 65000u;

    boost::asio::io_service io_service;
    
    boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::address::from_string(ip), port);
    
    boost::asio::ip::tcp::acceptor server_acceptor(io_service, endpoint);

    boost::asio::ip::tcp::socket server_socket(io_service);
    
    std::cout << "server is started" << std::endl;

    server_acceptor.accept(server_socket);
    std::string messagein = read(server_socket);
    std::string messageout = messagein + " is received";
    send(server_socket, messageout);
    while (true) {
        messagein = read(server_socket);
        messagein.pop_back();
        if (messagein == "exit") {
            std::cout << "client disconnected!" << std::endl;
            break;
        }
        std::cout << "client: " << messagein << std::endl;

        std::cout << "server: ";
        std::getline(std::cin, messageout);
        send(server_socket, messageout);

        if (messageout == "exit")
            break;
    }
    return 0;
}
