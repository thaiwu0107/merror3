//
//  MFELiveMakeup
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#pragma GCC visibility push(default)

namespace MFE {
    class DynamicColorParameters {
    public:
        DynamicColorParameters() : productValueBalancing(0.5), darkValueThreshold(0.5), saturationBalancing(0.9), saturationAdjustment(true) {};
        
        /**
         product's v value balancing (The higher the v, the lower the real v).
         Range: (0.0, 1.0).
         Default is 0.5.
         */
        float productValueBalancing;
        
        /**
         vdark threshold.
         Range: (0, +infinity].
         Default is 0.5.
         */
        float darkValueThreshold;
        
        /**
         saturation balancing. This is for compensating the vdark influence on saturation. The higher vdark is, the lower the saturation should be (i.e, under bright light, the color looks pale).
         Range: (0, +infinity].
         Default is 0.9.
         */
        float saturationBalancing;
        
        /**
         Whether or not it should adjust saturation.
         Default is true.
         */
        bool saturationAdjustment;
    };
}

#pragma GCC visibility pop
