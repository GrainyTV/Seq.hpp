# Getting Started
If you have ever used either a functional programming language or bash scripting, then the use of this library will feel pretty self-explanatory. The header file provides an overriden pipe operator and a bunch of (mainly higher-order) functions. The core idea is that the output of an operation will be used as the input of the next one, so you can chain them together. Let's look at a concrete use case.

Consider a function that finds the sum of all positive numbers from an inclusive interval. You could write this similarly to the example below. It is a valid solution in any case but more error prone than it should be. When dealing with loops and indices, I always seem run into all sorts of runtime errors like overindexing, off-by one errors, or infinite loops.

```
template<int Bottom, int Top>
int calculate()
{
    int sum = 0;

    for (int number = Bottom; number <= Top; ++number)
    {
        if (number % 2 == 0)
        {
            sum += number;
        }
    }

    return sum;
}
```
This is where my library comes into play. What if you could write the same code but in a more compact way, while avoiding these issues.
```
template<int Bottom, int Top>
int calculate()
{
    return 
        Seq::range<Bottom, Top + 1>()
        | Seq::filter(isEven)
        | Seq::sum();
}
```
If you have no idea how this is the same code don't worry, let me explain. You need to focus on the pipe characters. It is a binary operator with two sides. On the left side, you will always have a sequence expression. What is a sequence expression, you might ask. It can be a bunch of different types: arrays, c-arrays, vectors, strings, maps or even custom containers. The only criteria is that the type should have a valid begin + end combination. Now on the right side, you will always have a closure returned by one of the functions from the Seq namespace. These closures take a sequence expression as parameter and have the actual functions "baked" into them that will be invoked on each individual items. Then if the closure's return value is a valid sequence expression as well, it can be the left side of the next expression.
