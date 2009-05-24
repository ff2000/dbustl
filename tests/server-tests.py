#!/usr/bin/env python

import sys

import dbus
bus = dbus.SessionBus()

xml = bus.get_object('com.example.SampleService', '/').Introspect()         
xml += bus.get_object('com.example.SampleService', '/ServerObject').Introspect()         
xml += bus.get_object('com.example.SampleService', '/ServerObject/Child').Introspect()         
xml += bus.get_object('com.example.SampleService', '/Not').Introspect()         
xml += bus.get_object('com.example.SampleService', '/Not/A').Introspect()         
xml += bus.get_object('com.example.SampleService', '/Not/A/Child').Introspect()         
xml += bus.get_object('com.example.SampleService', '/Not/A/Child/Class').Introspect()         

if len(sys.argv) > 1 and sys.argv[1] == '-w':
    f = open('introspection.txt', 'w')
    f.write(xml)
    f.close()
else:
    f = open('introspection.txt')
    assert xml == f.read()

proxy = bus.get_object('com.example.SampleService',
                       '/ServerObject')
                       
proxy.test_callvoid0()
proxy.test_callvoid1(1)
proxy.test_callvoid2(1, 1)
proxy.test_callvoid3(1, 1, 1)
                       
assert proxy.test_call0() == 0
assert proxy.test_call1(1) == 1
assert proxy.test_call2(1, 1) == 2
assert proxy.test_call3(1, 1, 1) == 3

#We should complain if signature is invalid
try:
    proxy.test_call2(1, 0.5)
    assert False
except dbus.exceptions.DBusException, ex:
    pass

#Exception test 1
try:
    proxy.test_ex1()
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.mycompany.test: This is a test exception"

#Exception test 2
try:
    proxy.test_ex2()
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.dbustl.CPPException: Error"

#Exception test 3
try:
    proxy.test_ex3()
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.dbustl.CPPException: Unknown C++ exception"

#Flexible executor test
assert proxy.test_flexible_executor(1.0, 1.0) == 1

#Flexible executor exception test
try:
    proxy.test_flexible_executor(1.0, 0.0)
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.mycompany.test: Division by 0"

#Test interfaces
proxy.test_if()
assert proxy.test_if(dbus_interface='com.example.Interface1') == 'com.example.Interface1'
assert proxy.test_if(dbus_interface='com.example.Interface2') == 'com.example.Interface2'

#FIXME: check those signals are really received
proxy.test_signal()
proxy.test_signal2()
proxy.test_signal3()

proxy.stop()

print "Ok"
