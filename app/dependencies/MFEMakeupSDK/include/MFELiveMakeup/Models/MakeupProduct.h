//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include "DynamicColorParameters.h"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup Models
     *  @{
     */

    /**
     * Class for representing a particular makeup product's material properties. This only contains information about
     * its material appearance (e.g. color, glitter, etc.) and does not contain placement information or product
     * meta information (e.g. name, description, etc.). These other details are handled by other classes.
     */
    class MakeupProduct {
    public:
        //@{
        /**
         * RGBA color of the makeup (0 - 255).
         * Default value: RGBA(0, 0, 0, 255)
         * The alpha component controls the coloring intensity.
         * For example, setting alpha to zero but having a non-zero sparkle intensity will cause it to apply glitter only.
         */
        int colorR;
        int colorG;
        int colorB;
        int colorA;
        //@}

        /**
         * Makeup intensity (0.0 - 1.0). Controls the opacity of the makeup.
         * Default value: 1.0.
         */
        float intensity;

        /**
         * Makeup gloss (0.0 and higher, unbounded).
         * Default value: 0.0.
         * Controls the gloss of the makeup by amplifying relatively bright regions.
         * As a reference, a value of 4.0 works well for lipgloss.
         */
        float gloss;

        /**
         Makeup gloss detail (0.0 - 1.0)
         Default value: 0.0.
         Controls the amount of detail in the gloss. Using a small value (e.g. 0) produces a
         more metallic look while larger values create a more wet appearance.
         This is currently only supported by MFEMakeupProcedureLipstick.
         */
        float glossDetail;

        /**
         * Controls the hardness of the gloss effect (0.0 - 1.0) to produce a wet-like effect.
         * Default value: 0.0.
         * This is currently only supported by MakeupProcedureLipstick.
         */
        float wetness;

        /**
         * Makeup glitter intensity (0.0 - 1.0). This controls the opacity of the sparkles.
         * Default value: 0.0.
         */
        float sparkleIntensity;

        /**
         * Makeup glitter RGB (0 - 255). Usually this is white.
         * Default value: RGB(255, 255, 255).
         */
        int sparkleR;
        int sparkleG;
        int sparkleB;

        /**
         * Makeup glitter density (0.0 - 1.0). This controls how many sparkles there are.
         * Default value: 1.0.
         */
        float sparkleDensity;

        /**
         * Skin clearing effect (0.0 - 1.0).
         * Default value: 0.0.
         * Some products may have a skin clearing effect (for example, removing redness, pimples, etc.).
         * This is currently only supported by MakeupProcedureFoundation.
         */
        float skinClearing;
        
        /**
         * Skin glow effect (0.0 - 1.0).
         * Default value: 0.0.
         * Some products may have a skin glowing effect.
         * This is currently only supported by MakeupProcedureFoundation.
         */
        float skinGlow;

        /**
         * Whether or not enable dynamicColor. Dynamic color tries to adjust the makeup rendering to match
         * the current lighting conditions.
         * Default value: true.
         */
        bool useDynamicColor;
        
        /**
         Parameters used for dynamic coloring. The default values should suffice for most applications.
         */
        DynamicColorParameters dynamicColorParameters;

        // Constructors
        MakeupProduct();
        MakeupProduct(int colorR, int colorG, int colorB, int colorA, float intensity,
                      float gloss, float glossDetail, float wetness, float sparkleIntensity,
                      int sparkleR, int sparkleG, int sparkleB, float sparkleDensity,
                      float skinClearing, float skinGlow, bool useDynamicColor);

        // Equality operators
        bool operator==(const MakeupProduct &rhs) const;

        bool operator!=(const MakeupProduct &rhs) const;
    };

    /** @}*/
}

#pragma GCC visibility pop
