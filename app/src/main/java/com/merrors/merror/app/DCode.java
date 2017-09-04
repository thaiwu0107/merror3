package com.merrors.merror.app;

import android.support.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * Created by ggttoo44 on 2017/8/23.
 */

public interface DCode {

    interface FlagFragments{
        int Flag_HomeFragment = 0x00001;
        String TAG_HomeContextFragment = "TAG_HomeFragment";
        @Retention(RetentionPolicy.SOURCE)
        @IntDef({Flag_HomeFragment})
        @interface Flag_Fragmnet{}
    }

    /**
     Specifies the tracker and makeup assets bundle to use
     */
    interface AssetsConfig {
        // If set to true this will use the full face tracker resources, otherwise it will use the smaller version
        boolean kUseFullFaceTrackerResources = true;
        String TrackerAssetDir = kUseFullFaceTrackerResources? "MFEFaceTrackerBundleFull" : "MFEFaceTrackerBundleSmall";
        String TrackerSaveDir = kUseFullFaceTrackerResources? "MFEFaceTrackerBundleFull" : "MFEFaceTrackerBundleSmall";
        String MFEBundleAssetDir = "MFELiveMakeupBundle";
        String MFEBundleSaveDir = "MFELiveMakeupBundle";
    }
}
