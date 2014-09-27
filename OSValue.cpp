#include "OSValue.h"
#include <iostream>
#include <stdlib.h>

namespace ObjectScript {

using namespace std;

Value::Value(ObjectScript::OS* os, int off) {
    if(os->isNumber(off)) {
        iVal = os->toNumber(off);
        varType = OSV_NUMBER;
    } else if(os->isType(ObjectScript::OS_VALUE_TYPE_BOOL, off)) {
        bVal = os->toBool(off);
        varType = OSV_BOOL;
    } else if(os->isNull(off)) {
        isNull = true;
        varType = OSV_NULL;
    } else if(os->isString(off) || os->isObject(off) || os->isArray(off) || os->isFunction(off)) {
        valueID = os->getValueId(off);
        os->retainValueById(valueID);
        myOS = os;
        varType = OSV_OBJ;
    } else {
        cerr << "*** OSValue: Supplied value is neither Number, Bool, Null, String, Object, Array or Function! ***" << endl;
        cerr << "*** OSValue: We only support above types. ***" << endl;
    }
}

string Value::type() {
    switch(varType) {
        case OSV_NULL:
            return string("null");
        case OSV_BOOL:
            return string("boolean");
        case OSV_NUMBER:
            return string("number");
        case OSV_OBJ:
            return string(myOS->getTypeStrById(valueID).toChar());
    }
}

Value::~Value() {
    if(varType == OSV_OBJ) myOS->releaseValueById(valueID);
}

char* Value::get(char* index) {
    // Used later to avoid error about protected scope.
    OS::String o_str(myOS);

    if(varType == OSV_OBJ) {
        switch(myOS->getTypeById(valueID)) {
            case OS_VALUE_TYPE_NULL:
            case OS_VALUE_TYPE_BOOL:
            case OS_VALUE_TYPE_NUMBER:
            case OS_VALUE_TYPE_USERDATA:
            case OS_VALUE_TYPE_USERPTR:
            case OS_VALUE_TYPE_FUNCTION:
            case OS_VALUE_TYPE_CFUNCTION:
            case OS_VALUE_TYPE_ARRAY:
                // Those values can not be indexed with a string value.
                return NULL;
            case OS_VALUE_TYPE_STRING:
                // Strings need int indexes.
                myOS->pushValueById(valueID);
                o_str = myOS->toString();
                myOS->pop();
                // Not elegant, as OS_CHAR could be char* ... whatever.
                return (char*)(o_str[atoi(index)]);
            case OS_VALUE_TYPE_OBJECT:
                myOS->pushValueById(valueID);
                myOS->pushString(index);
                myOS->getProperty();
                return (char*)(myOS->popString().toChar());
        }
    }
    cerr << "*** OSValue: Reached end of 'char* Value::get(char*)' ***" << endl;
    return NULL;
}

string Value::get(string index) {
    return string( get(index.c_str()) );
}

int Value::get(int index) {
    if(varType == OSV_OBJ) {
        switch(myOS->getTypeById(valueID)) {
            case OS_VALUE_TYPE_NULL:
            case OS_VALUE_TYPE_BOOL:
            case OS_VALUE_TYPE_NUMBER:
            case OS_VALUE_TYPE_USERDATA:
            case OS_VALUE_TYPE_USERPTR:
            case OS_VALUE_TYPE_FUNCTION:
            case OS_VALUE_TYPE_CFUNCTION:
                return NULL;
            case OS_VALUE_TYPE_STRING:
                // Indexing a string results in a char. No go for ints here.
                return NULL;
            case OS_VALUE_TYPE_ARRAY:
                myOS->pushValueById(valueID);
                myOS->pushNumber(index);
                myOS->getProperty();
                return myOS->popInt();
            case OS_VALUE_TYPE_OBJECT:
                myOS->pushValueById(valueID);
                myOS->pushNumber(index);
                myOS->getProperty();
                return myOS->popInt();
        }
    }
    cerr << "*** OSValue: Reached end of 'int Value::get(int)' ***" << endl;
    return NULL;

}

// Overloaders point at ::get(...) methods. For objects.
//const string Value::operator [](string index) { return get(index); }
const char* Value::operator [](const char* index) { return (const char*)get(index).c_str(); }

// Caster
/*Value::operator int () {
    int o_len;
    switch(varType) {
        case OSV_NULL:
            return 0;
        case OSV_BOOL:
            return (int)bVal;
        case OSV_NUMBER:
            return iVal;
        case OSV_OBJ:
            switch(myOS->getTypeById(valueID)) {
                case OS_VALUE_TYPE_NULL:
                case OS_VALUE_TYPE_BOOL:
                case OS_VALUE_TYPE_NUMBER:
                case OS_VALUE_TYPE_STRING:
                    return NULL;
                case OS_VALUE_TYPE_ARRAY:
                case OS_VALUE_TYPE_OBJECT:
                    // These values are ID'd. So we can go from there.
                    myOS->pushValueById(valueID);
                    o_len = myOS->getLen();
                    myOS->pop();
                    return o_len;
                case OS_VALUE_TYPE_USERDATA:
                case OS_VALUE_TYPE_USERPTR:
                case OS_VALUE_TYPE_FUNCTION:
                case OS_VALUE_TYPE_CFUNCTION:
                    // No chance we're able to return a length here.
                    return NULL;
            }
    }
}*/
/*Value::operator float () {
    // This is ONLY possible on a numeric value.
    if(varType == OSV_NUMBER) {
        return fVal;
    } else return NULL;
}*/
Value::operator bool() {
    // This pretty much only works on bools.
    if(varType == OSV_BOOL) {
        return bVal;
    } else return NULL;
}
Value::operator string () {
    // Strings are a different topic...but do-able.
    if(varType == OSV_OBJ) {
        myOS->pushValueById(valueID);
        OS::String str = myOS->toString();
        myOS->pop();
        return string( str.toChar() );
    } else {
        // o.o i am too lazy for this now.
        return string("<Lazyness wins.>");
    }
}

}
