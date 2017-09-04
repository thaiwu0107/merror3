//
//  MFELiveMakeup
//  Copyright © 2017 ModiFace Inc. All rights reserved.
//

#pragma once

#include "MakeupMask.h"

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup Models
     *  @{
     */
    
    /**
     This is a deformable version of MakeupMask. Unlike MakeupMask where you can only apply a
     linear (usually affine) warping to the entire mask, this mask type allows you to apply a non-linear warping to the mask.
     
     This is useful for more complex masks that need to deform to fit some shape. For example, eye shadow masks that deform to
     wrap around the eyes and brows.
     
     The warping is defined using a rectangular 2D grid of points. The coordinate system of that grid is defined with respect
     to the vertices in the base mask class (MakeupMask). Point (0, 0) of the grid corresponds to the top
     left corner vertex (index 1) and (1, 1) corresponds with the bottom right corner vertex (index 2).
     
     This mask assumes that the vertices are ordered as: bottom-left, top-left, bottom-right, top-right.
     
     Important: the default assignment and copy constructor does a shallow copy of the deformable grid.
     This is useful if you want to have multiple masks share the same grid for performance reasons. For example,
     if multiple eyeshadow masks share the same grid, that grid will only be updated once per frame. For this to work,
     an unique ID > 0 must be assigned to each frame for TrackingData. For backwards compatibility,
     if the frame ID = 0 (the default value), then this optimization will be turned off and the grid will be updated
     multiple times per frame.
     
     If you do not want to share the grid when creating a copy of the mask, use the clone() function to create a deep copy.
     */
    class DeformableMakeupMask : public MakeupMask {
    public:
        class SharedDeformableGrid {
        public:
            DeformableGrid grid;
            unsigned int lastFrameUpdateID; // The frame ID in which this grid was last updated, with 0 meaning never updated
            
            SharedDeformableGrid(int numRows, int numCols) : grid(numRows, numCols), lastFrameUpdateID(0) {};
        };
        
        DeformableMakeupMask(int numRows, int numCols);
        DeformableMakeupMask(std::shared_ptr<SharedDeformableGrid> sharedGrid);
        virtual ~DeformableMakeupMask();
        
        std::shared_ptr<SharedDeformableGrid> sharedGrid;
        
        virtual bool isDeformableMask() const;
        
    protected:
        // Some helper functions and classes
        
        static cv::Matx33f computeImageToGridCoordsTransform(cv::Size imageSize, std::array<float, 8> vertices);
        
        typedef struct {
            cv::Point2f pt1;
            cv::Point2f pt2;
            cv::Point2f cp;
            
            // t ranges from 0 to 1
            cv::Point2f at(float t) {
                return (1 - t) * (1 - t) * pt1 + 2 * (1 - t) * t * cp + t * t * pt2;
            }
        } QuadraticBezier;
        
        typedef struct WeightedPointPair {
            /// Source point (reference point) for warping
            cv::Point2f srcPt;
            
            /// Target point for warping. The warping is done such that the srcPt will match dstPt.
            cv::Point2f dstPt;
            
            // Weighting (default is 1.0)
            // This value is entirely relative. For example a pair with weight 1.1 will have a stronger effect
            // relative to another pair with weight 1.0.
            float weight;
            
            /// To be used by the warping algorithm, do not fill in
            float computedWeight;
            
            WeightedPointPair() : weight(1.0), computedWeight(0.0) {};
            WeightedPointPair(cv::Point2f src, cv::Point2f dst, float weight = 1.0) : srcPt(src), dstPt(dst), weight(weight), computedWeight(0.0) {};
        } WeightedPointPair;
        
        bool gridRequiresUpdate(unsigned int frameID) { return frameID == 0 || frameID != sharedGrid->lastFrameUpdateID; }
    };
    
    typedef std::shared_ptr<DeformableMakeupMask> DeformableMakeupMaskPtr;
    
    /** @}*/
}

#pragma GCC visibility pop
