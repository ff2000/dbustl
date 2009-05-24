/*
 *  DBusTL - D-Bus Template Library
 *
 *  Copyright (C) 2008, 2009  Fabien Chevalier <chefabien@gmail.com>
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

#include <iostream>
#include <cstdint>

#include <dbustl-1/dbustl>
#include <dbustl-1/GlibEventLoopIntegration>

using namespace std;
using namespace dbustl;

class Calculator : private DBusObject {
public:
    Calculator(Connection *conn);
private:
    int64_t add(int64_t a, int64_t b);
    int64_t div(int64_t a, int64_t b);
    void mul(int64_t a, int64_t b);
};

Calculator::Calculator(Connection *conn)
 : DBusObject("/Calculator", "com.CalculatorInterface", conn)
{
    exportMethod("add", this, &Calculator::add);
    exportMethod("div", this, &Calculator::div);
    
    exportMethod("mul", this, &Calculator::mul);
    exportSignal<int64_t>("mulResult");
}

int64_t Calculator::add(int64_t a, int64_t b)
{
    return a + b;
}

int64_t Calculator::div(int64_t a, int64_t b)
{
    if(b == 0) {
        throw DBusException(
            "com.CalculatorInterface.ParameterError",
            "Division by 0");
    }
    return a / b;
}

void Calculator::mul(int64_t a, int64_t b)
{
    emitSignal("mulResult", a * b);
}

int main(int, char **)
{    
    const string& serviceName = "com.myCalculator";
    GlibEventLoopIntegration mli;
    Connection::useEventLoop(mli);    
    Connection *session = Connection::sessionBus();
        
    if(!session->isConnected()) {
        return -1;
    }
    
    session->busRequestName(serviceName);
    
    cout << "D-Bus Calculator service running on '" << serviceName << "'" << endl;
    
    Calculator calc(session);
    
    GMainLoop *mainloop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(mainloop);        
    
    session->busReleaseName(serviceName);
    g_main_loop_unref(mainloop);

    return 0;
}
