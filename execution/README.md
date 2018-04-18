# Execution Engine

Sub-directory containg relevant files for the Execution Engine.

## CMakeLists.txt

This is the cmake build list for the execution sub-project.

## examples/

These are some example illustrating the usage of some of our standard tools.
This directory is most likely temporary.

## includes/

These are all headers needed to build the execution team's code.

### includes/execution/

These are all headers written by the execution team.

#### includes/execution/interfaces

Our pure virtual classes or interfaces.

#### includes/execution/impl

Implementation headers, which most likely inherit from an interface.

#### includes/execution/tests

Headers that should only be included from test sources.

## src/

All of our source files required for building the execution library.

- Anything ending in \*\_test.cc is a test source and does not contribute
  to production builds.
