/*
 *  DBusTL - D-Bus Template Library
 *
 *  Copyright (C) 2008, 2009  Fabien Chevalier <chefabien@gmail.com>
 *  
 *
 *  This file is part of the D-Bus Template Library.
 *
 *  The D-Bus Template Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  D-Bus Template Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with D-Bus Template Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <dbustl-1/EventLoopIntegration>

#include <cassert>

namespace dbustl {

EventLoopIntegration::EventLoopIntegration() : _conn(0) 
{
}

EventLoopIntegration::~EventLoopIntegration()
{
}

void EventLoopIntegration::connect(Connection* conn)
{
    assert(conn);
    assert(!_conn);

    if(internalConnect(conn)) {
        _conn = conn;
    }
}

}
