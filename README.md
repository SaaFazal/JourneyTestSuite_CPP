# Journey Test Suite (C++ Boost UTF)

This project contains an automated unit test suite for the `durationBeforeTravellingBegins()` function from a GPS journey analysis application. It was developed as part of a university Software Engineering assignment at Nottingham Trent University.

## 🔍 Purpose

The goal was to validate and compare two developer implementations (`journey-devA`, `journey-devB`) of a function that calculates how long a journey remains stationary before actual travel begins.

## 🧪 Features

- ✅ 8 Boost UTF test cases covering:
  - Normal input
  - Edge and boundary conditions
  - Invalid arguments and exception handling
- 📦 Built using Qt and C++20
- 🔄 Git branches: `journey-testing`, `journey-devA`, `journey-devB`

## 📂 Structure

- `tests/journey-tests.cpp`: Unit test suite
- `src/journey.cpp`: Developer implementations
- `headers/journey.h`: Interface specifications
- `bin/`: Compiled outputs (ignored in Git)
- `data/`: GPX input files for journey simulation

## 🛠️ Build Instructions

Tested on Ubuntu 24.04 with:
- `qmake`
- `make`
- `Boost Unit Test Framework`

### To Build & Run:
```bash
make clean
qmake Journey-Tests.pro
make
./bin/journey-tests
```

## 📊 Results Summary

The test suite uncovered 5 bugs in `devA` and 6 bugs in `devB`. See the attached report for full analysis.

## 👤 Author

**Sahadh Fazal**
**work.sahadh@gmail.com"**
