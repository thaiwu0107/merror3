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

    class FoundationMask : public MakeupMask {
    public:
        FoundationMask();

        virtual ~FoundationMask();

        /**
         * Parameters
         */

        /**
         * How much to blur the mask. Default value is 1.0.
         */
        float blurFactor;

        /**
         * Modifies the intensity of mask. Default value is 1.0;
         */
        float intensityModifier;

        /**
         * Whether to erase the top of the nose. Default value is true.
         */
        bool eraseTopNose;

        /**
         * Whether to erase the nose. Default value is true.
         */
        bool eraseNose;

        /**
         * Whether to erase the brow region. Default value is true.
         */
        bool eraseBrow;

        /**
         * Whether to erase the region between the eyes and brows. Default value is true.
         */
        bool eraseTopEyes;

        /**
         * Whether to erase the the eyes. Default value is true.
         */
        bool eraseEyes;

        /**
         * Whether to erase the mouth. Default value is true.
         */
        bool eraseMouth;

        /**
         * Whether to erase the nostril area. Default value is true.
         */
        bool eraseNostril;

        /**
         * How much to expand the mask at the top. Default value is 1.0.
         */
        float expandFactorTop;

        /**
         * How much to expand the mask at the bottom. Default value is 1.0.
         */
        float expandFactorBottom;

        void updateWithTrackingData(const TrackingData &data) override;

        bool isStaticMask() const override;

        bool equals(const MakeupMask &other) const override;

        std::shared_ptr<MakeupMask> clone() const override;
    };

    typedef std::shared_ptr<FoundationMask> FoundationMaskPtr;

    /** @}*/
}

#pragma GCC visibility pop
