FROM arm32v7/debian

RUN apt-get update
RUN apt-get install -y \
    build-essential \
    cmake \
    libasound2-dev \
    libbz2-dev \
    libgl-dev \
    libglew-dev \
    libjpeg-dev \
    libmad0-dev \
    libmp3lame-dev \
    libogg-dev \
    libpulse-dev \
    libvorbis-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxtst-dev \
    nasm \
    pkg-config \
    yasm \
    zlib1g-dev

