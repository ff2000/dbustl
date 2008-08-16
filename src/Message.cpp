#include <dbustl/Message>

namespace dbustl {

Message::Message() : _msg(NULL), _valid(true), _iteratorInitialized(false), _parsedArguments(0)
{
}

Message::Message(DBusMessage *msg)
  : _msg(msg), _valid(true), _iteratorInitialized(false), _parsedArguments(0)
{
}

Message::Message(const Message& other) 
{
    if(other._msg) {
        dbus_message_ref(other._msg);
    }
    _msg = other._msg;
    _valid = other._valid;
    _iteratorInitialized = other._iteratorInitialized;
    _parsedArguments = other._parsedArguments;
};

Message::~Message()
{
    if(_msg) {
        dbus_message_unref(_msg);
    }
};

Message& Message::operator=(Message& other) {
    if(other._msg) {
        dbus_message_ref(other._msg);
    }
    
    if(_msg) {
        dbus_message_unref(_msg);
    }
    
    _valid = other._valid;
    _iteratorInitialized = other._iteratorInitialized;
    _parsedArguments = other._parsedArguments;
    _msg = other._msg;
    return *this;
};

Message& Message::operator=(DBusMessage *msg) {
    if(_msg) {
      dbus_message_unref(_msg);
    }
    
    _valid = true;
    _iteratorInitialized = false;
    _parsedArguments = 0;
    _msg = msg;
    return *this;
};


}
