#
# Copyright 2016 Mattia Basaglia
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
add_custom_target(examples)

function(example name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} ${LIBRARY_NAME})
    add_dependencies(examples ${name})
endfunction()

set(OPENSSL_FOUND ON)
find_package(OpenSSL QUIET)
if(OPENSSL_FOUND)
    add_subdirectory(ssl)
else()
    message(STATUS "SSL examples disabled (You need OpenSSL for them)")
endif()

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(client_server)
