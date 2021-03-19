FROM ubuntu:focal

ENV MAP_URL=https://raw.githubusercontent.com/KevinLADLee/KevinLADLee.github.io/master/hongkong.osm.pbf
ENV DEBIAN_FRONTEND=noninteractive

ARG MIRROR='http://mirrors.aliyun.com'
RUN sed -i "s@http://.*archive.ubuntu.com@$MIRROR@g" /etc/apt/sources.list && \
    sed -i "s@http://.*security.ubuntu.com@$MIRROR@g" /etc/apt/sources.list

RUN apt-get update \
    && apt-get install --no-install-recommends -y build-essential sudo curl wget apt-transport-https ca-certificates locales git vim \
    && rm -rf /var/lib/apt/lists/* /tmp/*

RUN apt-get update \
    && apt-get install --no-install-recommends -y \
        python3-dev \
        python3-pip \
        libboost-all-dev \
        libxml2-dev \
        ffmpeg \
        cmake \
        libzip-dev \
        libtbb-dev \
        ccache \
        gdal-bin \
        libreadline-dev \
        libbz2-dev \
        pkg-config \
    && rm -rf /var/lib/apt/lists/* /tmp/* 

ADD . /opt/mod-abm-2.0

WORKDIR /opt

RUN mkdir osrm \
    && cd osrm \
    && curl -R -O http://www.lua.org/ftp/lua-5.3.6.tar.gz \
    && tar zxf lua-5.3.6.tar.gz \
    && cd lua-5.3.6 \
    && make linux test \
    && make install \
    && cd .. \
    && rm lua-5.3.6.tar.gz 

RUN if [ "x$(nproc)" = "x1" ] ; then export USE_PROC=1 ; else export USE_PROC=$(($(nproc)/2)) ; fi \
    && cd osrm \
    && git clone https://github.com/Project-OSRM/osrm-backend.git \
    && cd osrm-backend \
    && mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . -j${USE_PROC} \ 
    && cmake --build . --target install  

RUN cd osrm \
    && mkdir map \
    && cd map \
    && wget -qc $MAP_URL \
    && cd .. \
    && ./osrm-backend/build/osrm-extract ./map/hongkong.osm.pbf -p osrm-backend/profiles/car.lua \
    && ./osrm-backend/build/osrm-partition ./map/hongkong.osrm \
    && ./osrm-backend/build/osrm-customize ./map/hongkong.osrm 

RUN if [ "x$(nproc)" = "x1" ] ; then export USE_PROC=1 ; else export USE_PROC=$(($(nproc)/2)) ; fi && \
    cd /opt/mod-abm-2.0 \
    && cmake -S . -B build \
    && cmake --build build -j${USE_PROC}

RUN pip3 --no-cache-dir install pip --upgrade -i https://mirrors.aliyun.com/pypi/simple/ --trusted-host https://mirrors.aliyun.com \
    && pip3 config set global.index-url https://mirrors.aliyun.com/pypi/simple/ \
    && pip3 config set install.trusted-host mirrors.aliyun.com \
    && pip3 --no-cache-dir install setuptools wheel numpy jupyter ipython jupyterlab

EXPOSE 8888
EXPOSE 8889

ENV SHELL=/bin/bash

CMD ["bash", "-c", "jupyter lab --ip=0.0.0.0 --port=8889 --no-browser --allow-root"]
