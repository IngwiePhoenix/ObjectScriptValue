/*
    ObjectScript Value class, by Ingwie Phoenix

    This class saves, retains and keeps values from OS so you can use them at any point later.
    Be careful. the lifecycle of this class is connected to the one of hte variable.
    Delete this class -> Value is freed!

    Also, include your ObjectScript header before this file, so it'll work.
*/

#include <map>
#include <list>
#include <iostream>

#ifndef OSV_NO_INCLUDE
#include "objectscript.h"
#endif

class OSValue {
private:
    enum OSVType {
        OSV_NULL,
        OSV_BOOL,
        OSV_NUMBER,
        OSV_FLOAT,
        OSV_OBJ
    };
    OSVType varType;

    // Primitive types are not stored, so we store them.
    bool isNull;
    OS_NUMBER iVal;
    float fVal;
    bool bVal;

    ObjectScript::OS* myOS;
    int valueID;

public:
    OSValue(ObjectScript::OS*, int off);
    ~OSValue();

    void type();

    std::string get(std::string index) {
        myOS->pushValueById(valueID);
        myOS->getProperty(-1, index.c_str());
        std::string rtstr( myOS->popString().toChar() );
        myOS->pop();
        return rtstr;
    }

    /*
    operator int ();
    operator float ();
    operator bool ();
    operator std::string ();
    #ifdef OS_CHAR
    operator OS_CHAR ();
    #endif
    operator [](int index);
    operator [](std::string index);
    operator [](char* index);

    // Table operator: var[1,2]
    //operator [](int x, int y);
    */
};
