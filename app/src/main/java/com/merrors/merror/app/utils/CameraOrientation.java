package com.merrors.merror.app.utils;


/**
 * Camera orientation enum
 */
public enum CameraOrientation {
    ROTATE0_NOFLIP(0, false),
    ROTATE0_FLIP(1, false),
    ROTATE90_NOFLIP(2, true),
    ROTATE90_FLIP(3, true),
    ROTATE180_NOFLIP(4, false),
    ROTATE180_FLIP(5, false),
    ROTATE270_NOFLIP(6, true),
    ROTATE270_FLIP(7, true);

    private final int mId;
    private final boolean mSwapSize;

    CameraOrientation(int id, boolean swap){
        mId = id;
        mSwapSize = swap;
    }

    public int getId(){
        return mId;
    }

    public boolean swapSize(){
        return mSwapSize;
    }
}