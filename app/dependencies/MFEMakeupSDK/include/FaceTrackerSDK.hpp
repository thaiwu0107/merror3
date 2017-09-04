//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <MFELiveCommon.h>

#include "MFEMakeupSDK.hpp"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup MFEMakeupSDK
     *  @{
     */

    /**
     * Class that provides face tracking.
     */
    class FaceTrackerSDK {
    public:
        /**
         * Constructor.
         *
         * MFEMakeupSDK_init must be called before this constructor.
         */
        FaceTrackerSDK();

        virtual ~FaceTrackerSDK();

        /**
         Defines the available tracker data sets
         */
        enum class ResourceSet {
            //! Provides a smaller size at the cost of reduced accuracy (about 5 MB)
            Small,
            
            //! Provides the full version which is more accurate but larger in size (about 30 MB)
            Full
        };
        
        /**
         Loads the face tracking data. This is a slow operation so it should not be called on the main thread.
         @param bundePath Absolute path to the face tracker bundle.
         @param resourceSet Which resource set to use
         */
        void loadResources(std::string bundlePath, ResourceSet resourceSet);

        /**
         Detect and track face in the current frame. The resources must be loaded before this is called.
         
         Important: if you already have a grayscale (1 channel) version of the frame, pass that in instead
         of the color image as this will improve performance. Internally, the face tracker will convert color images
         to grayscale, but will skip this conversion if the input image is already grayscale.
         
         @param frame Image frame to detect. This can be a color (3 or 4 channel) or grayscale (1 channel) image.
         @param points The returned face points (only valid if face was detected)
         @return true if face was detected
         */
        bool track(const cv::Mat &frame, BaseFacePoints &points);

        /**
         Resets the current face tracker (but keeps the loaded face tracker data).
         This causes it to discard all internal tracking state and attempt a full redetection from scratch on the next call of track.
         
         Important: if you are using the face tracker on unrelated still images (e.g. not consecutive frames from a video),
         then you should call reset before calling track on every new image so that it does not try to track faces across unrelated
         images.
         */
        void reset();

    private:
        class MFEFaceTrackerSDKImpl;

        MFEFaceTrackerSDKImpl *impl;
    };

    /** @}*/
}

#pragma GCC visibility pop
