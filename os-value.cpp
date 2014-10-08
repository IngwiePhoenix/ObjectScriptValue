#include <iostream>
#include "objectscript.h"
#include "os-value.h"

namespace ObjectScript {
namespace Value {

// Var, base class
Var::Var(OS* os, int off=-1) : valueID(-1), offset(off) {
    myOS=os;
    type = myOS->getType(off);
}

// Primitive, Null, Int, Float, Bool.
Primitive::Primitive(OS* os, int off) : Var(os, off) { initialize(); }
Primitive::Primitive(OS* os) : Var(os, -1) { initialize(); }

bool Primitive::initialize() {
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

Primitive::operator int() { return iVal; }
Primitive::operator float() { return fVal; }
Primitive::operator OS_NUMBER() { return nVal; }
Primitive::operator bool() { return bVal; }

// String class
String::String(OS* os, int off) : Var(os, off) { initialize(); }
String::String(OS* os) : Var(os, -1) { initialize(); }
String::~String() { myOS->releaseValueById(valueID); }

bool String::initialize() {
    if(type == OS_VALUE_TYPE_STRING) {
        // We can store a valueID here.
        valueID = myOS->getValueId(offset);
        myOS->retainValueById(valueID);
        return true;
    }
    return false;
}

String::operator const char*() {
    myOS->pushValueById(valueID);
    return myOS->popString().toChar();
}
String::operator std::string() {
    myOS->pushValueById(valueID);
    return std::string( myOS->popString().toChar() );
}

// Array class
Array::Array(OS* os, int off) : Var(os, off) { initialize(); }
Array::Array(OS* os) : Var(os, -1) { initialize(); }
Array::~Array() { myOS->releaseValueById(valueID); }

bool Array::initialize() {
    if(type == OS_VALUE_TYPE_ARRAY) {
        valueID = myOS->getValueId(offset);
        myOS->retainValueById(valueID);
        return true;
    }
    return false;
}

int Array::length() {
    myOS->pushValueById(valueID);
    int len = myOS->getLen();
    myOS->pop();
    return len;
}

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

// Object class
Object::Object(OS* os, int off) : Var(os, off) { initialize(); }
Object::Object(OS* os) : Var(os, -1) { initialize(); }
Object::~Object() { myOS->releaseValueById(valueID); }

bool Object::initialize() {
    if(type == OS_VALUE_TYPE_OBJECT) {
        valueID = myOS->getValueId(offset);
        myOS->retainValueById(valueID);
        return true;
    }
    return false;
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

// Function class
Function::Function(OS* os, int off) : Var(os, off) { initialize(); }
Function::Function(OS* os) : Var(os, -1) { initialize(); }
Function::~Function() { myOS->releaseValueById(valueID); }

bool Function::initialize() {
    if(type == OS_VALUE_TYPE_FUNCTION) {
        valueID = myOS->getValueId(offset);
        myOS->retainValueById(valueID);
        return true;
    }
    return false;
}

void Function::operator ()(int args, int rtVals) {
    myOS->pushValueById(valueID);
    myOS->call(args, rtVals);
}

// Finally, Method class
Method::Method(OS* os, Object* obj, const char* methodName) {
    Obj = obj;
    myOS = os;
    os->pushValueById(obj->valueID);
    os->pushString(methodName);
    os->getProperty();
    Mtd = new Function(os);
    os->pop(); // Property
    os->pop(); // Object
}
Method::~Method() {
    Obj->~Object();
    Mtd->~Function();
}
void Method::operator ()(int args, int rtVals) {
    myOS->pushValueById(Obj->valueID);
    myOS->pushValueById(Mtd->valueID);
    myOS->callTF(args, rtVals);
}

}
}
