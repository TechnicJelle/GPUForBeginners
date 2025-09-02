All helpful contributions are welcome, however small they may be!

Let's make this guide the best guide there is!

# Writing style

Colour is spelled in American style, to match SDL's API.

All images should have a proper alt text.
It should be at least one full sentence, starting with a capital and ending with a period.

# Coding style

Curly braces on the next line.

Classes and Structs are start with a capital,
and words in them also start with a capital.  
Example: `struct Vertex{}`, `struct MyAppState{}`

Variable names start with lowercase,
and words in them start with a capital.  
Example: `window`, `vertexBuffer`

Spaces between `if`, `switch` and the parentheses.

With pointers, the `*` goes on the left, with the type.  
Do not define multiple variables on the same line,
except for very primitive types in data classes.  
For example, this is allowed:

```c++
struct Vertex
{
	float x, y, z;
};
```

But this is not:

```c++
struct MyAppState
{
	SDL_GPUBuffer* vertexBufferA, vertexBufferB = nullptr;
};
```

When referring to an option in a struct definition,
put the `.` in front of the option name.  
Example: `.load_op`, `.rasterizer_state`
