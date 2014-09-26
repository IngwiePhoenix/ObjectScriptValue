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
    int get(int index);

    // Operator[] to overlay get(...)
    std::string operator [](std::string index);
    char* operator [](char* index);
    //const char* operator [](long, const char* index);

    // Objects.
    bool set(std::string key, int value);
    bool set(std::string key, float value);
    bool set(std::string key, bool value);
    bool set(std::string key, std::string value);
    bool set(std::string key, char* value);
    bool set(std::string key, const char* value);
    bool set(std::string key, Value value);

    // C-style string keys.
    bool set(const char* key, int value);
    bool set(const char* key, float value);
    bool set(const char* key, bool value);
    bool set(const char* key, std::string value);
    bool set(const char* key, char* value);
    bool set(const char* key, const char* value);
    bool set(const char* key, Value value);

    // Arrays
    bool set(int key, int value);
    bool set(int key, float value);
    bool set(int key, bool value);
    bool set(int key, std::string value);
    bool set(int key, char* value);
    bool set(int key, const char* value);
    bool set(int key, Value value);

    // Operators to "cast" to specific type.
    /*
    operator int ();
    operator float ();
    operator bool ();
    operator std::string ();
    */
};

}
