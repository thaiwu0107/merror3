//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include "MFELiveMakeup/Models/MakeupLook.h"
#include "MFELiveMakeup/Models/MakeupProcedure.h"

/**
 * This class models what makeup products are applied and their application style.
 * You can extend this class with additional styles and makeup types if desired.
 * Alternatively, you may design your own model class. This one is provided as a
 * basic starting point.
 *
 * For basic use cases this class should be sufficient for describing the applied makeup.
 * For more advanced use cases you may need to manually create and define some makeup procedures.
 *
 */
class AppliedMakeup {
public:
    template<typename StyleT> class MakeupApplication {
    public:
        MFE::MakeupProduct product;
        bool isEnabled;
        StyleT style;
        
        operator bool() const { return isEnabled; }
        
        MakeupApplication() : isEnabled(false), style(static_cast<StyleT>(0)) {};
        MakeupApplication(MFE::MakeupProduct prod, StyleT style) : product(prod), isEnabled(true), style(style) {};
    };
    
    enum class DefaultStyle { Default };
    
    enum class BlushStyle {
        DiagonalBlurUp,
        DiagonalBlurDown,
        Circular,
        Diagonal
    };
    
    enum class MascaraStyle {
        Top,
        Full,
        DenseTop,
        DenseFull
    };
    
    enum class EyeLinerStyle {
        Top,
        Full,
        Natural,
        CatEye
    };
    
    MakeupApplication<DefaultStyle> lipstick;
    MakeupApplication<DefaultStyle> lipLiner;
    MakeupApplication<BlushStyle> blush;
    MakeupApplication<DefaultStyle> brow;
    MakeupApplication<DefaultStyle> foundation;
    MakeupApplication<DefaultStyle> lidShadow;
    MakeupApplication<DefaultStyle> creaseShadow;
    MakeupApplication<DefaultStyle> highlightShadow;
    MakeupApplication<EyeLinerStyle> eyeLiner;
    MakeupApplication<MascaraStyle> mascara;
};

/**
 * This is a utility class that generates a MFE::MakeupLook given an AppliedMakeup object.
 */
class LookGenerator {
protected:
    std::string mAssetPath;
    std::string pathForResource(std::string name);

    MFE::MakeupProcedurePtr createBlushProcedure(MFE::MakeupProduct product, AppliedMakeup::BlushStyle style);
    MFE::MakeupProcedurePtr createLipLinerProcedure(MFE::MakeupProduct product);
    MFE::MakeupProcedurePtr createBrowRecolourProcedure(MFE::MakeupProduct product);
    MFE::MakeupProcedurePtr createEyeLinerProcedure(MFE::MakeupProduct product, AppliedMakeup::EyeLinerStyle style);
    MFE::MakeupProcedurePtr createEyeShadowsProcedure(MFE::MakeupProduct lidProduct, MFE::MakeupProduct creaseProduct, MFE::MakeupProduct highlightProduct);
    MFE::MakeupProcedurePtr createFoundationProcedure(MFE::MakeupProduct product);
    MFE::MakeupProcedurePtr createLipstickProcedure(MFE::MakeupProduct product);
    MFE::MakeupProcedurePtr createLashProcedure(MFE::MakeupProduct product, AppliedMakeup::MascaraStyle style);
    
public:
    LookGenerator(std::string assetPath);
    MFE::MakeupLook generateMakeupLook(const AppliedMakeup &appliedMakeup);
};
