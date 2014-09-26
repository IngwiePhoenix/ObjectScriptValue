#include <iostream>
#include "objectscript.h"
#include "OSValue.h"

using namespace std;
using namespace ObjectScript;

// How to use a OS String
int testFunc1(OS* os, int params, int, int, void*) {
    string ex( os->toString(-params+0).toChar() );
    cout << "Example string: " << ex << endl;
    return 0;
}

// But now, lets get serious. Verify the type.
Value* val;
int testFunc2(OS* os, int params, int, int, void*) {
    val = new Value(os, -params+0);
    cout << "Type: " << val->type() << endl;
    return 0;
}

// How about we actually get something off it?
// Assuming: { foo: "bar" }
int testFunc3(OS* os, int params, int, int, void*) {
    Value obj(os, -params+0);
    cout << "Foo: " << obj["foo"] << endl;
    return 0;
}

int main() {
    OS* os = OS::create();

    OS::FuncDef func1 = {OS_TEXT("testFunc1"), testFunc1};
    OS::FuncDef func2 = {OS_TEXT("testFunc2"), testFunc2};
    OS::FuncDef func3 = {OS_TEXT("testFunc3"), testFunc3};
    os->setGlobal(func1);
    os->setGlobal(func2);
    os->setGlobal(func3);

    // Basic
    os->eval("testFunc1 'meep!';");

    // Type testing
    os->eval("testFunc2( {key: 20} )");
    os->eval("testFunc2(function(){return true;})");
    os->eval("testFunc2(null)");
    os->eval("testFunc2(3.19)");
    os->eval("testFunc2([1, '2', {k:3}])");

    // Getting properties. Expecting Foo here-
    os->eval("testFunc3( {foo: \"bar\"} )");

    cout << "Current stack size: " << os->getStackSize() << endl;
    cout << "Last value is a: " << os->getTypeStr().toChar() << endl;

    cout << "Gonna foo it again!" << endl;
    os->eval("testFunc3( {foo: \"bar\"} )");
}
