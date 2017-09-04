//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <memory>
#include <MFELiveMakeup/Models/Masks/MakeupMask.h>

class DemoOverlayMask : public MFE::MakeupMask {

public:
    DemoOverlayMask();

    virtual ~DemoOverlayMask();

    void updateWithTrackingData(const MFE::TrackingData &data) override;

    std::shared_ptr<MFE::MakeupMask> clone() const override;

};

typedef std::shared_ptr<DemoOverlayMask> DemoOverlayMaskPtr;
