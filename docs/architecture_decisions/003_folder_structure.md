# 2. Folder structure

Date: 2025-03-06

## Status

Accepted

## Context

The project consists of multiple libraries / executables.

## Decision

- Each lib/executable should have its own folder. Developers should be able to distinguish between the different libs/executables.
- Each lib/executable should have a src folder containing both cpp and header files.
- Libs should have another folder with the name of the lib in the src directory. This is done so the include statement will start with the library name.
- The package manager, documentation, common cmake files should be at the root of the project so it can be used by all libs/executables.
- Unit tests should be next to the relevant source code in a folder called "tests".
- Integration tests, black box tests, etc. should be in a folder called tests outside of the src folder.

Example:

- lib
  - src
    - lib
      - foo.h
      - foo.cpp
      - tests
        - foo_tests.cpp
  - tests
    - integration-test
- exe
  - src
    - bar.h
    - bar.cpp
  - tests
    - integration-test

## Consequences

We will have a clean folder structure with multiple libs/executables that is easy to scan and understand.
It will also be clear what libraries are included in the source files and where they come from.
