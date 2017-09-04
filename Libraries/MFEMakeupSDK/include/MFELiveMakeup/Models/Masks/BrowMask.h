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

    class BrowMask : public MakeupMask {
    public:
        BrowMask();

        virtual ~BrowMask();

        /**
         * Parameters
         */

        /**
         * How much to blur the mask. Default value is 1.0.
         */
        float blurFactor;

        /**
         * How thick the brow should be. Default value is 1.0.
         */
        float thicknessFactor;

        void updateWithTrackingData(const TrackingData &data) override;

        bool isStaticMask() const override;

        bool equals(const MakeupMask &other) const override;

        std::shared_ptr<MakeupMask> clone() const override;
    };

    typedef std::shared_ptr<BrowMask> BrowMaskPtr;

    /** @}*/
}

#pragma GCC visibility pop
