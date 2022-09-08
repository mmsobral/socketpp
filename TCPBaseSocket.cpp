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
#include "TCPBaseSocket.h"

namespace sockpp {

    TCPBaseSocket::TCPBaseSocket() : BaseSocket() {
    }

    TCPBaseSocket::TCPBaseSocket(const TCPBaseSocket &orig) : BaseSocket(orig) {
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

    std::optional<Connection> TCPServerSocket::accept() {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);

        int con = ::accept(sd, (sockaddr *) &addr, &len);
        if (con < 0) throw TCPBaseSocket::SocketException(errno);
        conns.push_back(std::make_unique<Connection>(con));
        return std::optional<Connection>{*conns.back()};
    }

    int TCPServerSocket::get_num_connections() const {
        return conns.size();
    }

    void TCPServerSocket::close_connection(Connection &sock) {
        for (auto it = conns.begin(); it != conns.end(); it++) {
            if (it->get() == &sock) {
                conns.erase(it);
                break;
            }
        }

    }

    std::optional<Connection> TCPServerSocket::wait() {
        return wait(0);
    }

    std::optional<Connection> TCPServerSocket::wait(long timeout_ms) {
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

        for (auto &sock: conns) {
            int sockd = sock->get_descriptor();
            if (FD_ISSET(sockd, &socks)) {
                if (not sock->isConnected()) {
                    throw TCPServerSocket::DisconnectedException(sock->get_peer());
                }
                if (sock->isNew()) sock->set_used();
                return std::optional<Connection>{*sock};
            }
        }

        return std::nullopt;
    }

    void TCPServerSocket::check_disconnected() {
        for (auto it = conns.begin(); it != conns.end(); it++) {
            auto & conn = *it;
            if (not conn->isConnected()) {
                it = conns.erase(it);
            }
        }
    }

    Connection::Connection(int socket_descriptor) :
            TCPBaseSocket(socket_descriptor), novo(true) {
    }

}