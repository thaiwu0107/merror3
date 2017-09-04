//
//  ExampleLooks.hpp
//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include "MFELiveMakeup/Models/MakeupLook.h"
#include "LookGenerator.hpp"

namespace ExampleLooks {
    MFE::MakeupLook getLook(std::string resourcesPath);

    void addLips(int color, float opacity, float glitter, float gloss);
    void removeLips();

    void addFoundation(int color, float opacity, float smoothing);
    void removeFoundation();

    void addEyeliner(int color, int style, float opacity);
    void removeEyeliner();

    void addLash(int color, int style, float opacity);
    void removeLash();

    void addBlush(int color, int style, float opacity);
    void removeBlush();

    void addBrow(int color, float opacity);
    void removeBrow();

    void addLipliner(int color, float opacity);
    void removeLipliner();

    void addEyeshadowLid(int color, float opacity, float glitter);
    void removeEyeshadowLid();

    void addEyeshadowCrease(int color, float opacity, float glitter);
    void removeEyeshadowCrease();

    void addEyeshadowHigh(int color, float opacity, float glitter);
    void removeEyeshadowHigh();
};
