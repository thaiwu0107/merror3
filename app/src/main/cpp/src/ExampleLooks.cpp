//
//  ExampleLooks.cpp
//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#include "ExampleLooks.hpp"

#include "LookGenerator.hpp"
#include "DemoOverlayMask.hpp"

#include "MFELiveMakeup/Models/Masks/DeformableEyeShadowMask.h"
#include "MFELiveMakeup/Models/Masks/DeformableEyeLinerMask.h"

static std::string pathForResource(std::string basePath, std::string resource) {
    return basePath + (basePath.back() == '/' ? "" : "/") + resource;
}

AppliedMakeup makeup;

MFE::MakeupLook ExampleLooks::getLook(std::string resourcesPath) {
    LookGenerator lookGenerator(resourcesPath);
    return lookGenerator.generateMakeupLook(makeup);
}

void ExampleLooks::addLips(int color, float opacity, float glitter, float gloss){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.gloss = gloss*4.0;
    product.sparkleIntensity = glitter;
    product.intensity = opacity;
    product.wetness = gloss*1.0;

    makeup.lipstick.isEnabled = true;
    makeup.lipstick = AppliedMakeup::MakeupApplication<AppliedMakeup::DefaultStyle>(
            product, AppliedMakeup::DefaultStyle::Default);

}
void ExampleLooks::removeLips(){
    makeup.lipstick.isEnabled = false;
}

void ExampleLooks::addFoundation(int color, float opacity, float smoothing) {
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;
    product.skinClearing = smoothing;

    makeup.foundation.isEnabled = true;
    makeup.foundation = AppliedMakeup::MakeupApplication<AppliedMakeup::DefaultStyle>(product, AppliedMakeup::DefaultStyle::Default);
}
void ExampleLooks::removeFoundation(){
    makeup.foundation.isEnabled = false;
}

void ExampleLooks::addEyeliner(int color, int style, float opacity){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;

    makeup.eyeLiner.isEnabled = true;
    makeup.eyeLiner = AppliedMakeup::MakeupApplication<AppliedMakeup::EyeLinerStyle>(product, static_cast<AppliedMakeup::EyeLinerStyle>(style));
}
void ExampleLooks::removeEyeliner(){
    makeup.eyeLiner.isEnabled = false;
}

void ExampleLooks::addLash(int color, int style, float opacity){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;

    makeup.mascara.isEnabled = true;
    makeup.mascara = AppliedMakeup::MakeupApplication<AppliedMakeup::MascaraStyle >(product, static_cast<AppliedMakeup::MascaraStyle >(style));
}
void ExampleLooks::removeLash(){
    makeup.mascara.isEnabled = false;
}

void ExampleLooks::addBlush(int color, int style, float opacity){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;

    makeup.blush.isEnabled = true;
    makeup.blush = AppliedMakeup::MakeupApplication<AppliedMakeup::BlushStyle >(product, static_cast<AppliedMakeup::BlushStyle >(style));
}
void ExampleLooks::removeBlush(){
    makeup.blush.isEnabled = false;
}

void ExampleLooks::addBrow(int color, float opacity){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = 3.0f*opacity;

    makeup.brow.isEnabled = true;
    makeup.brow = AppliedMakeup::MakeupApplication<AppliedMakeup::DefaultStyle>(product, AppliedMakeup::DefaultStyle::Default);
}
void ExampleLooks::removeBrow(){
    makeup.brow.isEnabled = false;
}

void ExampleLooks::addLipliner(int color, float opacity){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;

    makeup.lipLiner.isEnabled = true;
    makeup.lipLiner = AppliedMakeup::MakeupApplication<AppliedMakeup::DefaultStyle>(product, AppliedMakeup::DefaultStyle::Default);
}
void ExampleLooks::removeLipliner(){
    makeup.lipLiner.isEnabled = false;
}

bool eyeshadowLidEnabled = false;
bool eyeshadowCreaseEnabled = false;
bool eyeshadowHighEnabled = false;

void ExampleLooks::addEyeshadowLid(int color, float opacity, float glitter){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;
    product.sparkleIntensity = glitter;

    eyeshadowLidEnabled = true;
    if (eyeshadowHighEnabled == false) makeup.highlightShadow.product.intensity = 0;
    if (eyeshadowCreaseEnabled == false) makeup.creaseShadow.product.intensity = 0;
    makeup.lidShadow.isEnabled = true;
    makeup.lidShadow = AppliedMakeup::MakeupApplication<AppliedMakeup::DefaultStyle>(product, AppliedMakeup::DefaultStyle::Default);
}
void ExampleLooks::removeEyeshadowLid(){
    eyeshadowLidEnabled = false;
    makeup.lidShadow.product.intensity = 0;
    makeup.lidShadow.isEnabled = false;
}

void ExampleLooks::addEyeshadowCrease(int color, float opacity, float glitter){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;
    product.sparkleIntensity = glitter;

    eyeshadowCreaseEnabled = true;
    if (eyeshadowHighEnabled == false) makeup.highlightShadow.product.intensity = 0;
    if (eyeshadowLidEnabled == false) makeup.lidShadow.product.intensity = 0;
    makeup.creaseShadow.isEnabled = true;
    makeup.creaseShadow = AppliedMakeup::MakeupApplication<AppliedMakeup::DefaultStyle>(product, AppliedMakeup::DefaultStyle::Default);
}
void ExampleLooks::removeEyeshadowCrease(){
    eyeshadowCreaseEnabled = false;
    makeup.creaseShadow.product.intensity = 0;
    makeup.creaseShadow.isEnabled = false;
}

void ExampleLooks::addEyeshadowHigh(int color, float opacity, float glitter){
    MFE::MakeupProduct product;
    product.colorR = (color & 0x00FF0000) >> 16;
    product.colorG = (color & 0x0000FF00) >> 8;
    product.colorB = (color & 0x000000FF);
    product.colorA = 255;
    product.intensity = opacity;
    product.sparkleIntensity = glitter;

    eyeshadowHighEnabled = true;
    if (eyeshadowCreaseEnabled == false) makeup.creaseShadow.product.intensity = 0;
    if (eyeshadowLidEnabled == false) makeup.lidShadow.product.intensity = 0;
    makeup.highlightShadow.isEnabled = true;
    makeup.highlightShadow = AppliedMakeup::MakeupApplication<AppliedMakeup::DefaultStyle>(product, AppliedMakeup::DefaultStyle::Default);
}
void ExampleLooks::removeEyeshadowHigh(){
    eyeshadowHighEnabled = false;
    makeup.highlightShadow.product.intensity = 0;
    makeup.highlightShadow.isEnabled = false;
}