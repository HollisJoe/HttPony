/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright  Mattia Basaglia
 *
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
#ifndef HTTPONY_HTTP_POST_URLENCODED_HPP
#define HTTPONY_HTTP_POST_URLENCODED_HPP

#include "post.hpp"
#include "httpony/uri.hpp"

namespace httpony {
namespace post {


class UrlEncoded final : public PostFormat
{
public:
    bool matches(const MimeType& mime) const override
    {
        return mime.matches_type("application", "x-www-form-urlencoded");
    }

    bool parse(Request& request) const override
    {
        if ( matches(request.body.content_type()) )
        {
            request.post = parse_query_string(request.body.read_all());
            return true;
        }
        return false;
    }

    bool format(Request& request) const override
    {
        request.body.start_output("application/x-www-form-urlencoded");
        request.body << build_query_string(request.post);
        return true;
    }
};

} // namespace post
} // namespace httpony
#endif // HTTPONY_HTTP_POST_URLENCODED_HPP
