#include "OSValue.h"
#include <iostream>

using namespace std;

OSValue::OSValue(ObjectScript::OS* os, int off) {
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

void OSValue::type() {
    cout << "Type: ";
    switch(varType) {
        case OSV_NULL:
            cout << "null";
        break;
        case OSV_BOOL:
            cout << "boolean";
        break;
        case OSV_NUMBER:
            cout << "number";
        break;
        case OSV_FLOAT:
            cout << "float";
        break;
        case OSV_OBJ:
            cout << myOS->getTypeStrById(valueID).toChar();
        break;
    }
    cout << endl;
}

OSValue::~OSValue() {
    myOS->releaseValueById(valueID);
}
