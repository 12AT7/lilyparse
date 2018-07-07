FROM debian:buster-slim
USER root
RUN apt-get update && apt-get install -y --no-install-recommends git g++ clang-6.0 cmake ninja-build curl lcov clang-tidy clang-format libboost-program-options-dev libboost-filesystem-dev libboost-iostreams-dev python3 python3-pip python3-setuptools
RUN apt-get install -y ca-certificates
RUN pip3 install pytest better-bencode
