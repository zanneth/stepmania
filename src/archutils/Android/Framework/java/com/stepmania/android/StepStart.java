package com.stepmania.android;

import android.app.Fragment;
import android.app.FragmentTransaction;
import android.app.NativeActivity;
import android.content.Context;
import android.media.AudioManager;

public class StepStart extends NativeActivity {

    // libload forcing; not loading FFMPEG libs before stepmania leads to a refusal to load.
    static {
        // FFMpeg
        System.loadLibrary("avcodec-55");
        System.loadLibrary("avfilter-4");
        System.loadLibrary("avformat-55");
        System.loadLibrary("avutil-52");
        System.loadLibrary("swresample-0");
        System.loadLibrary("swscale-2");
    }

    public int getNativeSampleRate() {
        AudioManager am = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        String sampleRate = am.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);

        if(!sampleRate.isEmpty())
            return Integer.valueOf(sampleRate);

        return -1;
    }
    public int getNativeFramesPerBuffer() {
        AudioManager am = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        String framesPerBuffer = am.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);

        if(!framesPerBuffer.isEmpty())
            return Integer.valueOf(framesPerBuffer);

        return -1;
    }
    public void crash(String reason) {
        throw new RuntimeException(reason);
    }

    // Le LoadingWindow
    LoadingFragment mStepmaniaLoadingWindow = null;
    LoadingFragment StepmaniaLoadingWindow() {
        if(mStepmaniaLoadingWindow == null)
            mStepmaniaLoadingWindow = new LoadingFragment();
        return mStepmaniaLoadingWindow;
    }
    private String fragid = "StLoadingWindow";
    public void fragInitializeLoadingWindowDialog() {
        FragmentTransaction ft = getFragmentManager().beginTransaction();
        Fragment previous=getFragmentManager().findFragmentByTag(fragid);
        StepmaniaLoadingWindow().show(ft,fragid);
    }
    public void fragTeardownLoadingWindowDialog() {
        StepmaniaLoadingWindow().dismiss();
        mStepmaniaLoadingWindow = null;
    }
    public void fragSetText(String text) {
        StepmaniaLoadingWindow().SetText(text);
    }
    public void fragSetProgress(int progress) {
        StepmaniaLoadingWindow().SetProgress(progress);
    }
    public void fragSetTotalWork(int tw) {
        StepmaniaLoadingWindow().SetTotalWork(tw);
    }
    public void fragSetIndeterminate(boolean indet) {
        StepmaniaLoadingWindow().SetIndeterminateState(indet);
    }

}
