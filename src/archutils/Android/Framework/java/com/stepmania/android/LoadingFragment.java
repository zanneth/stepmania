package com.stepmania.android;

import android.app.DialogFragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;

/**
 * Created by cybik on 3/30/14.
 */
public class LoadingFragment extends DialogFragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = View.inflate(
            getActivity().getApplicationContext(), R.layout.loadingdialog, null
        );
        // yay for JNI :/
        mResource = (TextView)view.findViewById(R.id.tvResource);
        mProgressBar = (ProgressBar)view.findViewById(R.id.pbLoading);

        mProgressBar.setIndeterminate(mIndeterminate);
        if(mtotalwork != 1) {
            mProgressBar.setMax(mtotalwork);
        }
        if(mprogress != 0) {
            mProgressBar.setProgress(mprogress);
        }

        if(getDialog() != null) {
            getDialog().setTitle(getString(R.string.app_name));
        }
        return view;
    }

    public void SetText(final String text) {
        if(mResource != null) {
            if(getActivity() != null) {
                getActivity().runOnUiThread(
                    new Runnable() {
                        @Override
                        public void run() {
                            mResource.setText(text);
                        }
                    }
                );
            }
        }
        else
            mtext = text;
    }
    public void SetProgress(final int progress) {
        if(mProgressBar != null){
            if(getActivity() != null) {
                getActivity().runOnUiThread(
                    new Runnable() {
                        @Override
                        public void run() {
                            mProgressBar.setProgress(progress);
                        }
                    }
                );
            }
        }
        else
            mprogress = progress;
    }
    public void SetTotalWork(final int total) {
        if(mProgressBar != null){
            if(getActivity() != null) {
                getActivity().runOnUiThread(
                    new Runnable() {
                        @Override
                        public void run() {
                            mProgressBar.setMax(total);
                        }
                    }
                );
            }
        }
        else
            mtotalwork = total;
    }
    public void SetIndeterminateState(final boolean isIndeterminate) {
        if(mProgressBar != null){
            if(getActivity() != null) {
                getActivity().runOnUiThread(
                    new Runnable() {
                        @Override
                        public void run() {
                            mProgressBar.setIndeterminate(isIndeterminate);
                        }
                    }
                );
            }
        }
        else
            mIndeterminate = isIndeterminate;
    }

    boolean mIndeterminate = false;
    int mtotalwork = 1;
    int mprogress = 0;
    String mtext = "";

    private TextView mResource = null;
    private ProgressBar mProgressBar = null;
}
