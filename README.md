# Sky Thread Pool

A bounded C++20 thread-pool primitive for local concurrent workloads in the SKYCOIN4444 engineering portfolio.

## Implemented

- Fixed worker count with constructor validation.
- Bounded pending-task queue with producer backpressure.
- `submit` returns `std::future` and propagates task exceptions.
- Deterministic `wait_idle()` and draining shutdown behavior.
- Copy/move disabled to make ownership explicit.
- CMake interface target for reuse plus a small executable demonstration.
- Deterministic tests covering execution, exception propagation, queue draining, and invalid configuration.
- Release build, warnings-as-errors, tests, ASan/UBSan verification, container build, non-root check, and container smoke test in GitHub Actions.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
./build/sky-thread-pool-demo
```

## Product boundary

Status: **engineering beta**.

This repository is a process-local concurrency library. It does not claim distributed job execution, durable queues, work stealing, CPU affinity management, cancellation tokens, priority scheduling, real-time guarantees, production deployment, or benchmarked capacity. Those capabilities require separate implementation and evidence.

## SKYCOIN4444 integration role

Use this component inside native workers or compute-oriented services that need bounded local concurrency. Network queues, durable orchestration, and cross-node scheduling belong in separate product boundaries.

## License

See `LICENSE`.
