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

    class LipstickMask : public MakeupMask {
    public:
        LipstickMask();

        virtual ~LipstickMask();

        enum class Style {
            Full,
            TwoToneInner,
            TwoToneOuter
        };

        /**
         * Parameters
         */

        /**
         * How much to blur the mask. Default value is 1.0.
         */
        float blurFactor;

        /**
         How much to blur inner part of the mask. Default value is 1.0.
         */
        float blurFactorInner;

        /**
         How much to expand the inner mouth region when rendering. The inner mouth region is the region
         that is not colored when the mouth is open. If you find that the coloring is going too much into the
         mouth, this value can be increased.
         Default value is 1.0.
         */
        float innerMouthExpandFactor;

        /**
         How much to shrink the outer lip points towards the inner lip points.
         A value of 1.0 will make it shrink to the same size as the inner lip points.
         Default value is 0.0.
         */
        float outerLipPointsShrinkFactor;
        
        /**
         * The lipstick style. Default value is `Style::Full`.
         */
        Style style;

        void updateWithTrackingData(const TrackingData &data) override;

        bool isStaticMask() const override;

        bool equals(const MakeupMask &other) const override;

        std::shared_ptr<MakeupMask> clone() const override;

    };

    typedef std::shared_ptr<LipstickMask> LipstickMaskPtr;

    /** @}*/
}

#pragma GCC visibility pop
