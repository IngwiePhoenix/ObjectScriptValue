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
OSValue* val;
int testFunc2(OS* os, int params, int, int, void*) {
    val = new OSValue(os, -params+0);
    val->type();
    return 0;
}

// How about we actually get something off it?
// Assuming: { foo: "bar" }
int testFunc3(OS* os, int params, int, int, void*) {
    OSValue obj(os, -params+0);
    cout << "Foo: " << obj.get("foo") << endl;
    return 0;
}

int main() {
    OS* os = OS::create();

    os->pushCFunction(testFunc1);
    os->setGlobal("testFunc1");

    os->pushCFunction(testFunc2);
    os->setGlobal("testFunc2");

    os->pushCFunction(testFunc3);
    os->setGlobal("testFunc3");

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
