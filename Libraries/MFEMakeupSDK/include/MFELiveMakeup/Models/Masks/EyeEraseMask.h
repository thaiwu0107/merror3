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

    class EyeEraseMask : public MakeupMask {
    public:
        EyeEraseMask();

        virtual ~EyeEraseMask();

        /**
         * Parameters
         */

        /**
         * How much to blur the mask. Default value is 1.0.
         */
        float blurFactor;

        /**
         * How much to expand the mask. A value > 1.0 means expand and a value < 1.0 means shrink.
         * Default value is 0.9.
         */
        float expandFactor;

        void updateWithTrackingData(const TrackingData &data) override;

        bool isStaticMask() const override;

        bool equals(const MakeupMask &other) const override;

        std::shared_ptr<MakeupMask> clone() const override;
    };

    typedef std::shared_ptr<EyeEraseMask> EyeEraseMaskPtr;

    /** @}*/
}

#pragma GCC visibility pop
