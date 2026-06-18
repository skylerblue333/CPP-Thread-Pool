# CPP-Thread-Pool

## Overview
A production-grade C++ thread pool using modern C++17 features including `std::future`, `std::packaged_task`, and `std::condition_variable`.

## Quick Start (1-Click Build)

```bash
git clone https://github.com/skylerblue333/CPP-Thread-Pool.git
cd CPP-Thread-Pool
mkdir build && cd build
cmake .. && make
./thread_pool
```

## Features
- C++17 standard
- Template-based task submission with futures
- Graceful shutdown
