# ObjectScript Value
## A C++ class to make access and storage of ObjectScript values easier.

Are you working with ObjectScript too, and want to take advantage of its objective aproach even from within your C++ app? Now, you can!

```c++
using namespace ObjectScript;

Value val(os, MyOffset);

// Convert it to a string...or char.
std::string foo = val;
char* foo = val;

// Access object properties
string hostname = val["hostname"];
// across multiple levels?
// Let's assume a Yii-kind of config: components -> redis -> host
string redisHost = val["components"]["redis"]["host"];

// Want to use it as a pointer? Okay. But you'll loose []'s.
Value* config;
void myFunction() {
	OS* os = OS::create();
	os->eval("return { username: 'Meep', password: 'Boop' };");
	config = new OSValue(os, -1); // Access latest value.
}
// Some point later...
void logIn() {
	string uname = config->get("username");
	string pword = config->get("password");
	// ...
}
```

### In development
This code is being made out of a personal need for my other project IceTea. But I want to make it possible to handle all OS types that exist. The API I look up to is V8.
