# Seq.hpp
Header-only function composition library for querying and manipulating collection and collection-like types using C++20.

## Getting started
If you've ever used a functional programming language then you won't have any problems using this library. You can go ahead to the **[usage options](#usage-options)**. You might also want to read the **[rationale](#rationale-and-context)** section to better understand why I created this project.

Otherwise, you can take a look at the **[examples](/examples)** folder to get a feeling for the library.

## Usage options
**With Meson**

Download the provided **[wrap file](Seq.hpp.wrap)** and place it in your `subprojects` folder. Then inside your `meson.build` file reference it like this:
```meson
seq_hpp_dep = dependency('Seq.hpp')

executable('...', '...',
    dependencies: [seq_hpp_dep])
```
Note that you can opt-in to use PCH to reduce compilation time with an additional parameter `cpp_pch: seq_hpp_dep.get_variable('pch')`.

**Without Meson**

Clone the repository and place the `seq` folder to your include directory e.g. `include`. Then make sure the path is visible during compilation using the `-Iinclude` flag and that you are using the `-std=c++20` standard.

## Contribution
If you have new functionality or a better way of writing already existing code then feel free to send in a pull request. In case you run into some bug you can open an issue ticket.

These are the tooling requirements:
- meson (>=1.2.0)
- clangd
- clang-tidy
- clang-format

The library has a soft-dependency on Meson to provide a centralized way of handling auxiliary tasks. These are the most important ones:

1. Creating a folder for build artifacts with the specified compiler and build options:
```
CXX=clang++ meson setup build -Dtests=ON -Dexamples=ON
```

2. Running the project's custom test suite:
```
meson test --verbose -C build
```

3. Formatting the source code of the project (sadly no built-in command for this one):
```
ninja clang-format -C build
```

4. Formatting the `meson.build` file:
```
meson format --inplace
```

## Rationale and context
The public API of the library is based on LINQ's [Enumerable class](https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable) and F#'s [Seq module](https://fsharp.github.io/fsharp-core-docs/reference/fsharp-collections-seqmodule) hence the name. They have pretty good documentation if you need to look up what each function does.

The question is why you would need this. You could technically say that `<algorithm>`, `<numeric>` and `<ranges>` already exist for a similar purpose. The main reason is they are really inconvenient to use.

The former two suffer from the issue that you have to spam begin-end iterator pairs everywhere. Like why can't the compiler infer whether the provided type does or does not work with the given function. The type system is more than capable of doing so. You fall into this situation where you start copy-pasting iterator functions like a madman, potentially running into runtime errors and the source code generally becomes more cluttered and harder to read.

The latter one fixes that problem but introduces new ones. The different function names are unnecessarily long and cryptic. Not to mention their signatures are so complex that you have no idea how they work internally.

I also could not find an already available library for my use case so I decided to make one from the ground up.
