FROM debian:buster-slim
USER root
RUN apt-get update && apt-get install -y --no-install-recommends git g++ clang-6.0 cmake ninja-build curl lcov libboost-program-options-dev libboost-filesystem-dev libboost-iostreams-dev
RUN apt-get install -y ca-certificates
