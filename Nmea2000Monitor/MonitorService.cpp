#include "MonitorService.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace asio = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


http_connection::http_connection(boost::asio::ip::tcp::socket socket)
:   socket_(std::move(socket)),
    buffer_{ 8192 },
    deadline_{ socket_.get_executor(), std::chrono::seconds(60) }
{
}

void http_connection::start()
{
    read_request();
    check_deadline();
}

void http_connection::read_request()
{
    auto self = shared_from_this();
    boost::beast::http::async_read(socket_, buffer_, request_, [self](beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (!ec) {
            self->process_request();
        }
    });
}

void http_connection::process_request()
{
    response_.version(request_.version());
    response_.keep_alive(false);

    switch (request_.method())
    {
    case http::verb::get:
        response_.result(http::status::ok);
        response_.set(http::field::server, "Beast");
        create_response();
        break;

    default:
        // We return responses indicating an error if
        // we do not recognize the request method.
        response_.result(http::status::bad_request);
        response_.set(http::field::content_type, "text/plain");
        beast::ostream(response_.body())
            << "Invalid request-method '"
            << std::string(request_.method_string())
            << "'";
        break;
    }
    write_response();
}

void http_connection::create_response()
{
    if (request_.target() == "/packet") {
        response_.set(http::field::content_type, "text/plain");
        beast::ostream(response_.body())
            << "TODO: implement packet handling\n";
    }
    else {
        response_.result(http::status::not_found);
        response_.set(http::field::content_type, "text/plain");
        beast::ostream(response_.body()) << "File not found\r\n";
    }
}

void http_connection::write_response()
{
    auto self = shared_from_this();
    response_.content_length(response_.body().size());
    http::async_write(socket_, response_, [self](beast::error_code ec, std::size_t) {
        self->socket_.shutdown(tcp::socket::shutdown_send, ec);
        self->deadline_.cancel();
    });
}

void http_connection::check_deadline()
{
    auto self = shared_from_this();
    deadline_.async_wait([self](beast::error_code ec) {
        if (!ec) {
            // Close socket to cancel any outstanding operation.
            self->socket_.close(ec);
        }
    });
}

static void http_server(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket& socket)
{
    acceptor.async_accept(socket, [&](beast::error_code ec) {
        if (!ec) {
            std::make_shared<http_connection>(std::move(socket))->start();
        }
        http_server(acceptor, socket);
    });
}

void MonitorService::thread_function()
{
    try {
        http_server(acceptor, socket);
        ioc.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}

MonitorService::MonitorService(MonitorSettings pSettings)
:   settings(pSettings),
    ioc{ 1 },
    acceptor{ ioc, {boost::asio::ip::make_address_v4("0.0.0.0"), static_cast<unsigned short>(settings.Port) }},
    socket{ ioc }
{
    m_thread = std::make_unique<std::thread>([this]() { this->thread_function(); });
}

MonitorService::~MonitorService()
{
    // stop
    stop();
}

void MonitorService::stop()
{
    if (m_thread) {
        ioc.stop();
        m_thread->join();
        m_thread.reset();
    }
}
