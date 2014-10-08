#include <iostream>
#include "objectscript.h"

namespace ObjectScript {
namespace Value {

    // The base class.
    class Var {
    protected:
        int offset;
    public:
        OS_EValueType type;
        OS* myOS;
        int valueID;
        Var(OS* os, int off=-1) : valueID(-1), offset(off) {
            myOS=os;
            type = myOS->getType(off);
        }
    };

    // Null, Bool, Int/Float
    class Primitive : public Var {
    private:
        int iVal;
        float fVal;
        OS_NUMBER nVal;
        bool bVal;
    public:
        Primitive(OS* os, int off=-1) : Var(os, off) { initialize(); }
        bool initialize() {
            if(type == OS_VALUE_TYPE_NULL) {
                iVal = 0;
                fVal = 0.0;
                bVal = false;
                nVal = 0;
                return true;
            }
            if(type == OS_VALUE_TYPE_BOOL) {
                bVal = myOS->toBool(offset);
                iVal = (bVal?1:0);
                fVal = (bVal?1.0:0.0);
                nVal = (OS_NUMBER)fVal;
                return true;
            } else if(type == OS_VALUE_TYPE_NUMBER) {
                iVal = myOS->toInt(offset);
                fVal = myOS->toFloat(offset);
                nVal = myOS->toNumber(offset);
                bVal = (iVal!=0?1:0);
                return true;
            }
            return false;
        }
        operator int() { return iVal; }
        operator float() { return fVal; }
        operator OS_NUMBER() { return nVal; }
        operator bool() { return bVal; }
    };

    class String : public Var {
    public:
        String(OS* os, int off=-1) : Var(os, off) { initialize(); }
        ~String() { myOS->releaseValueById(valueID); }
        bool initialize() {
            if(type == OS_VALUE_TYPE_STRING) {
                // We can store a valueID here.
                valueID = myOS->getValueId(offset);
                return true;
            }
            return false;
        }
        operator const char*() {
            myOS->pushValueById(valueID);
            return myOS->popString().toChar();
        }
        operator std::string() {
            myOS->pushValueById(valueID);
            return std::string( myOS->popString().toChar() );
        }
    };

    // Array containing primitives or Strings
    class Array : public Var {
    public:
        Array(OS* os, int off=-1) : Var(os, off) { initialize(); }
        ~Array() { myOS->releaseValueById(valueID); }
        bool initialize() {
            if(type == OS_VALUE_TYPE_ARRAY) {
                valueID = myOS->getValueId(offset);
                return true;
            }
            return false;
        }
        Var* operator [](int index);
        int length() {
            myOS->pushValueById(valueID);
            int len = myOS->getLen();
            myOS->pop();
            return len;
        }
    };

    // key=>value pairs
    class Object : public Var {
    public:
        Object(OS* os, int off=-1) : Var(os, off) { initialize(); }
        ~Object() { myOS->releaseValueById(valueID); }
        bool initialize() {
            if(type == OS_VALUE_TYPE_OBJECT) {
                valueID = myOS->getValueId(offset);
                return true;
            }
            return false;
        }
        Var* operator [](std::string index);
    };

    // Callables
    class Function : public Var {
    public:
        Function(OS* os, int off=-1) : Var(os, off) { initialize(); }
        ~Function() { myOS->releaseValueById(valueID); }
        bool initialize() {
            if(type == OS_VALUE_TYPE_FUNCTION) {
                valueID = myOS->getValueId(offset);
                return true;
            }
            return false;
        }
        bool operator ()(int arguments, void* userData) {
            std::cout << "Calling function with " << arguments << " arguments." << std::endl;
            return true;
        }
    };

    Var* Array::operator [](int index) {
        myOS->pushValueById(valueID);

        // Fetch the type and return accordingly.
        myOS->pushNumber(index);
        myOS->getProperty(); // Get array element.
        OS_EValueType _type = myOS->getType();
        if(_type == OS_VALUE_TYPE_NULL) {
            myOS->pushNumber(-1);
            Primitive* rt = new Primitive(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_BOOL || _type == OS_VALUE_TYPE_NUMBER) {
            Primitive* rt = new Primitive(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_STRING) {
            String* rt = new String(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_ARRAY) {
            Array* rt = new Array(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_OBJECT) {
            Object* rt = new Object(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_FUNCTION) {
            Function* rt = new Function(myOS);
            myOS->pop();
            return rt;
        } else {
            std::cerr << "** ObjectScript Value: Error in Array::operator[]()! **" << std::endl;
            myOS->pop();
            myOS->pushBool(false);
            Primitive* rt = new Primitive(myOS);
            myOS->pop();
            return rt;
        }
    }

    Var* Object::operator [](std::string index) {
        myOS->pushValueById(valueID);

        // Fetch the type and return accordingly.
        myOS->pushString(index.c_str());
        myOS->getProperty(); // Get array element.
        OS_EValueType _type = myOS->getType();
        if(_type == OS_VALUE_TYPE_NULL) {
            myOS->pushNumber(-1);
            Primitive* rt = new Primitive(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_BOOL || _type == OS_VALUE_TYPE_NUMBER) {
            Primitive* rt = new Primitive(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_STRING) {
            String* rt = new String(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_ARRAY) {
            Array* rt = new Array(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_OBJECT) {
            Object* rt = new Object(myOS);
            myOS->pop();
            return rt;
        } else if(_type == OS_VALUE_TYPE_FUNCTION) {
            Function* rt = new Function(myOS);
            myOS->pop();
            return rt;
        } else {
            std::cerr << "** ObjectScript Value: Error in Array::operator[]()! **" << std::endl;
            myOS->pop();
            myOS->pushBool(false);
            Primitive* rt = new Primitive(myOS);
            myOS->pop();
            return rt;
        }
    }

}
}
