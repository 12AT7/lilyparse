FROM debian:buster-slim
USER root
RUN apt-get update && apt-get install -y --no-install-recommends git g++ clang-6.0 cmake ninja-build curl lcov clang-tidy clang-format libboost-program-options-dev libboost-filesystem-dev libboost-iostreams-dev python3 python3-pip python3-setuptools python3-lxml 
RUN apt-get install -y ca-certificates
RUN pip3 install pytest better-bencode
RUN mkdir -p /usr/share/man/man1
RUN apt-get install -y --no-install-recommends openjdk-8-jre-headless wget
RUN wget -O allure2.tar.gz https://bintray.com/qameta/generic/download_file?file_path=io%2Fqameta%2Fallure%2Fallure%2F2.6.0%2Fallure-2.6.0.tgz
RUN tar xzf allure2.tar.gz
RUN PATH="/allure-2.6.0/bin:$PATH"
