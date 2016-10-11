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
    std::string join( std::vector<string>& elements, std::string delimiter )
    {
        std::stringstream ss;
        size_t length = elements.size();
        size_t last = length - 1;

        for( size_t i = 0; i < length; ++i )
        {
            ss << elements[i];

            if( i != last )
                ss << delimiter;
        }

        return ss.str();
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
