#pragma once
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
class watchDog
{
  public:
    watchDog(boost::asio::io_service &io_service) : _io_service(io_service), _peg_timer(io_service), _lost_times(5), _interval(10), _max_lost_times(5)
    {
    }
    ~watchDog()
    {
    }
    bool start(int interval, int times, std::function<void()> cb_function)
    {
        _max_lost_times = times;
        _on_lost = cb_function;
        _peg_timer.expires_from_now(boost::posix_time::seconds(interval));
        _peg_timer.async_wait([&](const boost::system::error_code &ec) {
            if (!ec)
            {
                _lost_times++;
                if (_lost_times >= _max_lost_times)
                {
                    _on_lost();
                }
            }
            else
            {
                __LOG(error, "timer error");
            }
        });
    }
    bool stop()
    {
        _peg_timer.cancel();
        _lost_times = 0;
    }
    bool peg()
    {
        _lost_times = 0;
        _peg_timer.expires_from_now(boost::posix_time::seconds(_interval));
    }

    boost::asio::io_service &_io_service;
    deadline_timer _peg_timer;
    int _max_lost_times;
    int _lost_times;
    int _interval;
    std::function<void()> _on_lost;
};