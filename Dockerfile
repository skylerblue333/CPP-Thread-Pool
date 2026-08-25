FROM gcc:15 AS builder
RUN apt-get update \
    && apt-get install -y --no-install-recommends cmake \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY CMakeLists.txt ./
COPY include ./include
COPY src ./src
COPY tests ./tests
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF && cmake --build build --parallel 2

FROM debian:bookworm-slim
RUN useradd --system --uid 10001 --no-create-home sky
WORKDIR /app
COPY --from=builder /src/build/sky-thread-pool-demo /app/sky-thread-pool-demo
USER 10001:10001
ENTRYPOINT ["/app/sky-thread-pool-demo"]
