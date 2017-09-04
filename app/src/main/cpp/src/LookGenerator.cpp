//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#include "LookGenerator.hpp"

#include "MFELiveMakeup/Models/Masks/BlushMask.h"
#include "MFELiveMakeup/Models/Masks/LipLinerMask.h"
#include "MFELiveMakeup/Models/Masks/BrowMask.h"
#include "MFELiveMakeup/Models/Masks/EyeLinerMask.h"
#include "MFELiveMakeup/Models/Masks/FoundationMask.h"
#include "MFELiveMakeup/Models/Masks/LipstickMask.h"
#include "MFELiveMakeup/Models/Masks/EyeEraseMask.h"
#include "MFELiveMakeup/Models/Masks/EyeShadowMask.h"

#include "DemoOverlayMask.hpp"

LookGenerator::LookGenerator(std::string assetPath) {
    mAssetPath = assetPath + (assetPath.back() == '/' ? "" : "/");
}

MFE::MakeupLook LookGenerator::generateMakeupLook(const AppliedMakeup &appliedMakeup) {
    MFE::MakeupLook look;
    
    if (appliedMakeup.foundation) {
        MFE::MakeupProcedurePtr foundation = createFoundationProcedure(appliedMakeup.foundation.product);
        look.procedures.push_back(foundation);
    }

    if (appliedMakeup.blush) {
        MFE::MakeupProcedurePtr blush = createBlushProcedure(appliedMakeup.blush.product, appliedMakeup.blush.style);
        look.procedures.push_back(blush);
    }

    if (appliedMakeup.lipstick) {
        MFE::MakeupProcedurePtr lipstick = createLipstickProcedure(appliedMakeup.lipstick.product);
        look.procedures.push_back(lipstick);
    }
    
    if (appliedMakeup.lipLiner) {
        MFE::MakeupProcedurePtr lipLiner = createLipLinerProcedure(appliedMakeup.lipLiner.product);
        look.procedures.push_back(lipLiner);
    }
    
    if (appliedMakeup.brow) {
        MFE::MakeupProcedurePtr browRecolour = createBrowRecolourProcedure(appliedMakeup.brow.product);
        look.procedures.push_back(browRecolour);
    }
    
    if (appliedMakeup.lidShadow || appliedMakeup.creaseShadow || appliedMakeup.highlightShadow) {
        MFE::MakeupProcedurePtr eyeShadows = createEyeShadowsProcedure(appliedMakeup.lidShadow.product, appliedMakeup.creaseShadow.product, appliedMakeup.highlightShadow.product);
        look.procedures.push_back(eyeShadows);
    }
    
    if (appliedMakeup.eyeLiner) {
        MFE::MakeupProcedurePtr eyeLiner = createEyeLinerProcedure(appliedMakeup.eyeLiner.product, appliedMakeup.eyeLiner.style);
        look.procedures.push_back(eyeLiner);
    }
    
    if (appliedMakeup.mascara) {
        MFE::MakeupProcedurePtr lash = createLashProcedure(appliedMakeup.mascara.product, appliedMakeup.mascara.style);
        look.procedures.push_back(lash);
    }
    
    return look;
}

std::string LookGenerator::pathForResource(std::string name) {
    return mAssetPath + name;
}

