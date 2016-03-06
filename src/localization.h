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

#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include <libintl.h>
#include <string>

inline char * _(const char *__msgid) {
    return dgettext(GETTEXT_PACKAGE, __msgid);
}

inline std::string _(const char *__msgid1, const char *__msgid2,
                     unsigned long int __n) {
    char buffer [256];
    if (snprintf ( buffer, 256, dngettext(GETTEXT_PACKAGE, __msgid1, __msgid2, __n), __n ) >= 0) {
        return buffer;
    } else {
        return std::string();
    }
}

#endif // LOCALIZATION_H_


