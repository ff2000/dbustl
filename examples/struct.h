/** 
 *  DBusTL - D-Bus Template Library
 *
 *  Copyright (C) 2008  Fabien Chevalier <chefabien@gmail.com>
 *  
 *
 *  This file is part of the DBus Template Library.
 *
 *  The DBus Template Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  DBus Template Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DBus Template Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
#include <string>
#include <vector>
 
#include <dbustl-1/dbustl>

struct ExampleStruct {
	std::string field1;
	double field2;
	std::vector<int> field3;
};

DBUSTL_REGISTER_STRUCT_3(
    ExampleStruct, 
    field1, 
    field2, 
    field3
)
