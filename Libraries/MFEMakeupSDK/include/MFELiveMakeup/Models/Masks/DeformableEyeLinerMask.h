//
//  MFELiveMakeup
//  Copyright © 2017 ModiFace Inc. All rights reserved.
//

#pragma once

#include "DeformableMakeupMask.h"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup Models
     *  @{
     */
    
    class DeformableEyeLinerMask : public DeformableMakeupMask {
    public:
        //! The reference eye points defined within the coordinate system of the mask image
        std::array<cv::Point2f, 4> referenceEyePoints;
        
        //! The width of the mask image (in pixels)
        int referenceWidth;
        
        //! The height of the mask image (in pixels)
        int referenceHeight;
        
        DeformableEyeLinerMask(int numRows, int numCols);
        DeformableEyeLinerMask(std::shared_ptr<SharedDeformableGrid> sharedGrid);
        
        void updateWithTrackingData(const TrackingData &data) override;
        bool isStaticMask() const override;
        bool equals(const MakeupMask& other) const override;
        std::shared_ptr<MakeupMask> clone() const override;
    };
    
    typedef std::shared_ptr<DeformableEyeLinerMask> DeformableEyeLinerMaskPtr;
    
    /** @}*/
}

#pragma GCC visibility pop
