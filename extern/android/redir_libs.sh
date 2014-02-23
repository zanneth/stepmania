#! /bin/bash

rm -rf jniLibs;

for i in armeabi x86
do
    mkdir -p jniLibs/$i; echo "ARM structure done"
    cp ffmpeg_output/$i/lib/lib*-*.so jniLibs/$i; echo "$i FFMPEG relibbing done"
done

#mkdir -p jniLibs/armeabi; echo "ARM structure done"
#cp ffmpeg/android/arm/lib/lib*-*.so jniLibs/armeabi; echo "ARM relibbing done"
#cp libs/armeabi/libstepstart.so jniLibs/armeabi

#mkdir -p jniLibs/x86 ; echo "x86 structure done"
#cp ffmpeg/android/x*/lib/lib*-*.so jniLibs/x86; echo "x86 relibbing done"
#cp libs/x86/libstepstart.so jniLibs/x86
