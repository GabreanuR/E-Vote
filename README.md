# E-Vote – Electronic Voting System

**E-Vote** is a C++ terminal-based application that simulates a secure and extensible electronic voting system. Designed for educational and demonstrative purposes, this project showcases core Object-Oriented Programming principles, exception handling, STL usage, templates, and design patterns, all wrapped in a clean CLI interface.

---

## Features

### Voter Mode
- Login with ID and password
- View open elections available to your voter type (Student, Professional, etc.)
- Cast votes (randomly generated in demo mode)
- View previous results and your own public votes
- Change your vote if the election is still active

### Admin Mode
- Create new elections (e.g., Referendum, Local)
- Start or stop an active election
- Add or remove voters from the registry
- View results of all elections

### Simulation
- Voters are preloaded from a `.txt` file
- Voting is randomly simulated to showcase election logic
- Terminal menu-driven interface
- Support for public/private vote visibility and dynamic election behavior

---

# PROJECT INSTRUCTONS GIVEN IN ASSINGMENT:

| Teaching Assistent | Dragoș Bahrim                           |
|--------------------|-----------------------------------------|
| Link template      | https://github.com/Ionnier/oop-template |

## Compiling Instructions

The project is configuered with Cmake.

Terminal instructions:

1. The configuration step
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# or ./scripts/cmake.sh configure
```

Or on Windows with GCC:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# or ./scripts/cmake.sh configure -g Ninja
```

At this step we can ask to generate project files for various work environments.

## Mandatory requirements

Failure to comply will result in the project being disqualified.

  - the program will be written in C++
  - the program will have an interactive menu (just to illustrate the functionality)
  - the program has no compilation errors
  - no global variables
  - private (or protected) member data
  - GitHub Actions passed
  - appropriate and punctual Git commits
  - meaningless use of language features
  - use of language features with the aim of "legally" violating another rule
      - excessive use of friend classes
      - excessive use of static elements
  - each requirement will be scored 1/2 on implementation + 1/2 on the fact that it was implemented meaningfully

## Requirements:
- [ ] defining at least **2-3 class hierarchies** that interact within the chosen theme (either through composition, aggregation or just calling each other's methods in a logical way) (6p)
  - minimum one class with:
    - [ ] initialization constructors [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] overloaded constructor [*](https://github.com/Ionnier/poo/tree/main/labs/L02#supra%C3%AEnc%C4%83rcarea-func%C8%9Biilor)
    - [ ] copy constructors [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] copy `operator=` [*](https://github.com/Ionnier/poo/tree/main/labs/L02#supra%C3%AEnc%C4%83rcarea-operatorilor)
    - [ ] destructor [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] display `operator<<` (std::ostream) [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L123)
    - [ ] reading `operator>>` (std::istream) [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L128)
    - [ ] another overloaded operator as a member function [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L32)
    - [ ] another overloaded operator as a non-member function [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L39) - not necessarily as a friend
  - in derived
      - [ ] implementing the chosen functionalities through [upcast](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding) and [downcast](https://github.com/Ionnier/poo/tree/main/labs/L04#smarter-downcast-dynamic-cast)
        - this will be done through **2-3** methods specific to the chosen theme
        - functions for reading/displaying or the destructor are not included although you will have to implement them 
      - [ ] calling the base class constructor from [constructors from derived classes](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-la-derivare)
      - [ ] overwritten [cc](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-de-copiere-la-derivare)/op= for correct copying/attributions
      - [ ] destructor [virtual](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding)
  - for the other classes only what is needed will be defined
  - at least a more developed hierarchy (with 2-3 classes from a base class)
  - class hierarchy is also considered if there is only one base class but which does not inherit from a class in another hierarchy
- [ ] as many as possible `const` [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#reminder-const-everywhere)
- [ ] functions and attributes `static` (in classes) [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#static)
  - [ ] 1+ non-trivial static attributes
  - [ ] 1+ non-trivial static functions
- [ ] exceptions [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#exception-handling)
  - start from `std::exception`
  - illustrate exception propagation
  - illustrate upcasting in catch blocks
  - minimally used in a place where error handling in classic modes is more difficult
- [ ] using an abstract class [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#clase-abstracte)
 - [ ] template classes
   - [ ] creating a template class [(1p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [ ] 2 instantiations of this class (0.5p)
 - STL [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L07#stl)
   - [ ] using two different structures (containers) (vector, list or any other container that is more or less an array)
   - [ ] using a lambda function algorithm (e.g. sort, transform)
 - Design Patterns [(0.75p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [ ] using two design templates

### Observations

* There may be deductions of up to 2 points for various aspects such as:
  - memory leaks
  - not using virtual destructor when needed
  - abusing various concepts (all functions declared virtual)
  - calling virtual functions in constructors

* Generally, these are present in [CppCoreGuideline](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md), but you don't need to go through the document, just write the code organized enough.
* The `build/` and `install_dir/` folders are added to the `.gitignore` file because they contain generated files and do not help us version them.
