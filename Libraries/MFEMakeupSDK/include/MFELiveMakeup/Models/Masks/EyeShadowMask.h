//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include "MakeupMask.h"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup Models
     *  @{
     */

    class EyeShadowMask : public MakeupMask {
    public:
        EyeShadowMask();

        virtual ~EyeShadowMask();

        /**
         * Parameters
         */

        /**
         * Whether this is to be used as a erase mask. The intensity is calculated differently
         * depending on the value. Default value is false.
         */
        bool isSubtractMask;

        /**
         * Offset in the X direction (a positive X moves the mask outwards towards the edge of the face).
         * Default value is 0.0.
         */
        float offsetX;

        /**
         * Offset in the Y direction (a positive Y moves the mask downwards).
         * Default value is 0.0.
         */
        float offsetY;

        /**
         * Scaling in the X direction. Default value is 1.0.
         */
        float scaleX;

        /**
         * Scaling in the Y direction. Default value is 1.0.
         */
        float scaleY;

        /**
         * If the angle is steep, the mask will be not be shown by setting its intensity to zero.
         * Default value is true.
         */
        bool disableMaskOnSteepAngles;

        void updateWithTrackingData(const TrackingData &data) override;

        bool isStaticMask() const override;

        bool equals(const MakeupMask &other) const override;

        std::shared_ptr<MakeupMask> clone() const override;
    };

    typedef std::shared_ptr<EyeShadowMask> EyeShadowMaskPtr;

    /** @}*/
}

#pragma GCC visibility pop
