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
#ifndef HTTPONY_SSL_SERVER_HPP
#define HTTPONY_SSL_SERVER_HPP

#include "httpony/ssl/ssl_socket.hpp"
#include "httpony/http/agent/server.hpp"

namespace httpony {
namespace ssl {

/**
 * \brief Base class for SSL-enabled servers
 */
class SslServer : public Server
{
public:
    explicit SslServer(
        io::ListenAddress listen,
        const std::string& cert_file,
        const std::string& key_file,
        const std::string& dh_file = {}
    )
        : Server(std::move(listen)),
          context(boost_ssl::context::sslv23)
    {
        context.set_password_callback([this]
            (std::size_t max_length, boost_ssl::context::password_purpose purpose)
            { return password(); }
        );
        context.use_certificate_chain_file(cert_file);
        context.use_private_key_file(key_file, boost::asio::ssl::context::pem);
        if ( !dh_file.empty() )
            context.use_tmp_dh_file(dh_file);
    }

    /**
     * \brief This should return the password for the encrypted key
     */
    virtual std::string password() const
    {
        return "";
    }

private:
    /**
     * \brief Creates a connection linked to a SSL socket
     */
    std::shared_ptr<io::Connection> create_connection() final
    {
        return std::make_shared<io::Connection>(io::SocketTag<SslSocket>{}, context);
    }

    /**
     * \brief Performs the SSL handshake
     */
    bool accept(io::Connection& connection) final
    {
        SslSocket& socket = socket_cast(connection.socket());

        boost::system::error_code error_code;
        socket.ssl_socket().handshake(boost_ssl::stream_base::server, error_code);
        if ( error_code )
        {
            error(connection, error_code.message());
            return false;
        }

        return true;
    }

    /**
     * We know the socket is a SslSocket because create_connection() is final
     */
    static SslSocket& socket_cast(io::TimeoutSocket& in)
    {
        return static_cast<SslSocket&>(in.socket_wrapper());
    }

    boost_ssl::context context;
};

} // namespace ssl
} // namespace httpony
#endif // HTTPONY_SSL_SERVER_HPP
