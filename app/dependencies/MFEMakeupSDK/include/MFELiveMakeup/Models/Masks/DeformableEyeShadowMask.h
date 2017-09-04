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
    
    class DeformableEyeShadowMask : public DeformableMakeupMask {
    public:
        //! The reference eye points defined within the coordinate system of the mask image
        std::array<cv::Point2f, 4> referenceEyePoints;
        
        //! The reference brow points defined within the coordinate system of the mask image
        std::array<cv::Point2f, 3> referenceBrowPoints;
        
        //! The width of the mask image (in pixels)
        int referenceWidth;
        
        //! The height of the mask image (in pixels)
        int referenceHeight;
        
        /**
         The size of the area to deform around each reference point (default is 1.0).
         Increasing this value tends to make the deformation more continuous throughout, but also
         less precise (the reference points do not match up as closely to the target points).
         */
        float deformationAreaFactor;
        
        DeformableEyeShadowMask(int numRows, int numCols);
        DeformableEyeShadowMask(std::shared_ptr<SharedDeformableGrid> sharedGrid);
        
        void updateWithTrackingData(const TrackingData &data) override;
        bool isStaticMask() const override;
        bool equals(const MakeupMask& other) const override;
        std::shared_ptr<MakeupMask> clone() const override;
    };
    
    typedef std::shared_ptr<DeformableEyeShadowMask> DeformableEyeShadowMaskPtr;
    
    /** @}*/
}

#pragma GCC visibility pop
