/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright 2016 Mattia Basaglia
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

#include <iostream>
#include <algorithm>

#include "muhttpd.hpp"


class MyServer : public muhttpd::Server
{
public:
    using Server::Server;

    void show_headers(const std::string& title, const muhttpd::Headers& headers)
    {
        std::cout << title << ":\n";
        for ( const auto& head : headers )
            std::cout << '\t' << head.name << ' ' << head.value << '\n';
    }

    muhttpd::Response respond(const muhttpd::Request& request) override
    {
        std::cout << request.from.string << ' ' << request.from.port << ' '
                  << request.version << ' '
                  << request.method << ' ' << request.url << ' '
                  << request.auth.user << ' ' << request.auth.password
                  << '\n';

        show_headers("Headers", request.headers);
        show_headers("Cookies", request.cookies);
        show_headers("Get", request.get);
        show_headers("Post", request.post);

        std::string body = request.body;
        std::replace_if(body.begin(), body.end(), [](char c){return c < ' ';}, ' ');
        std::cout << '\n' << body << '\n';
        return muhttpd::Response("Hello, world!\n");
    }
};

int main()
{
    MyServer server(8888);

    server.start();

    std::cout << "Server started, hit enter to quit\n";
    std::cin.get();

    return 0;
}