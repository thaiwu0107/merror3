//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include "BaseFacePoints.hpp"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup MFELiveCommon_Models
     *  @{
     */

    /**
     * Container class of an image with face data of the face in the image.
     */
    class TrackingData {
    public:
        BaseFacePoints facePoints; ///< The face points, which is only valid if hasFacePoints is true
        bool hasFacePoints; ///< Whether the facePoints is valid
        
        cv::Mat image; ///< A 3 or 4 channel RGB(A) or BGR(A) image
        bool isBGR; ///< If true, image is a BGR(A) image, otherwise a RGB(A) image
        
        unsigned int frameID; ///< An unique ID > 0 is assigned to every new frame. This is used for performance optimizations.
        
        TrackingData() : hasFacePoints(false), isBGR(false), frameID(0) {}
    };

    /** @}*/
}

#pragma GCC visibility pop