MFE::MakeupProcedurePtr LookGenerator::createBlushProcedure(MFE::MakeupProduct product, AppliedMakeup::BlushStyle style) {
    std::string blushMaskFilename;
    
    switch (style) {
        case AppliedMakeup::BlushStyle::DiagonalBlurUp: blushMaskFilename = "ml_msk_blush_1.png"; break;
        case AppliedMakeup::BlushStyle::DiagonalBlurDown: blushMaskFilename = "ml_msk_blush_2.png"; break;
        case AppliedMakeup::BlushStyle::Circular: blushMaskFilename = "ml_msk_blush_3.png"; break;
        case AppliedMakeup::BlushStyle::Diagonal: blushMaskFilename = "ml_msk_blush_4.png"; break;
        default: break;
    }
    
    MFE::BlushMaskPtr leftDrawMask = std::make_shared<MFE::BlushMask>();
    leftDrawMask->staticMaskFilename = pathForResource("Masks/Blush/" + blushMaskFilename);
    leftDrawMask->side = MFE::MakeupMask::Side::Left;

    MFE::BlushMaskPtr rightDrawMask = std::make_shared<MFE::BlushMask>(*leftDrawMask);
    rightDrawMask->side = MFE::MakeupMask::Side::Right;
    rightDrawMask->isStaticMaskHorizontallyFlipped = true;

    MFE::BlushMaskPtr leftEraseMask = std::make_shared<MFE::BlushMask>();
    leftEraseMask->staticMaskFilename = pathForResource("Masks/Subtract/blush_drop_4_sub_left.png");
    leftEraseMask->side = MFE::MakeupMask::Side::Left;
    leftEraseMask->isSubtractMask = true;

    MFE::BlushMaskPtr rightEraseMask = std::make_shared<MFE::BlushMask>(*leftEraseMask);
    rightEraseMask->side = MFE::MakeupMask::Side::Right;
    rightEraseMask->isStaticMaskHorizontallyFlipped = true;

    MFE::MakeupActionDrawMakeupPtr drawLeftBlush = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawLeftBlush->mask = leftDrawMask;
    drawLeftBlush->product = product;

    MFE::MakeupActionDrawMakeupPtr drawRightBlush = std::make_shared<MFE::MakeupActionDrawMakeup>(*drawLeftBlush);
    drawRightBlush->mask = rightDrawMask;

    MFE::MakeupActionErasePtr eraseLeftBlush = std::make_shared<MFE::MakeupActionErase>();
    eraseLeftBlush->mask = leftEraseMask;

    MFE::MakeupActionErasePtr eraseRightBlush = std::make_shared<MFE::MakeupActionErase>(*eraseLeftBlush);
    eraseRightBlush->mask = rightEraseMask;

    MFE::MakeupProcedureGeneralPtr blushProcedure = std::make_shared<MFE::MakeupProcedureGeneral>();
    blushProcedure->name = "Blush";
    blushProcedure->actions.push_back(drawLeftBlush);
    blushProcedure->actions.push_back(drawRightBlush);
    blushProcedure->actions.push_back(eraseLeftBlush);
    blushProcedure->actions.push_back(eraseRightBlush);

    return blushProcedure;
}

MFE::MakeupProcedurePtr LookGenerator::createLipLinerProcedure(MFE::MakeupProduct product) {
    MFE::LipLinerMaskPtr lipLinerMask = std::make_shared<MFE::LipLinerMask>();

    MFE::MakeupActionDrawMakeupPtr drawLiner = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawLiner->mask = lipLinerMask;
    drawLiner->product = product;

    MFE::MakeupProcedureGeneralPtr lipLinerProcedure = std::make_shared<MFE::MakeupProcedureGeneral>();
    lipLinerProcedure->name = "Lip Liner";
    lipLinerProcedure->actions.push_back(drawLiner);

    return lipLinerProcedure;
}

MFE::MakeupProcedurePtr LookGenerator::createBrowRecolourProcedure(MFE::MakeupProduct product) {
    MFE::BrowMaskPtr leftBrowMask = std::make_shared<MFE::BrowMask>();
    leftBrowMask->side = MFE::BrowMask::Side::Left;

    MFE::BrowMaskPtr rightBrowMask = std::make_shared<MFE::BrowMask>();
    rightBrowMask->side = MFE::BrowMask::Side::Right;

    MFE::MakeupActionDrawMakeupPtr drawBrowLeft = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawBrowLeft->mask = leftBrowMask;
    drawBrowLeft->product = product;

    MFE::MakeupActionDrawMakeupPtr drawBrowRight = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawBrowRight->mask = rightBrowMask;
    drawBrowRight->product = product;

    MFE::MakeupProcedureBrowRecolourPtr browRecolourProcedure = std::make_shared<MFE::MakeupProcedureBrowRecolour>();
    browRecolourProcedure->name = "Brow";
    browRecolourProcedure->actions.push_back(drawBrowLeft);
    browRecolourProcedure->actions.push_back(drawBrowRight);

    return browRecolourProcedure;
}

