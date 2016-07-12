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

#ifndef HTTPONY_SERVER_HPP
#define HTTPONY_SERVER_HPP

#include <memory>

#include "connection.hpp"

namespace httpony {

/**
 * \brief Base class for a simple HTTP server
 * \note It reads POST data in a single buffer instead of streaming it
 */
class Server
{
public:

    explicit Server(IPAddress listen);
    explicit Server(uint16_t listen_port);

    ~Server();

    /**
     * \brief Listening address
     */
    IPAddress listen_address() const;

    /**
     * \brief Starts the server in a background thread
     * \throws runtime_error if it cannot be started
     */
    void start();

    /**
     * \brief Whether the server has been started
     */
    bool started() const;

    /**
     * \brief Stops the background threads
     */
    void stop();

    /**
     * \brief The timeout for network I/O operations
     */
    melanolib::Optional<melanolib::time::seconds> timeout() const;

    void set_timeout(melanolib::time::seconds timeout);

    void clear_timeout();

    /**
     * \brief Maximum size of a request body to be accepted
     */
    std::size_t max_request_body() const;

    void set_max_request_body(std::size_t size);

    /**
     * \brief Function handling requests
     */
    virtual void respond(Connection& connection, Request&& request) = 0;

    /**
     * \brief Writes a line of log into \p output based on format
     */
    void log_response(
        const std::string& format,
        const Connection& connection,
        const Request& request,
        const Response& response,
        std::ostream& output) const;

private:
    /**
     * \brief Writes a single log item into \p output
     */
    virtual void process_log_format(
        char label,
        const std::string& argument,
        const Connection& connection,
        const Request& request,
        const Response& response,
        std::ostream& output
    ) const;

    struct Data;
    std::unique_ptr<Data> data;
};

} // namespace httpony
#endif // HTTPONY_SERVER_HPP
