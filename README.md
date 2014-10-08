# ObjectScript Value
## A C++ class to make access and storage of ObjectScript values easier.

Are you working with ObjectScript too, and want to take advantage of its objective aproach even from within your C++ app? Now, you can!

```c++
#include "objectscript.h"
#include "os-value.hpp"

using namespace ObjectScript;

// Within a native function registered with OS,
// you can do something like this:
Value::Object obj(os);

// You then can fetch values, like so.
Value::String* str = (Value::String*)obj["MyKey"];
```

See more in `test.cpp`, as I keep updating this API.


## Features
It's ment to help you with OS development, so it will:

- Understand every value type used by ObjectScript.
- Convert the variables into the type you want - `std::string`, `const char*` and alike.
- Help you store objects and other values away for later use. I.e.: `map<string, Value::Object>`
- Synchronize the life cycle. If you delete a Value, then it also disappears from OS.

Currently, its a header-only thing, but more for convenience. It's ment to be changed later on.

### In development
This code is being made out of a personal need for my other project IceTea. But I want to make it possible to handle all OS types that exist. The API I look up to is V8.
