#!/usr/bin/env python

usage = """Usage:
python example-service.py
"""

# Copyright (C) 2004-2006 Red Hat Inc. <http://www.redhat.com/>
# Copyright (C) 2005-2007 Collabora Ltd. <http://www.collabora.co.uk/>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

import gobject

import dbus
import dbus.service
import dbus.mainloop.glib
from os import system

class DemoException(dbus.DBusException):
    _dbus_error_name = 'com.example.DemoException'

class SomeObject(dbus.service.Object):

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='', out_signature='')
    def SimpleProc(self):
        pass

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='s', out_signature='s')
    def SimpleHello(self, hello_message):
        return hello_message

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='b', out_signature='b')
    def test_boolean(self, b):
        return True

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='y', out_signature='y')
    def test_byte(self, b):
        return b + 1

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='n', out_signature='n')
    def test_int16(self, b):
        return b - 1

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='q', out_signature='q')
    def test_uint16(self, b):
        return b + 1

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='i', out_signature='i')
    def test_int32(self, b):
        return b - 1

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='u', out_signature='u')
    def test_uint32(self, b):
        return b + 1

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='x', out_signature='x')
    def test_int64(self, b):
        return b - 1

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='t', out_signature='t')
    def test_uint64(self, b):
        return b + 1

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='d', out_signature='d')
    def test_double(self, b):
        return b

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='as', out_signature='as')
    def test_array_of_string(self, array):
        return array

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='ai', out_signature='ai')
    def test_array_of_int(self, array):
        return array

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='aai', out_signature='aai')
    def test_array_of_array_of_integer(self, array):
        return array

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='a{is}', out_signature='a{is}')
    def test_dict_of_integer_string(self, dict):
        return dict

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='', out_signature='')
    def test_sleep_2s(self):
        system("sleep 2")
        return

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='', out_signature='')
    def RaiseException(self):
        raise DemoException('The RaiseException method does what you might '
                            'expect')

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='', out_signature='')
    def SendSignals(self):
        self.exampleSignal()
        self.exampleSignal2()
        self.exampleSignal3()
        self.exampleSignal4()

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='', out_signature='(ss)')
    def GetTuple(self):
        return ("Hello Tuple", " from example-service.py")

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='', out_signature='a{ss}')
    def GetDict(self):
        return {"first": "Hello Dict", "second": " from example-service.py"}

    @dbus.service.method("com.example.SampleInterface",
                         in_signature='', out_signature='')
    def Exit(self):
        mainloop.quit()

    @dbus.service.signal("com.example.SampleInterface",
                         signature='')
    def exampleSignal(self):
        pass

    @dbus.service.signal("com.example.SampleInterface",
                         signature='')
    def exampleSignal2(self):
        pass

    @dbus.service.signal("com.example.SampleInterface",
                         signature='')
    def exampleSignal3(self):
        pass

    @dbus.service.signal("com.example.SampleInterface",
                         signature='')
    def exampleSignal4(self):
        pass

if __name__ == '__main__':
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    session_bus = dbus.SessionBus()
    name = dbus.service.BusName("com.example.SampleService", session_bus)
    object = SomeObject(session_bus, '/PythonServerObject')

    mainloop = gobject.MainLoop()
    print "Running example service."
    print usage
    mainloop.run()
