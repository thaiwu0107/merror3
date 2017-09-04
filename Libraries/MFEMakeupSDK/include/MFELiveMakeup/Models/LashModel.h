//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <string>
#include "DynamicColorParameters.h"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup Models
     *  @{
     */


    /**
     * A lash model defines the appearance of false lashes and is defined with respect to the left lash.
     */
    class LashModel {
    public:
        /**
         * The lash RGB color (0 - 255).
         */
        int colorR;
        int colorG;
        int colorB;

        /**
         * Whether or not we should treat the image as a mask and recolor or if we should render the image directly as the lash
         */
        bool shouldRecolor;

        /**
         * The alpha of the lash (0.0 - 1.0).
         */
        float intensity;

        /**
         * The x value at which the contour begins in the lash image. Ranges from 0 to width of the lash image.
         */
        int minX;

        /**
         * The x value at which the contour ends in the lash image. Ranges from 0 to width of the lash image.
         */
        int maxX;

        /**
         * The minimum pixel alpha value at which a pixel is considered part of the contour. Ranges from 0 to 255.
         */
        int contourThreshold;

        /**
         * Whether this is the left or right lash
         * Left means "left on screen" which is the lash for the person's right eye
         */
        bool isLeftLash;

        /**
         * Whether this is the upper or lower lash.
         */
        bool isUpperLash;

        /**
         Parameters used for dynamic coloring. The default values should suffice for most applications.
         */
        DynamicColorParameters dynamicColorParameters;
        
        /**
         Whether or not enable dynamicColor, default is true
         */
        bool useDynamicColor;
        
        /**
         * Scales the lashes vertically. Useful for lengthening the lashes.
         */
        float verticalScalingFactor;

        /**
         * Absolute path to the lash image.
         */
        std::string imagePath;

        LashModel();

        // Equality operators
        bool operator==(const LashModel &rhs) const;

        bool operator!=(const LashModel &rhs) const;
    };

    /** @}*/
}

#pragma GCC visibility pop
