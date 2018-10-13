
#include "logger/logger.hpp"
#include "watchDog/watchDog.hpp"
#include <string>
#include <iostream>
#include <boost/asio/ip/address.hpp>
#include <boost/format.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <thread>
#include <chrono>

int main()
{
    set_log_level(logger_iface::log_level::debug);

    boost::asio::io_service ioService;
    watchDog wd_test(ioService);
    wd_test.start(2, 2, []() {
        __LOG(debug, "heart beat lost");
    });
    std::thread asio_thread([&]() {
        __LOG(debug, "ioservice is running");
        ioService.run();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    __LOG(debug, "now peg watch dog");
    wd_test.peg();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    ioService.stop();
    asio_thread.join();
    __LOG(debug, "exiting example");
}
