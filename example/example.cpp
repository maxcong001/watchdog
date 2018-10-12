
#include "logger/logger.hpp"
#include "watchDog/watchDog.hpp"
#include <string>
#include <iostream>
#include <boost/asio/ip/address.hpp>
#include <boost/format.hpp>
#include <boost/asio/deadline_timer.hpp>
int main()
{
    set_log_level(logger_iface::log_level::debug);
    boost::asio::io_service ioService;

    watchDog wd_test()
}
