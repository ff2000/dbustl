/*
 *  DBUSTL - DBus Template Library
 *
 *  Copyright (C) 2008  Fabien Chevalier <fabchevalier@free.fr>
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

// This file Handles serialization & deserialization for C++ basic types
//  - Integral types
//  - Floating point types
//  - Bool
//  - String types (std::string and const char * and char*)

#include <dbustl-1/types/Serialization>
#include <dbustl-1/types/Tools>

#include <stdint.h> //Note : this should be changed as <cstdint> once compilers supports it

namespace dbustl {
namespace types {
    
template<typename T> 
dbus_bool_t __deserializeSignedIntegral(DBusMessageIter* it, T* arg)
{
    switch(dbus_message_iter_get_arg_type(it)) {
    case DBUS_TYPE_BYTE: {
        uint8_t d;          
        dbus_message_iter_get_basic(it, &d);
        *arg = d;
        return TRUE;
    }
    case DBUS_TYPE_INT16: {
        int16_t d;          
        dbus_message_iter_get_basic(it, &d);
        *arg = d;
        return TRUE;
    }
    case DBUS_TYPE_UINT16: {
        if(sizeof(T) >= 4) {
            int16_t d;          
            dbus_message_iter_get_basic(it, &d);
            *arg = d;
            return TRUE;
        }
        return FALSE;
    }
    case DBUS_TYPE_INT32: {
        if(sizeof(T) >= 4) {
            int32_t d;          
            dbus_message_iter_get_basic(it, &d);
            *arg = d;
            return TRUE;
        }
        return FALSE;
    }
    case DBUS_TYPE_UINT32: {
        if(sizeof(T) >= 8) {
            uint32_t d;          
            dbus_message_iter_get_basic(it, &d);
            *arg = d;
            return TRUE;
        }
        return FALSE;
    }
    case DBUS_TYPE_INT64: {
        if(sizeof(T) >= 8) {
            int64_t d;          
            dbus_message_iter_get_basic(it, &d);
            *arg = d;
            return TRUE;
        }
        return FALSE;
    }
    default:
        return FALSE;
    };
}

template<typename T> 
dbus_bool_t __deserializeUnsignedIntegral(DBusMessageIter* it, T* arg)
{
    switch(dbus_message_iter_get_arg_type(it)) {
    case DBUS_TYPE_BYTE: {
        uint8_t d;          
        dbus_message_iter_get_basic(it, &d);
        *arg = d;
        return TRUE;
    }
    case DBUS_TYPE_UINT16: {
        if(sizeof(T) >= 2) {
            int16_t d;          
            dbus_message_iter_get_basic(it, &d);
            *arg = d;
            return TRUE;
        }
        return FALSE;
    }
    case DBUS_TYPE_UINT32: {
        if(sizeof(T) >= 4) {
            uint32_t d;          
            dbus_message_iter_get_basic(it, &d);
            *arg = d;
            return TRUE;
        }
        return FALSE;
    }
    case DBUS_TYPE_UINT64: {
        if(sizeof(T) >= 8) {
            uint64_t d;          
            dbus_message_iter_get_basic(it, &d);
            *arg = d;
            return TRUE;
        }
        return FALSE;
    }
    default:
        return FALSE;
    };
}

/* common serialize implementation */
template<typename T>
dbus_bool_t serialize(DBusMessageIter* it, const T& arg)
{
    return dbus_message_iter_append_basic(it, dbusPreferedType<T>(), &arg);
}

/* bool */
template<>
dbus_bool_t deserialize<bool>(DBusMessageIter* it, bool *arg)
{
    if(dbus_message_iter_get_arg_type(it) != DBUS_TYPE_BOOLEAN) {
        return FALSE;
    }
    dbus_message_iter_get_basic(it, arg);
    return TRUE;
}
template dbus_bool_t serialize(DBusMessageIter* it, const bool& arg);

/* char */
template dbus_bool_t serialize(DBusMessageIter* it, const char& arg);

/* signed char */
template dbus_bool_t serialize(DBusMessageIter* it, const signed char& arg);

