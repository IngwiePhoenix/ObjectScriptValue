#include "OSValue.h"
#include <iostream>

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
    switch(varType) {
        case OSV_NULL:
        case OSV_BOOL:
        case OSV_NUMBER:
        case OSV_FLOAT:
            return NULL;

        case OSV_OBJ:
            switch(myOS->getTypeById(valueID)) {
                case OS_VALUE_TYPE_UNKNOWN:
                case OS_VALUE_TYPE_NULL:
                case OS_VALUE_TYPE_BOOL:
                case OS_VALUE_TYPE_NUMBER:
                case OS_VALUE_TYPE_USERDATA:
                case OS_VALUE_TYPE_USERPTR:
                case OS_VALUE_TYPE_FUNCTION:
                case OS_VALUE_TYPE_CFUNCTION:
                    // Those values can not be indexed.
                    return NULL;
                case OS_VALUE_TYPE_STRING:
                    /* Strings need int indexes.
                    myOS->pushValueById(valueID);
                    OS::String str = myOS->toString();
                    myOS->pop();
                    return (char*)(str[index]);
                    */
                    return NULL;
                case OS_VALUE_TYPE_ARRAY:
                    myOS->pushValueById(valueID);
                    /* Obtain string value from array... */
                    myOS->pop();
                    return NULL;
                case OS_VALUE_TYPE_OBJECT:
                    myOS->pushValueById(valueID);
                    myOS->getProperty(index);
                    char* str = (char*)(myOS->toString().toChar());
                    myOS->pop();
                    return str;
            }
    }
    cout << "*** OSValue: Reached end of 'Value::get(...)' ***" << endl;
    return NULL;
}

string Value::get(string index) {
    return string( get(index.c_str()) );
}

}
