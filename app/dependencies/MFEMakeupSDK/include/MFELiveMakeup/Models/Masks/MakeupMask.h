//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <memory>
#include <array>
#include <string>

#include <opencv2/opencv.hpp>
#include <MFELiveCommon.h>

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup Models
     *  @{
     */

    /**
     * A class to represent a makeup mask. In general, there are two types of masks:
     * Static masks: masks that have a static image, but whose vertices change based on the face points
     * Dynamic masks: masks where both the image and vertices change based on the face points

     * Different classes of makeup masks should inherit from this class and offer more specific functionality.
     * For example, a blush mask can inherit from this and offer a few extra parameters,
     * such as how far out and how large the blush is. These parameters are exposed to mask creators and
     * makes fine tuning the masks more intuitive.
     */
    class MakeupMask {
    public:
        /**
         * Default constructor
         */
        MakeupMask();

        virtual ~MakeupMask();

        /**
         * Represents the 4 vertices of a (warped) box, in image coordinates. Ideally, the transformation between
         * 4 vertices of any unrotated rectangular box and these 4 vertices should be affine, or else OpenGL
         * will have issues when drawing the texture. The mask texture is drawn inside this box and warped to fit.
         * The ordering of the array is: pt1.x, pt1.y, pt2.x, pt2.y, ..., pt4.x, pt4.y.
         */
        std::array<float, 8> vertices;

        /**
         * For masks that use a static image, this contains the filename or path to a grayscale image.
         * Use an empty string if this is not a static mask.
         * Default value is an empty string.
         */
        std::string staticMaskFilename;

        /**
         * Set this to true if the mask should be horizontally flipped.
         */
        bool isStaticMaskHorizontallyFlipped;

        /**
         * For masks that use a dynamic image, this contains a CV_8UC1 image.
         * It should be set by the updateWithTrackingData function.
         */
        cv::Mat dynamicMaskImage;

        /**
         * Ranges from 0.0 to 1.0. For some mask types, this can change based on the face features. For example, blush
         * may dim on face rotations. This is set by the mask object itself and should not be changed from outside.
         */
        float intensity;

        /**
         * Indicates to the renderer that it should draw lines and points around the vertices.
         */
        bool shouldDrawDebugRegion;

        /**
         * Updates the mask based on the latest face point and image data.
         * For all masks, this will update the vertices and intesity.
         * For dynamic masks, this will also update the mask image.

         * The camera image is also passed in, which
         * is useful for masks that depend on the image data (e.g. the eyebrow mask).
         */
        virtual void updateWithTrackingData(const TrackingData &data) = 0;

        /**
         * For masks that depend on which side of the face it is on.
         */
        enum class Side {
            Left,
            Right,
            None
        };

        /**
         Which side of the face it is on.
         This is not required to set.
         For Blush, Brow, EyeErase, Eyeliner, Eyeshadow, default is Side::Left
         For Foundation, Lipliner, Lipstick, default is Side::None;
         */
        Side side;

        /**
         * Indicates whether this is a static mask. Static masks use
         * static images while dynamic masks use dynamically generated images.
         */
        virtual bool isStaticMask() const;

        /**
         * Whether this is a deformable mask (a subclass DeformableMakeupMask).
         * This is required due to a lack of RTTI.
         */
        virtual bool isDeformableMask() const;
        
        virtual bool equals(const MakeupMask &other) const;

        virtual std::shared_ptr<MakeupMask> clone() const = 0;

    protected:
        /**
         * Helper functions.
         * You only need to worry about these if you are creating a new class of masks.
         */

        static cv::Point2f getCoordPoint(cv::Point2f base, cv::Point2f xAxis, cv::Point2f yAxis,
                                         float magx, float magy);

        template<size_t Len>
        static std::array<cv::Point2f, Len> getBitmapCoordsVectorArray(
                std::array<cv::Point2f, Len> vecs, cv::Point2f bitmapTopLeft,
                cv::Point2f leftSideVector) {

            for (int i = 0; i < Len; i++) {
                vecs[i] = vecs[i] - bitmapTopLeft;
                float thetaFace = (float) acos(vecs[i].dot(leftSideVector) /
                                               (cv::norm(vecs[i]) * cv::norm(leftSideVector)));
                float xAdj = (float) sin(thetaFace) * cv::norm(vecs[i]);
                float yAdj = (float) cos(thetaFace) * cv::norm(vecs[i]);
                vecs[i] = cv::Point2f(xAdj, yAdj);
            }

            return vecs;
        }

        template<size_t N>
        static std::array<float, N * 2> convertFromPointToFloatArray(
                std::array<cv::Point2f, N> array) {
            std::array<float, N * 2> output;

            int i = 0;
            for (auto pt : array) {
                output[i++] = pt.x;
                output[i++] = pt.y;
            }

            return output;
        }

        static std::array<float, 8> verticesFromRect(cv::Rect box);
    };

    typedef std::shared_ptr<MakeupMask> MakeupMaskPtr;

    bool operator==(const MakeupMask &lhs, const MakeupMask &rhs);

    bool operator!=(const MakeupMask &lhs, const MakeupMask &rhs);

    /** @}*/
}

#pragma GCC visibility pop
