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

namespace ObjectScript {

class Value {
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

    OS* myOS;
    int valueID;

public:
    Value(OS*, int off);
    ~Value();

    // Type info
    std::string type();

    // Getter
    std::string get(std::string index);
    char* get(char* index);

    bool set(std::string key, int value);
    bool set(std::string key, float value);
    bool set(std::string key, bool value);
    bool set(std::string key, std::string value);
    bool set(std::string key, char* value);
    bool set(std::string key, const char* value);
    bool set(std::string key, Value value);

    bool set(const char* key, int value);
    bool set(const char* key, float value);
    bool set(const char* key, bool value);
    bool set(const char* key, std::string value);
    bool set(const char* key, char* value);
    bool set(const char* key, const char* value);
    bool set(const char* key, Value value);

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

}
