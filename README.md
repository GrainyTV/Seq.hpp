# Seq.hpp
Header-only function composition library for querying and manipulating collection and collection-like types using C++20.
```cpp
#include "seq/seq.hpp"
#include <iostream>

int main()
{
    auto squares =
        Seq::range(5)
        | Seq::filter([](int n) { return n % 2 == 0; })
        | Seq::map([](int n) { return n * n; });

    for (int s : squares)      // consume sequence
        std::cout << s << " "; // prints 0 4 16
}
```

## Getting started
If you've ever used a functional programming language then you won't have any problems using this library. You can go ahead to the **[usage options](#usage-options)**. You might also want to read the **[rationale](#rationale)** section.

Otherwise, you can take a look at the **[examples](/examples)** folder to get a feeling for the library.

## Usage options
**With Meson**

Download the provided **[wrap file](Seq.hpp.wrap)** and place it in your `subprojects` folder. Then inside your `meson.build` file reference it like this:
```meson
seq_hpp_dep = dependency('Seq.hpp')

executable('...', '...',
    dependencies: [seq_hpp_dep],
)
```

**Manually**

Clone the repository and copy the `seq` folder to your include directory. Make sure you are using at least C++20.

## Contribution
If you have new functionality or rewrite of existing code, feel free to send in a pull request. In case you run into some bug you can open an issue ticket.

### Project Dependencies
- `meson` >= 1.2.0
- `clang-tidy` >= 20
- `clang-format` >= 20

### How to Build
1. Create a build artifact with developer mode enabled. You can specify a compiler of choice.
```
CXX=clang++ meson setup build -Denable_dev=true
```

2. Change into the newly created folder.
```
cd build
```

3. Run the provided dev target. It will build and run the tests and examples, then format the source code.
```
meson compile dev
```

If you didn't encounter any issues running the target, your repository is ready for a pull request.

## Rationale
The public API of the library is based on the LINQ **[Enumerable](https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable)** class and **[Seq](https://fsharp.github.io/fsharp-core-docs/reference/fsharp-collections-seqmodule)** module from C# and F# respectively - hence the name. Their documentation is pretty good in case you need to look up what each function does.

You might ask what's the point when `algorithm`, `numeric` and `ranges` already exist for a similar purpose.

With the former two the problem is you have to spam begin-end iterator pairs everywhere. The compiler should be able to infer whether the type of a supplied variable is valid for a given operation but it doesn't, so you fall into this situation where you start copy-pasting iterator functions like a madman, potentially running into runtime errors and the source code generally becomes cluttered.

Ranges is a bit better but still not convenient to use. The provided function names are unnecessarily long and cryptic. Plus, their signatures are so complex that you have no idea how they work internally.