MFE::MakeupProcedurePtr LookGenerator::createEyeLinerProcedure(MFE::MakeupProduct product, AppliedMakeup::EyeLinerStyle style) {
    MFE::EyeLinerMask::LinerStyle linerStyle = MFE::EyeLinerMask::LinerStyle::Natural;
    
    switch (style) {
        case AppliedMakeup::EyeLinerStyle::Top: linerStyle = MFE::EyeLinerMask::LinerStyle::Natural; break;
        case AppliedMakeup::EyeLinerStyle::Full: linerStyle = MFE::EyeLinerMask::LinerStyle::Full; break;
        case AppliedMakeup::EyeLinerStyle::Natural: linerStyle = MFE::EyeLinerMask::LinerStyle::Natural; break;
        case AppliedMakeup::EyeLinerStyle::CatEye: linerStyle = MFE::EyeLinerMask::LinerStyle::CatEye; break;
        default: break;
    }

    MFE::EyeLinerMaskPtr leftMask = std::make_shared<MFE::EyeLinerMask>();
    leftMask->side = MFE::BrowMask::Side::Left;
    leftMask->linerStyle = linerStyle;

    MFE::EyeLinerMaskPtr rightMask = std::make_shared<MFE::EyeLinerMask>(*leftMask);
    rightMask->side = MFE::BrowMask::Side::Right;

    MFE::MakeupActionDrawMakeupPtr drawLeft = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawLeft->mask = leftMask;
    drawLeft->product = product;

    MFE::MakeupActionDrawMakeupPtr drawRight = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawRight->mask = rightMask;
    drawRight->product = product;

    MFE::MakeupProcedureGeneralPtr eyeLinerProcedure = std::make_shared<MFE::MakeupProcedureGeneral>();
    eyeLinerProcedure->name = "Eye liner";
    eyeLinerProcedure->actions.push_back(drawLeft);
    eyeLinerProcedure->actions.push_back(drawRight);

    return eyeLinerProcedure;
}

