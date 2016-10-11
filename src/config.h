/*
 * Copyright (C) 2016 Roman Shchekin aka mrqtros.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/**
 * Client configuration
 */
struct Config {
    typedef std::shared_ptr<Config> Ptr;

    // The custom HTTP user agent string for this library
    std::string user_agent { "academy-scope 0.1; (foo)" };

    // Cache dir.
    std::string cache_dir { "." }; // Will be set in runtime.
};

#endif // CONFIG_H_
