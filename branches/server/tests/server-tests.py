#!/usr/bin/env python

import dbus
bus = dbus.SessionBus()
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

try:
    proxy.test_ex1()
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.mycompany.test: This is a test exception"

try:
    proxy.test_ex2()
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.dbustl.CPPException: Error"

try:
    proxy.test_ex3()
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.dbustl.CPPException: Unknown C++ exception"

assert proxy.test_flexible_executor(1.0, 1.0) == 1

try:
    proxy.test_flexible_executor(1.0, 0.0)
    assert False
except dbus.exceptions.DBusException, ex:
    assert str(ex) == "org.mycompany.test: Division by 0"

proxy.test_signal()
proxy.test_signal2()

proxy.stop()

print "Ok"
