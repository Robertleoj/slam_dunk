ARG ARCH=x86_64
# 2020-10-10 Python 3.9.0
FROM quay.io/pypa/manylinux1_$ARCH

# Example build:
# docker build -t skhep/manylinuxgcc-x86_64 . -f Dockerfile --build-arg CORES=48 
# docker build -t skhep/manylinuxgcc-i686 . -f Dockerfile --build-arg CORES=48 --build-arg ARCH=i686

LABEL description="A docker image for building portable Python linux binary wheels using modern GCC"
LABEL maintainer="The SciKit-HEP Project"

RUN yum -y update \
    && yum -y install flex \
    && yum clean all

ARG ARCH
ARG CORES=4

# ARG GCC_VERSION=9.3.0
# ARG GCC_PATH=/usr/local/gcc-$GCC_VERSION
ARG GCC_VERSION=13.1.0
ARG GCC_PATH=/usr/local/gcc-13.1.0

RUN cd /tmp \
    && curl -L -o gcc.tar.gz "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz" \
    && tar xf gcc.tar.gz \
    && cd /tmp/gcc-$GCC_VERSION \
    && sed -i 's\ftp://gcc.gnu.org\https://gcc.gnu.org\g' contrib/download_prerequisites \
    && contrib/download_prerequisites \
    && mkdir build \
    && cd build \
    && ../configure -v \
        --build=$ARCH-linux-gnu \
        --host=$ARCH-linux-gnu \
        --target=$ARCH-linux-gnu \
        --prefix=/usr/local/gcc-$GCC_VERSION \
        --enable-checking=release \
        --enable-languages=c,c++,fortran \
        --disable-multilib \
        --program-suffix=-$GCC_VERSION \
    && make -j$CORES \
    && make install-strip \
    && cd /tmp \
    && rm -rf /tmp/gcc-$GCC_VERSION /tmp/gcc.tar.gz

ENV CFLAGS="-static-libstdc++" \
    CC=/usr/local/gcc-$GCC_VERSION/bin/gcc-$GCC_VERSION \
    CXX=/usr/local/gcc-$GCC_VERSION/bin/g++-$GCC_VERSION
