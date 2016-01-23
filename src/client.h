#ifndef CLIENT_H_
#define CLIENT_H_

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QJsonDocument>

#include "config.h"

/**
 * Provide a nice way to access the HTTP API.
 *
 * We don't want our scope's code to be mixed together with HTTP and JSON handling.
 */
class Client {
public:

    /**
     * Information about a City
     */
    struct City {
        unsigned int id;
        std::string name;
        std::string country;
    };

    /**
     * Temperature information for a day.
     */
    struct Temp {
        double max;
        double min;
        double cur;
    };

    /**
     * Weather information for a day.
     */
    struct Weather {
        unsigned int id;
        std::string main;
        std::string description;
        std::string icon;
        Temp temp;
    };

    /**
     * A list of weather information
     */
    typedef std::deque<Weather> WeatherList;

    /**
     * Weather information about the current day
     */
    struct Current {
        City city;
        Weather weather;
    };

    /**
     * Forecast information about a city
     */
    struct Forecast {
        City city;
        WeatherList weather;
    };

    Client(Config::Ptr config);

    virtual ~Client() = default;

    /**
     * Get the current weather for the specified location
     */
    virtual Current weather(const std::string &query);

    /**
     * Get the weather forecast for the specified location and duration
     */
    virtual Forecast forecast_daily(const std::string &query, unsigned int days = 7);

    /**
     * Cancel any pending queries (this method can be called from a different thread)
     */
    virtual void cancel();

    virtual Config::Ptr config();

protected:
    void get(const core::net::Uri::Path &path,
             const core::net::Uri::QueryParameters &parameters,
             QJsonDocument &root);
    /**
     * Progress callback that allows the query to cancel pending HTTP requests.
     */
    core::net::http::Request::Progress::Next progress_report(
            const core::net::http::Request::Progress& progress);

    /**
     * Hang onto the configuration information
     */
    Config::Ptr config_;

    /**
     * Thread-safe cancelled flag
     */
    std::atomic<bool> cancelled_;
};

#endif // CLIENT_H_

