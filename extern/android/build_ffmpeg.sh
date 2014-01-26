#! /bin/bash

function build_one
{
cd ffmpeg
make distclean
./configure \
    --arch=${ARCH} \
    --cpu=${CPU} \
    --prefix=${PREFIX} \
    --enable-cross-compile \
    --enable-shared \
    --disable-static \
    --disable-doc \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-ffserver \
    --disable-avdevice \
    --disable-doc \
    --disable-symver \
    --cross-prefix=${CROSS_COMPILE_PREFIX} \
    --target-os=linux \
    --sysroot=$SYSROOT \
    --extra-cflags="-Os -fpic $ADDI_CFLAGS" \
    --extra-ldflags="$ADDI_LDFLAGS" \
    ${ADDITIONAL_CONFIGURE_FLAG}
make -j16
make install
}

function usage
{
echo "Usage: $0 [arch] [android platform]"
echo "Arch: armeabi, x86"
echo "Android Platforms: 9 through 19, as digits"
}

if [ "$1" == "" ]; then
  usage
  exit 0
fi

if [ "$1" != "" ]; then
 SETARCH=$1
fi

if [ "$2" != "" ]; then
 SETPLATFORM=$2
fi

if [ "$3" != "" ]; then
 NDK=$3
else
 NDK=/opt/android-ndk
fi

if [ "$SETARCH" == "armv7-a" ]; then
 SETPREFIX="armeabi"
 CPU="armv7-a"
 ARCH="arm"

 ADDI_CFLAGS="-m${ARCH}"
 TOOLCHAIN=${NDK}/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64
 CROSS_COMPILE_PREFIX="${TOOLCHAIN}/bin/arm-linux-androideabi-"
 # If we want to troll the Tegra2 guys, we can...
 # ADDITIONAL_CONFIGURE_FLAG="--enable-neon"
elif [ "$SETARCH" == "x86" ]; then
 SETPREFIX="x86"
 CPU="i686" # not supporting x86_64 in android just yet.
 ARCH=${SETPREFIX}

 ADDI_CFLAGS="" # nothing yet.
 TOOLCHAIN=${NDK}/toolchains/x86-4.8/prebuilt/linux-x86_64
 CROSS_COMPILE_PREFIX="${TOOLCHAIN}/bin/i686-linux-android-"
 ADDITIONAL_CONFIGURE_FLAG="--disable-asm"
fi

PREFIX=$(pwd)/ffmpeg_output/${SETPREFIX}
SYSROOT=${NDK}/platforms/android-${SETPLATFORM}/arch-${ARCH}/

build_one
