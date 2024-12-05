/* 
 * File:   TCPBaseSocket.cpp
 * Author: msobral
 * 
 * Created on 6 de Julho de 2016, 16:14
 */

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <exception>
#include <algorithm>
#include "TCPBaseSocket.h"

namespace sockpp {

    TCPBaseSocket::TCPBaseSocket() : BaseSocket() {
    }

    TCPBaseSocket::TCPBaseSocket(const TCPBaseSocket &orig) : BaseSocket(orig) {
    }

    TCPBaseSocket::TCPBaseSocket(TCPBaseSocket &&orig) : BaseSocket(orig) {
    }

    TCPBaseSocket::TCPBaseSocket(const AddrInfo &addr) {
        init(SOCK_STREAM, 6, addr);
    }

    TCPBaseSocket::TCPBaseSocket(int socket_descriptor) : BaseSocket(socket_descriptor) {
    }

    TCPBaseSocket::~TCPBaseSocket() {
    }

    bool TCPBaseSocket::isConnected() const {
        char x;
        struct timeval tv = {0, 0};
        fd_set socks;
        FD_ZERO(&socks);
        FD_SET(sd, &socks);

        int n = select(sd + 1, &socks, NULL, NULL, &tv);
        if (n) {
            ssize_t r = ::recv(sd, &x, 1, MSG_DONTWAIT | MSG_PEEK);
            return (r > 0);
        }
        return true;
    }

    TCPClientSocket::TCPClientSocket() : TCPBaseSocket() {
    }

    TCPClientSocket::TCPClientSocket(const AddrInfo &addr) : TCPBaseSocket(addr) {
    }

    TCPServerSocket::TCPServerSocket(const AddrInfo &addr) : TCPBaseSocket(addr) {
        if (::listen(sd, 5) < 0) throw TCPBaseSocket::SocketException(errno);
    }

    TCPServerSocket::TCPServerSocket(uint16_t port) : TCPBaseSocket(AddrInfo{"0.0.0.0", port}) {
        if (::listen(sd, 5) < 0) throw TCPBaseSocket::SocketException(errno);
    }

    TCPServerSocket::~TCPServerSocket() {
    }

    std::optional<std::shared_ptr<Connection>> TCPServerSocket::accept() {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);

        int con = ::accept(sd, (sockaddr *) &addr, &len);
//        if (con < 0) throw TCPBaseSocket::SocketException(errno);
        if (con < 0) return std::nullopt;
        conns.push_back(std::make_shared<Connection>(con));
        return std::optional<std::shared_ptr<Connection>>{conns.back()};
    }

    int TCPServerSocket::get_num_connections() const {
        return conns.size();
    }

    void TCPServerSocket::close_connection(std::shared_ptr<Connection> sock) {
        auto it = std::find_if(conns.begin(), conns.end(), [&sock](auto & conn) { return conn == sock;});
        if (it != conns.end()) {
            conns.erase(it);
        }
    }

    std::optional<std::shared_ptr<Connection>> TCPServerSocket::wait() {
        return wait(0);
    }

    std::optional<std::shared_ptr<Connection>> TCPServerSocket::wait(long timeout_ms) {
        if (not sd) throw TCPBaseSocket::SocketException(EINVAL);
        timeval tv, *tv_ptr = NULL;

        if (timeout_ms) {
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = 1000 * (timeout_ms % 1000);
            tv_ptr = &tv;
        }

        check_disconnected();

        fd_set socks;
        FD_ZERO(&socks);
        FD_SET(sd, &socks);
        int maior = sd;

        for (auto & conn: conns) {
            int sockd = conn->get_descriptor();
            FD_SET(sockd, &socks);
            if (sockd > maior) maior = sockd;
        }

        int n = select(maior + 1, &socks, NULL, NULL, tv_ptr);

        if (not n) return std::nullopt;
//        if (not n) throw TCPServerSocket::TimeoutException();

        if (FD_ISSET(sd, &socks)) {
            return accept();
        }

        auto it = std::find_if(conns.begin(), conns.end(), [&socks](auto & sock) {
            if (FD_ISSET(sock->get_descriptor(), &socks)) {
                if (sock->isConnected()) {
                    if (sock->isNew()) sock->set_used();
                    return true;
                }
            }
            return false;
        });
        if (it != conns.end()) {
            return std::optional<std::shared_ptr<Connection>>{*it};
        }

        return std::nullopt;
    }

    void TCPServerSocket::check_disconnected() {
        auto last = conns.end();
        auto end = std::remove_if(conns.begin(), last, [](auto & p_conn) {return ! p_conn->isConnected();});
        if (end != last) conns.erase(end);
    }

    Connection::Connection(int socket_descriptor) :
            TCPBaseSocket(socket_descriptor), novo(true) {
    }

}