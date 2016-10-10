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

#ifndef THESUFFERING_H_
#define THESUFFERING_H_

#include <string>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>

namespace utils
{
    std::string join(const std::vector<std::string>& vec, const char* delim)
    {
        std::stringstream res;
        copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(res, delim));
        return res.str();
    }

//    void split(const std::string &s, char delim, std::vector<std::string> &elems) {
//        std::stringstream ss;
//        ss.str(s);
//        std::string item;
//        while (std::getline(ss, item, delim)) {
//            elems.push_back(item);
//        }
//    }

//    std::vector<std::string> split(const std::string &s, char delim) {
//        std::vector<std::string> elems;
//        split(s, delim, elems);
//        return elems;
//    }
}

#endif // THESUFFERING_H_
