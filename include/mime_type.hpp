/**
 * \file
 * \author Mattia Basaglia
 * \copyright Copyright 2016 Mattia Basaglia
 * \section License
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef HTTPONY_MIME_TYPE_HPP
#define HTTPONY_MIME_TYPE_HPP

#include <ostream>

#include <melanolib/string/quickstream.hpp>
#include <melanolib/string/simple_stringutils.hpp>

#include "headers.hpp"

namespace httpony {

class MimeType
{
public:
    using Parameter = std::pair<std::string, std::string>;

    MimeType(const std::string& string)
    {
        melanolib::string::QuickStream stream(string);
        set_type(stream.get_line('/'));
        set_subtype(stream.get_until(
            [](char c){ return std::isspace(c) || c ==';'; }
        ));

        if ( !stream.eof() )
        {
            stream.get_until(
                [](char c){ return !std::isspace(c) && c != ';'; },
                false
            );

            Parameter parameter;
            parameter.first = stream.get_line('=');

            if ( stream.peek() == '"' )
            {
                stream.ignore(1);
                parameter.second = stream.get_line('"');
            }
            else
            {
                parameter.second = stream.get_line();
            }

            set_parameter(parameter);

        }
    }

    MimeType(){}

    MimeType(const std::string& type, const std::string& subtype, const Parameter& param = {})
    {
        set_type(type);
        set_subtype(subtype);
        set_parameter(param);
    }

    bool valid() const
    {
        return !_type.empty() && !_subtype.empty();
    }

    bool operator==(const MimeType& oth) const
    {
        return _type == oth._type && _subtype == oth._subtype &&
            _parameter.first == oth._parameter.first && _parameter.second == oth._parameter.second;
    }

    bool operator!=(const MimeType& oth) const
    {
        return !(*this == oth);
    }

    void set_type(const std::string& type)
    {
        _type = melanolib::string::strtolower(type);
    }

    void set_subtype(const std::string& subtype)
    {
        _subtype = melanolib::string::strtolower(subtype);
    }

    void set_parameter(const Parameter& param)
    {
        if ( param.first.empty() || param.second.empty() )
        {
            _parameter = Parameter();
            return;
        }

        _parameter.first = melanolib::string::strtolower(param.first);

        if ( _parameter.first == "charset" )
            _parameter.second = melanolib::string::strtolower(param.second);
        else
            _parameter.second = param.second;
    }

    std::string type() const
    {
        return _type;
    }

    std::string subtype() const
    {
        return _subtype;
    }

    Parameter parameter() const
    {
        return _parameter;
    }

    friend std::ostream& operator<<(std::ostream& os, const MimeType& mime)
    {
        os << mime._type << '/' << mime._subtype;
        if ( !mime._parameter.first.empty() )
            os << ';' << mime._parameter.first << '=' << mime._parameter.second;
        return os;
    }

private:
    std::string _type;
    std::string _subtype;
    Parameter _parameter;
};

} // namespace httpony
#endif // HTTPONY_MIME_TYPE_HPP