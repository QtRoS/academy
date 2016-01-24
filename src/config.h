#ifndef CONFIG_H_
#define CONFIG_H_

/**
 * Client configuration
 */
struct Config {
    typedef std::shared_ptr<Config> Ptr;

    // The root of all API request URLs
    std::string apiroot { "http://api.openweathermap.org" };

    // The custom HTTP user agent string for this library
    std::string user_agent { "course-explorer-scope 0.1; (foo)" };
};

#endif // CONFIG_H_
