#pragma once
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include "logger/logger.hpp"
class watchDog
{
  public:
    watchDog(boost::asio::io_service &io_service) : _io_service(io_service), _peg_timer(io_service), _lost_times(5), _interval(10), _max_lost_times(5)
    {
    }
    ~watchDog()
    {
        __LOG(debug, "dtor of watch dog");
    }
    bool start(int interval, int times, std::function<void()> cb_function)
    {
        __LOG(debug, "start watch dog, interval is :" << interval << ", times is :" << times);
        _max_lost_times = times;
        _on_lost = cb_function;
        _interval = interval;
        _peg_timer.expires_from_now(boost::posix_time::seconds(_interval));
        _peg_timer.async_wait(boost::bind(&watchDog::handle_timer, this, _1));
        return true;
    }

    void handle_timer(const boost::system::error_code &ec)
    {
        if (!ec)
        {
            _lost_times++;
            if (_lost_times >= _max_lost_times)
            {
                _on_lost();
                _lost_times = 0;
            }
            __LOG(debug, "peg timer expire, now lost time is :" << _lost_times);
            _peg_timer.expires_from_now(boost::posix_time::seconds(_interval));
            _peg_timer.async_wait(boost::bind(&watchDog::handle_timer, this, _1));
        }
        else
        {
            __LOG(error, "timer error");
        }
    }
    bool stop()
    {
        __LOG(debug, "stop watch dog");
        _peg_timer.cancel();
        _lost_times = 0;
        return true;
    }
    bool peg()
    {
        __LOG(debug, "peg watch dog");
        _lost_times = 0;

        _peg_timer.expires_from_now(boost::posix_time::seconds(_interval));
        _peg_timer.async_wait(boost::bind(&watchDog::handle_timer, this, _1));

        return true;
    }

    boost::asio::io_service &_io_service;
    boost::asio::deadline_timer _peg_timer;
    int _lost_times;
    int _interval;
    std::function<void()> _on_lost;
    int _max_lost_times;
};
