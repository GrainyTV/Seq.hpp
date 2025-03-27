# Seq.hpp
This project is a header-only function composition library that makes it easier to work with sequence types. The inspiration behind this project is that I felt C++ was lacking in this aspect compared to something like F# where the [sequence module](https://fsharp.github.io/fsharp-core-docs/reference/fsharp-collections-seqmodule.html) is really powerful and I could not find an equivalent library that achieves this. The oldest supported version is C++14 where [polymorphic closures](https://isocpp.org/wiki/faq/cpp14-language#generic-lambdas) were first introduced.

## How To Use
The whole project is defined in a single file so you do not need to build anything in advance, all you have to do is include it and then you can start using it. I tried to go for a moderately simplistic API. Check out the [getting started](docs/getting_started.md) guide and the examples for more information.
