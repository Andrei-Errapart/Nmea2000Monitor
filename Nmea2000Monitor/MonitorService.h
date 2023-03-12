#pragma once

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

#include "MonitorSettings.h"


class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
    // typedef boost::asio::ip::tcp tcp;

    http_connection(boost::asio::ip::tcp::socket socket);

    // Initiate the asynchronous operations associated with the connection.
    void start();

private:
    // The socket for the currently connected client.
    boost::asio::ip::tcp::socket socket_;

    // The buffer for performing reads.
    boost::beast::flat_buffer buffer_;

    // The request message.
    boost::beast::http::request<boost::beast::http::dynamic_body> request_;

    // The response message.
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    boost::asio::steady_timer deadline_;

    // Asynchronously receive a complete request message.
    void read_request();

    // Determine what needs to be done with the request message.
    void process_request();

    // Construct a response message based on the program state.
    void create_response();

    // Asynchronously transmit the response message.
    void write_response();

    // Check whether we have spent enough time on this connection.
    void check_deadline();
};

class MonitorService {
private:
    MonitorSettings settings;
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::acceptor  acceptor;
    boost::asio::ip::tcp::socket    socket;
    std::unique_ptr<std::thread>    m_thread;

    void thread_function();
public:
    MonitorService(MonitorSettings pSettings);
    ~MonitorService();
    void stop();
};