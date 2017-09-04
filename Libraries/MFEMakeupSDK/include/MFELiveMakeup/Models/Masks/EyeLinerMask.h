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

    class EyeLinerMask : public MakeupMask {
    public:
        EyeLinerMask();

        enum class LinerStyle {
            Natural = 1,
            CatEye,
            PuppyEye,
            Smoky,
            Top,
            TopPointy,
            Full,
            FullPointy,
            Bottom,
            TopStrong,
            InnerCorner,
            CatEyeChanel,
        };

        /**
         * Parameters
         */

        /**
         * The liner style. Default value is `LinerStyle::Natural`.
         */
        LinerStyle linerStyle;

        void updateWithTrackingData(const TrackingData &data) override;

        bool isStaticMask() const override;

        bool equals(const MakeupMask &other) const override;

        std::shared_ptr<MakeupMask> clone() const override;
    };

    typedef std::shared_ptr<EyeLinerMask> EyeLinerMaskPtr;

    /** @}*/
}

#pragma GCC visibility pop
