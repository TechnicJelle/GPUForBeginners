All helpful contributions are welcome, however small they may be!

Let's make this guide the best guide there is, _together_!

# Writing style

## Spelling

Colour is spelled in American style, to match SDL's API.

## Images

All images should have a proper alt text.

It should be at least one full sentence, starting with a capital and ending with a period.

## Referring to Struct Options

When referring to an option in a struct definition,
put the `.` in front of the option name.

Examples: `.load_op`, `.rasterizer_state`

## Referring to Functions

When referring to a function, write the name of the function, followed by a pair of parentheses.  
You may add the word "function" afterwards, if it fits neatly in the sentence.

Do not include the whole signature; just the parentheses is enough.

Examples:

- "in the `SDL_AppInit()` function"
- "With the `SDL_MapGPUTransferBuffer()` function"

## Markdown

### Newlines

Newlines above and below each heading.

### Linebreaks

Use two spaces for line breaks, not backslashes.

### Line length

Try to keep the line length reasonable.
Less than around 120 characters wide.

---

# Coding style

## Curly Braces

Opening curly braces on the next line.

## Naming

### Structs & Classes

Structs and Classes are start with a capital,
and words in them also start with a capital.

Examples: `struct Vertex{}`, `struct MyAppState{}`

### Variables

Variable names start with lowercase,
and words in them start with a capital.

Examples: `window`, `vertexBuffer`

### Functions

Functions start with a capital,
and words in them also start with a capital.

Examples: `LoadShader()`, `CreatePipeline()`

## Spaces

There should be spaces between `if`, `else`, `switch` and their parentheses.

## Indentation

Indentation will be done using tabs.  
Where alignment is desired, spaces will follow after the tabs.

Example:

```c++
void SomeFunc()
{
	if (Thing() &&
	    OtherThing())
	{}
}
```

## Pointers

With pointers, the `*` goes on the left, with the type.

This is controversial, but I personally find that
considering the pointer being part of the type is simpler to understand.

I am aware this could cause issues when defining multiple variables on the same line,
so simply do not do this.

However, for very primitive types in data structs, it is allowed.

For example, this is okay:

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
