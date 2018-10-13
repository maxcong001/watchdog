
#include "logger/logger.hpp"
#include "watchDog/watchDog.hpp"
#include <string>
#include <iostream>
#include <boost/asio/ip/address.hpp>
#include <boost/format.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <thread>
#include <chrono>

boost::asio::io_service ioService;

boost::asio::deadline_timer t(ioService, boost::posix_time::seconds(1));

void print(const boost::system::error_code & /*e*/)
{
    std::cout << "Hello, world!\n";
    t.expires_from_now(boost::posix_time::seconds(1));
    t.async_wait(print);
}

int main()
{
    set_log_level(logger_iface::log_level::debug);

    std::thread asio_thread([&]() {
        __LOG(debug, "ioservice is running");
        ioService.run();
    });
    t.async_wait(print);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    watchDog wd_test(ioService);
    wd_test.start(2, 2, []() {
        __LOG(debug, "heart beat lost");
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    __LOG(debug, "now peg watch dog");
    wd_test.peg();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    ioService.stop();
    asio_thread.join();
    __LOG(debug, "exiting example");
}
