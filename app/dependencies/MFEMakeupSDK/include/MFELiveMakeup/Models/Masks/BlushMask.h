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

    class BlushMask : public MakeupMask {
    public:
        BlushMask();

        virtual ~BlushMask();

        /**
         *Parameters
         */

        /**
         * Whether this is to be used as a erase mask. The intensity is calculated differently
         * depending on the value. Default value is false.
         */
        bool isSubtractMask;

        /**
         * Scaling in the X direction. Default value is 1.0.
         */
        float scaleX;

        /**
         * Scaling in the Y direction. Default value is 1.0.
         */
        float scaleY;

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

        void updateWithTrackingData(const TrackingData &data) override;

        bool isStaticMask() const override;

        bool equals(const MakeupMask &other) const override;

        std::shared_ptr<MakeupMask> clone() const override;
    };

    typedef std::shared_ptr<BlushMask> BlushMaskPtr;

    /** @}*/
}

#pragma GCC visibility pop
