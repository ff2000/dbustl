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

inline dbus_bool_t __deserializeBasic(DBusMessageIter* it, void* arg, int dbus_type)
{
    if(dbus_message_iter_get_arg_type(it) != dbus_type) {
        return FALSE;
    }
    dbus_message_iter_get_basic(it, arg);
    return TRUE;
}

/* common implementations */
template<typename T>
dbus_bool_t Serializer<T>::run(DBusMessageIter* it, const T& arg)
{
    return dbus_message_iter_append_basic(it, basicType<T>::value, &arg);
}

/* bool */
template<>
struct Deserializer<bool> {
    static dbus_bool_t run(DBusMessageIter* it, bool* arg);
};

dbus_bool_t Deserializer<bool>::run(DBusMessageIter* it, bool* arg)
{
    return __deserializeBasic(it, arg, DBUS_TYPE_BOOLEAN);
}

template dbus_bool_t Serializer<bool>::run(DBusMessageIter* it, const bool& arg);

/* char */
template dbus_bool_t Serializer<char>::run(DBusMessageIter* it, const char& arg);

/* signed char */
template dbus_bool_t Serializer<signed char>::run(DBusMessageIter* it, const signed char& arg);

/* unsigned char */
template dbus_bool_t Serializer<unsigned char>::run(DBusMessageIter* it, const unsigned char& arg);
template<>
struct Deserializer<unsigned char> {
    static dbus_bool_t run(DBusMessageIter* it, unsigned char* arg);
};
dbus_bool_t Deserializer<unsigned char>::run(DBusMessageIter* it, unsigned char* arg)
{
    if(dbus_message_iter_get_arg_type(it) != DBUS_TYPE_BOOLEAN) {
        return FALSE;
    }
    dbus_message_iter_get_basic(it, arg);
    return TRUE;
}

/* short */
template dbus_bool_t Serializer<short>::run(DBusMessageIter* it, const short& arg);
template<>
struct Deserializer<short> {
    static dbus_bool_t run(DBusMessageIter* it, short* arg);
};
dbus_bool_t Deserializer<short>::run(DBusMessageIter* it, short* arg)
{
    return __deserializeSignedIntegral<short>(it, arg);
}

/* unsigned short */
template dbus_bool_t Serializer<unsigned short>::run(DBusMessageIter* it, const unsigned short& arg);
template<>
struct Deserializer<unsigned short> {
    static dbus_bool_t run(DBusMessageIter* it, unsigned short* arg);
};
dbus_bool_t Deserializer<unsigned short>::run(DBusMessageIter* it, unsigned short* arg)
{
    return __deserializeUnsignedIntegral<unsigned short>(it, arg);
}

/* int */
template dbus_bool_t Serializer<int>::run(DBusMessageIter* it, const int& arg);
template<>
struct Deserializer<int> {
    static dbus_bool_t run(DBusMessageIter* it, int* arg);
};
dbus_bool_t Deserializer<int>::run(DBusMessageIter* it, int* arg)
{
    return __deserializeSignedIntegral<int>(it, arg);
}

/* unsigned int */
template dbus_bool_t Serializer<unsigned int>::run(DBusMessageIter* it, const unsigned int& arg);
template<>
struct Deserializer<unsigned int> {
    static dbus_bool_t run(DBusMessageIter* it, unsigned int* arg);
};
dbus_bool_t Deserializer<unsigned int>::run(DBusMessageIter* it, unsigned int* arg)
{
    return __deserializeUnsignedIntegral<unsigned int>(it, arg);
}

/* long */
template dbus_bool_t Serializer<long>::run(DBusMessageIter* it, const long& arg);
template<>
struct Deserializer<long> {
    static dbus_bool_t run(DBusMessageIter* it, long* arg);
};
dbus_bool_t Deserializer<long>::run(DBusMessageIter* it, long* arg)
{
    return __deserializeSignedIntegral<long>(it, arg);
}

/* unsigned long */
template dbus_bool_t Serializer<unsigned long>::run(DBusMessageIter* it, const unsigned long& arg);
template<>
struct Deserializer<unsigned long> {
    static dbus_bool_t run(DBusMessageIter* it, unsigned long* arg);
};
dbus_bool_t Deserializer<unsigned long>::run(DBusMessageIter* it, unsigned long* arg)
{
    return __deserializeUnsignedIntegral<unsigned long>(it, arg);
}

/* long long */
template dbus_bool_t Serializer<long long>::run(DBusMessageIter* it, const long long& arg);
template<>
struct Deserializer<long long> {
    static dbus_bool_t run(DBusMessageIter* it, long long* arg);
};
dbus_bool_t Deserializer<long long>::run(DBusMessageIter* it, long long* arg)
{
    return __deserializeSignedIntegral<long long>(it, arg);
}

/* unsigned long long */
template dbus_bool_t Serializer<unsigned long long>::run(DBusMessageIter* it, const unsigned long long& arg);
template<>
struct Deserializer<unsigned long long> {
    static dbus_bool_t run(DBusMessageIter* it, unsigned long long* arg);
};
dbus_bool_t Deserializer<unsigned long long>::run(DBusMessageIter* it, unsigned long long* arg)
{
    return __deserializeUnsignedIntegral<unsigned long long>(it, arg);
}

/* float - one way only */
template<>
struct Serializer<float> {
    static dbus_bool_t run(DBusMessageIter* it, const float& arg);
};

dbus_bool_t Serializer<float>::run(DBusMessageIter* it, const float& arg)
{
    double val = arg;
    return dbus_message_iter_append_basic(it, DBUS_TYPE_DOUBLE, &val);
}

/* double */
template dbus_bool_t Serializer<double>::run(DBusMessageIter* it, const double& arg);
template<>
struct Deserializer<double> {
    static dbus_bool_t run(DBusMessageIter* it, double* arg);
};
dbus_bool_t Deserializer<double>::run(DBusMessageIter* it, double* arg)
{
    return __deserializeBasic(it, arg, DBUS_TYPE_DOUBLE);
}

/* const char* - one way only */
template dbus_bool_t Serializer<char const*>::run(DBusMessageIter* it, char const* const & arg);

/* char* - one way only */
template dbus_bool_t Serializer<char *>::run(DBusMessageIter* it, char* const & arg);

/* std::string */
template<>
struct Serializer<std::string> {
    static dbus_bool_t run(DBusMessageIter* it, const std::string& arg);
};
dbus_bool_t Serializer<std::string>::run(DBusMessageIter* it, const std::string& arg)
{
    return Serializer<const char*>::run(it, arg.c_str());
}
template<>
struct Deserializer<std::string> {
    static dbus_bool_t run(DBusMessageIter* it, std::string* arg);
};
dbus_bool_t Deserializer<std::string>::run(DBusMessageIter* it, std::string* arg)
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
