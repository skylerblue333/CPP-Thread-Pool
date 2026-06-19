FROM gcc:13
WORKDIR /app
COPY . .
RUN g++ -std=c++17 -o app src/main.cpp
CMD ["./app"]
