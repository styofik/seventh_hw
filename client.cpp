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

    boost::asio::ip::tcp::socket client_socket(io_service);
    client_socket.connect(endpoint);

    std::string messageout, messagein;
    std::cout << "" << std::endl;
    send(client_socket, "client request for connection");
    while (true) {
        messagein = read(client_socket);
        messagein.pop_back();
        if (messagein == "exit") {
            std::cout << "server disconnected" << std::endl;
            break;
        }
        std::cout << "server: " << messagein << std::endl;

        std::cout << "client: ";
        std::getline(std::cin, messageout);
        send(client_socket, messageout);

        if (messageout == "exit")
            break;
    }
    return 0;
}
