#include "os-value.h"
#include "objectscript.h"
#include <iostream>

using namespace std;
using namespace ObjectScript;

int main(void) {
    OS* os = OS::create();
    //os->retain();

    /*
        In the following tests, we're pushing values into the stack,
        just as it would appear in a function call.

        We then return the actual value as string,
        then test it as the C++ instance.
    */

    // 0. Helper macros
    #define printReal() cout << "The actual value is: " << os->toString(-1).toChar() << endl
    #define next() cout << endl;

    // 1. Numbers
    os->pushNumber(10);
    printReal();
    Value::Primitive n(os);
    cout << "Primitive is: " << (int)n << endl;
    os->pop();
    next()

    // 2. Floats
    os->pushNumber(3.91);
    printReal();
    Value::Primitive f(os);
    cout << "Primitive as float is: " << (float)f << endl;
    os->pop();
    next()

    // 3. String
    os->pushString("Fubar'd");
    printReal();
    Value::String s(os);
    cout << "String value is: " << (const char*)s << endl;
    os->pop();
    next()

    // 4. Let's try out an array...this is harder.
    os->newArray();
    os->pushNumber(99);
    os->addProperty(-2);
    os->pushString("Dohlen");
    os->addProperty(-2);
    // Array resides within -1, and has consumed the other values. -2 and -3 are null.
    // Now, let's try to test it.
    printReal();
    /*
        Accessing an array is not easy, and creating values of it, is neither.
        In V8, you have a Handle<> class - it uses templating.
        In the future, I will use this too. For now, just cast the value
        to the proper Value type.
    */
    Value::Array a(os);
    Value::Primitive* pn = (Value::Primitive*)a[0];
    Value::String* ps = (Value::String*)a[1];
    // We have to dereference the pointer, so we can trigger the casting methods again...
    // I am sure there is an easier way to aproach this!...somehow.
    cout << "[" << (int)(*pn) << "] -> " << (const char*)(*ps) << endl;
    // Array test complete.
    delete pn;
    delete ps;
    os->pop();
    next()

    // 5. Objects.
    os->newObject();
    os->pushString("bar");
    os->setProperty(-2, "foo");
    // { "foo": "bar" }
    printReal();
    Value::Object o(os);
    Value::String* obj_s = (Value::String*)o["foo"];
    cout << "foo -> " << (const char*)(*obj_s) << endl;
    delete obj_s;
    os->pop();
    next()

    // 6. And now...functions.
    // THIS is a hard one! O.O
    os->eval("function greeter() { echo \"Greetings, C++! How\'re you doing?\\n\" }");
    os->getGlobal("greeter"); // Put the function to stack
    printReal();
    Value::Function func(os);
    func(0,0);
    next()

    // Let's try a method.
    os->eval("myObj = {meeper:function(){ echo \"Meep meep!\\n\" }}");
    os->getGlobal("myObj");
    printReal();
    Value::Object myObj(os);
    Value::Method mt(os, &myObj, "meeper");
    mt(0,0);
    next()

    // 8. Functions, with parameters.
    os->eval("function greeter2(name) { echo \"Hey ${name}!\\n\"}");
    os->getGlobal("greeter2");
    printReal();
    Value::Function greeter2(os);
    os->pushString("Ingwie Phoenix");
    greeter2(1,0);
    next()

    // 9. Object method with parameter
    os->eval("myObj2 = { myFunc: function(e) { echo \"Debug message is: ${e}\\n\" } }");
    os->getGlobal("myObj2");
    printReal();
    Value::Object myObj2(os);
    Value::Method myFunc(os, &myObj2, "myFunc");
    os->pushString("Everything's okay!");
    myFunc(1,0);

    os->release();
}