MFE::MakeupProcedurePtr LookGenerator::createEyeShadowsProcedure(MFE::MakeupProduct lidProduct, MFE::MakeupProduct creaseProduct, MFE::MakeupProduct highlightProduct) {
    
    MFE::MakeupProcedureGeneralPtr eyeshadowProcedure = std::make_shared<MFE::MakeupProcedureGeneral>();
    eyeshadowProcedure->name = "Eyeshadows";
    
    MFE::EyeShadowMaskPtr leftLidMask = std::make_shared<MFE::EyeShadowMask>();
    leftLidMask->side = MFE::EyeShadowMask::Side::Left;
    leftLidMask->staticMaskFilename = pathForResource("Masks/Eyeshadow/ml_msk_eyeshadow_1.png");
    
    MFE::EyeShadowMaskPtr leftCreaseMask = std::make_shared<MFE::EyeShadowMask>();
    leftCreaseMask->side = MFE::EyeShadowMask::Side::Left;
    leftCreaseMask->staticMaskFilename = pathForResource("Masks/Eyeshadow/ml_msk_eyeshadow_2.png");
    
    MFE::EyeShadowMaskPtr leftHighlightMask = std::make_shared<MFE::EyeShadowMask>();
    leftHighlightMask->side = MFE::EyeShadowMask::Side::Left;
    leftHighlightMask->staticMaskFilename = pathForResource("Masks/Eyeshadow/ml_msk_eyeshadow_3.png");
    
    MFE::EyeShadowMaskPtr rightLidMask = std::make_shared<MFE::EyeShadowMask>(*leftLidMask);
    rightLidMask->side = MFE::EyeShadowMask::Side::Right;
    rightLidMask->isStaticMaskHorizontallyFlipped = true;
    
    MFE::EyeShadowMaskPtr rightCreaseMask = std::make_shared<MFE::EyeShadowMask>(*leftCreaseMask);
    rightCreaseMask->side = MFE::EyeShadowMask::Side::Right;
    rightCreaseMask->isStaticMaskHorizontallyFlipped = true;
    
    MFE::EyeShadowMaskPtr rightHighlightMask = std::make_shared<MFE::EyeShadowMask>(*leftHighlightMask);
    rightHighlightMask->side = MFE::EyeShadowMask::Side::Right;
    rightHighlightMask->isStaticMaskHorizontallyFlipped = true;
    
    MFE::MakeupActionDrawMakeupPtr drawLeftLidMask = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawLeftLidMask->mask = leftLidMask;
    drawLeftLidMask->product = lidProduct;
    
    MFE::MakeupActionDrawMakeupPtr drawLeftCreaseMask = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawLeftCreaseMask->mask = leftCreaseMask;
    drawLeftCreaseMask->product = creaseProduct;
    
    MFE::MakeupActionDrawMakeupPtr drawLeftHighlightMask = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawLeftHighlightMask->mask = leftHighlightMask;
    drawLeftHighlightMask->product = highlightProduct;
    
    MFE::MakeupActionDrawMakeupPtr drawRightLidMask = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawRightLidMask->mask = rightLidMask;
    drawRightLidMask->product = lidProduct;
    
    MFE::MakeupActionDrawMakeupPtr drawRightCreaseMask = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawRightCreaseMask->mask = rightCreaseMask;
    drawRightCreaseMask->product = creaseProduct;
    
    MFE::MakeupActionDrawMakeupPtr drawRightHighlightMask = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawRightHighlightMask->mask = rightHighlightMask;
    drawRightHighlightMask->product = highlightProduct;
    
    eyeshadowProcedure->actions.push_back(drawLeftLidMask);
    eyeshadowProcedure->actions.push_back(drawLeftCreaseMask);
    eyeshadowProcedure->actions.push_back(drawLeftHighlightMask);
    
    eyeshadowProcedure->actions.push_back(drawRightLidMask);
    eyeshadowProcedure->actions.push_back(drawRightCreaseMask);
    eyeshadowProcedure->actions.push_back(drawRightHighlightMask);
    
    MFE::EyeEraseMaskPtr leftEyeMask = std::make_shared<MFE::EyeEraseMask>();
    leftEyeMask->side = MFE::EyeEraseMask::Side::Left;
    leftEyeMask->blurFactor = 0.5;
    leftEyeMask->expandFactor = 1.1;
    
    MFE::EyeEraseMaskPtr rightEyeMask = std::make_shared<MFE::EyeEraseMask>();
    rightEyeMask->side = MFE::EyeEraseMask::Side::Right;
    rightEyeMask->blurFactor = 0.5;
    rightEyeMask->expandFactor = 1.1;
    
    MFE::MakeupActionErasePtr eraseLeftAction = std::make_shared<MFE::MakeupActionErase>();
    eraseLeftAction->mask = leftEyeMask;
    eraseLeftAction->eraseAmount = 1.0;
    
    MFE::MakeupActionErasePtr eraseRightAction = std::make_shared<MFE::MakeupActionErase>();
    eraseRightAction->mask = rightEyeMask;
    eraseRightAction->eraseAmount = 1.0;
    
    eyeshadowProcedure->actions.push_back(eraseLeftAction);
    eyeshadowProcedure->actions.push_back(eraseRightAction);
    
    return eyeshadowProcedure;
}

MFE::MakeupProcedurePtr LookGenerator::createFoundationProcedure(MFE::MakeupProduct product) {
    MFE::FoundationMaskPtr foundationMask = std::make_shared<MFE::FoundationMask>();

    MFE::MakeupActionDrawMakeupPtr drawFoundation = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawFoundation->mask = foundationMask;
    drawFoundation->product = product;

    MFE::MakeupProcedureFoundationPtr foundation = std::make_shared<MFE::MakeupProcedureFoundation>();
    foundation->name = "Foundation";
    foundation->actions.push_back(drawFoundation);

    return foundation;
}