/* unsigned char */
template dbus_bool_t serialize(DBusMessageIter* it, const unsigned char& arg);
template<>
dbus_bool_t deserialize<unsigned char>(DBusMessageIter* it, unsigned char *arg)
{
    if(dbus_message_iter_get_arg_type(it) != DBUS_TYPE_BYTE) {
        return FALSE;
    }
    dbus_message_iter_get_basic(it, arg);
    return TRUE;
}

/* short */
template dbus_bool_t serialize(DBusMessageIter* it, const short& arg);
template<>
dbus_bool_t deserialize<short>(DBusMessageIter* it, short* arg)
{
    return __deserializeSignedIntegral<short>(it, arg);
}

/* unsigned short */
template dbus_bool_t serialize(DBusMessageIter* it, const unsigned short& arg);
template<>
dbus_bool_t deserialize<unsigned short>(DBusMessageIter* it, unsigned short* arg)
{
    return __deserializeUnsignedIntegral<unsigned short>(it, arg);
}

/* int */
template dbus_bool_t serialize(DBusMessageIter* it, const int& arg);
template<>
dbus_bool_t deserialize<int>(DBusMessageIter* it, int* arg)
{
    return __deserializeSignedIntegral<int>(it, arg);
}

/* unsigned int */
template dbus_bool_t serialize(DBusMessageIter* it, const unsigned int& arg);
template<>
dbus_bool_t deserialize<unsigned int>(DBusMessageIter* it, unsigned int* arg)
{
    return __deserializeUnsignedIntegral<unsigned int>(it, arg);
}

/* long */
template dbus_bool_t serialize(DBusMessageIter* it, const long& arg);
template<>
dbus_bool_t deserialize<long>(DBusMessageIter* it, long* arg)
{
    return __deserializeSignedIntegral<long>(it, arg);
}

/* unsigned long */
template dbus_bool_t serialize(DBusMessageIter* it, const unsigned long& arg);
template<> 
dbus_bool_t deserialize<unsigned long>(DBusMessageIter* it, unsigned long* arg)
{
    return __deserializeUnsignedIntegral<unsigned long>(it, arg);
}

/* long long */
template dbus_bool_t serialize(DBusMessageIter* it, const long long& arg);
template<> 
dbus_bool_t deserialize<long long>(DBusMessageIter* it, long long* arg)
{
    return __deserializeSignedIntegral<long long>(it, arg);
}

/* unsigned long long */
template dbus_bool_t serialize(DBusMessageIter* it, const unsigned long long& arg);
template<> 
dbus_bool_t deserialize<unsigned long long>(DBusMessageIter* it, unsigned long long* arg)
{
    return __deserializeUnsignedIntegral<unsigned long long>(it, arg);
}

/* float - on way only */
template<> 
dbus_bool_t serialize<float>(DBusMessageIter* it, const float& arg)
{
    double val = arg;
    return dbus_message_iter_append_basic(it, DBUS_TYPE_DOUBLE, &val);
}

/* double */
template dbus_bool_t serialize(DBusMessageIter* it, const double& arg);
template<> 
dbus_bool_t deserialize<double>(DBusMessageIter* it, double *arg)
{
    if(dbus_message_iter_get_arg_type(it) != DBUS_TYPE_DOUBLE) {
        return FALSE;
    }
    dbus_message_iter_get_basic(it, arg);
    return TRUE;
}

/* const char* - one way only */
template dbus_bool_t serialize(DBusMessageIter* it, char const* const & arg);

/* char* - one way only */
template dbus_bool_t serialize(DBusMessageIter* it, char* const & arg);

/* std::string */
template<> 
dbus_bool_t serialize<std::string>(DBusMessageIter* it, const std::string& arg)
{
    return serialize(it, arg.c_str());
}

template<> 
dbus_bool_t deserialize<std::string>(DBusMessageIter* it, std::string* arg)
{
    const char *str;
    if(dbus_message_iter_get_arg_type(it) != DBUS_TYPE_STRING) {
        return FALSE;
    }
    dbus_message_iter_get_basic(it, &str);
    *arg = str;
    return TRUE;
}

}
}
