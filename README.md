# libfoxochat
Libfoxochat is a user-friendly c++ lib to comfortably work with foxochat api.

# Build

```
git clone https://github.com/Foxocorp/libfoxochat
cd libfoxochat
mkdir cmake-build
cd cmake-build 
cmake ..
```

# Dependency

## Build requirements
[cmake](https://cmake.org/) (3.13+)

C++ 17 or newer standard (clang/g++/clang-cl/MSVC)

## External Dependencies 
[OpenSSL](https://openssl.org/) For HTTPS and WSS

## Optional Dependencies
Zlib (optional) to compress data

## Included Dependencies
[JSON](https://json.nlohmann.me/) (Getting and building with project)
[IXWebSocket](https://machinezone.github.io/IXWebSocket/) to make http requests and connect to websockets