MFE::MakeupProcedurePtr LookGenerator::createLipstickProcedure(MFE::MakeupProduct product) {
    MFE::LipstickMaskPtr lipMask = std::make_shared<MFE::LipstickMask>();
    lipMask->blurFactor = 0.5;

    MFE::MakeupActionDrawMakeupPtr drawLip = std::make_shared<MFE::MakeupActionDrawMakeup>();
    drawLip->mask = lipMask;
    drawLip->product = product;

    MFE::MakeupProcedureLipstickPtr lipstick = std::make_shared<MFE::MakeupProcedureLipstick>();
    lipstick->reduceShineOnEdges = true;
    lipstick->name = "Lipstick";
    lipstick->actions.push_back(drawLip);

    return lipstick;
}

MFE::MakeupProcedurePtr LookGenerator::createLashProcedure(MFE::MakeupProduct product, AppliedMakeup::MascaraStyle style) {
    MFE::LashModel lashModelTop;
    lashModelTop.intensity = product.intensity;
    lashModelTop.isUpperLash = true;
    lashModelTop.contourThreshold = 10;
    lashModelTop.colorR = product.colorR;
    lashModelTop.colorG = product.colorG;
    lashModelTop.colorB = product.colorB;
    
    MFE::LashModel lashModelBottom = lashModelTop;
    lashModelBottom.isUpperLash = false;
    
    bool hasTopLash = false;
    bool hasBottomLash = false;
    
    if (style == AppliedMakeup::MascaraStyle::Top || style == AppliedMakeup::MascaraStyle::Full) {
        lashModelTop.minX = 31;
        lashModelTop.maxX = 141;
        lashModelTop.imagePath = pathForResource("Masks/Mascara/ml_msk_mascara_1.png");
        
        hasTopLash = true;
    }
    else if (style == AppliedMakeup::MascaraStyle::DenseTop || style == AppliedMakeup::MascaraStyle::DenseFull) {
        lashModelTop.minX = 31;
        lashModelTop.maxX = 134;
        lashModelTop.imagePath = pathForResource("Masks/Mascara/ml_msk_mascara_3.png");
        
        hasTopLash = true;
    }
    
    if (style == AppliedMakeup::MascaraStyle::Full) {
        lashModelBottom.minX = 22;
        lashModelBottom.maxX = 142;
        lashModelBottom.imagePath = pathForResource("Masks/Mascara/ml_msk_mascara_2.png");
        
        hasBottomLash = true;
    }
    else if (style == AppliedMakeup::MascaraStyle::DenseFull) {
        lashModelBottom.minX = 22;
        lashModelBottom.maxX = 140;
        lashModelBottom.imagePath = pathForResource("Masks/Mascara/ml_msk_mascara_4.png");
        
        hasBottomLash = true;
    }
    
    MFE::MakeupProcedureLashesPtr lash = std::make_shared<MFE::MakeupProcedureLashes>();
    
    if (hasTopLash) {
        MFE::MakeupActionDrawLashPtr drawTopLashLeft = std::make_shared<MFE::MakeupActionDrawLash>();
        drawTopLashLeft->lashModel = lashModelTop;
        drawTopLashLeft->isLeftSide = true;
        
        MFE::MakeupActionDrawLashPtr drawTopLashRight = std::make_shared<MFE::MakeupActionDrawLash>();
        drawTopLashRight->lashModel = lashModelTop;
        drawTopLashRight->isLeftSide = false;
        
        lash->actions.push_back(drawTopLashLeft);
        lash->actions.push_back(drawTopLashRight);
    }
    
    if (hasBottomLash) {
        MFE::MakeupActionDrawLashPtr drawBotLashLeft = std::make_shared<MFE::MakeupActionDrawLash>();
        drawBotLashLeft->lashModel = lashModelBottom;
        drawBotLashLeft->isLeftSide = true;
        
        MFE::MakeupActionDrawLashPtr drawBotLashRight = std::make_shared<MFE::MakeupActionDrawLash>();
        drawBotLashRight->lashModel = lashModelBottom;
        drawBotLashRight->isLeftSide = false;
        
        lash->actions.push_back(drawBotLashLeft);
        lash->actions.push_back(drawBotLashRight);
    }
    
    return lash;
}
