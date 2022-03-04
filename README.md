# lox-cpp

This is my implementation of the *first* interpreter described in the book [Crafting Interpreters](https://craftinginterpreters.com/index.html) by Robert Nystrom.

## Lox

"Lox" is a small object oriented language that looks like Javascript. You can check some [examples in the book](https://craftinginterpreters.com/the-lox-language.html).

## Implementation

The book implements the first interpreter (jlox) in Java, but I wanted to practice C++ a little bit more and decided to give it a go. It was a big challenge though, and I had to cut some corners and hack my way out of a few complicated issues (memory management, pointers, error handling, etc).

In the end, I realized I was struggling with C++ too much and I wanted to restart in something like C#, and gave it up for a while but since I was already dealing with Classes (the second to last chapter from part II), I decided to give it one more try and finish it, even though my implementation would end up having bugs and memory leaks. 😕

I might decide going back to it later and try using smart pointers and other Modern C++ features - that I should probably be using from the beginning, anyway. 🤔

Or, I should probably just finish the second part of the book that implements the second interpreter (clox) in plain C with the more interesting stuff. 😄

## Structure

A short description for the main files:

```
lox-cpp
│   
│   lox-test-runner.sh         A simple bash script to run some tests against the interpreter
│   lox-cpp.cpp                Main entry point
│   Lox.[cpp,h]                Main runtime
│   Token.[cpp.h]              Tokens generated by scanner
│   Scanner.[cpp,h]            Lexycal analyzer (splits input into tokens for the parser)
│   Parser.[cpp,h]             Parses tokens into Abstract Syntax Tree (AST)
│   Resolver.[cpp,h]           Resolves names references for interpreter
│   Interpreter.[cpp,h]        Main tree interpreter
│   Expr.[cpp,h]               AST - Classes for expressions
│   Stmt.[cpp.h]               AST - Classes for statements
│   List*.[cpp,h]              AST - Helper classes
│   Object.[cpp,h]             Basic Lox value
│   Lox*.[cpp,h]               Classes to represent Lox values like classes, functions, etc
│   NativeFunctions.h          Native function implementations callable from Lox code
│
├───lox-generate-ast           Utility to generate the AST classes for expressions and statements
│
└───lox-ast-printer            Test program to print the tree representation for the AST
```

Modules:

```
                                   ┌──────────┐
                                   │ Resolver ├──► Variable bindings
                                   └──────────┘          │
                                         ▲               ▼
┌─────────┐            ┌────────┐        │        ┌─────────────┐
│ Scanner ├─► Tokens ─►│ Parser ├─────► AST ─────►│ Interpreter │
└─────────┘            └────────┘                 └─────────────┘
```

## Building

The project/solution was created in Visual Studio Community 2017 but there should be no dependencies on it, or on Windows, and it should compile in other platforms and compilers with some adjustments.

## Testing

On a bash prompt, in the project root, run:

```
./lox-test-runner.sh
```

Or, if making changes, leave the test runner in *watch* mode:

```
./lox-test-runner.sh -w
```

## Usage

Run in interactively (currently, no support for multiline statements in REPL mode):

```
lox-cpp
```

Pass a script as an argument:

```
lox-cpp program.lox
```

From standard input:

```
lox-cpp < program.lox

-- or --

cat program.lox | lox-cpp
```