//
//  Copyright (c) 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <array>
#include <functional>
#include <opencv2/opencv.hpp>

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup MFELiveCommon_Models
     *  @{
     */

    /**
     * This class stores the face points for a particular image or video frame.
     */
    class BaseFacePoints {
    public:

        /**
         * Default constructor (everything is initialized to zero).
         */
        BaseFacePoints();

        BaseFacePoints(const BaseFacePoints &pts);

        void operator=(const BaseFacePoints &pts);

        /**
         * Specifies the number of points for each face point group.
         */
        enum class PointCount : size_t {
            Eye = 4,
            Cheek = 4,
            Nose = 11,
            OuterMouth = 10,
            InnerMouth = 8,
            Brow = 3,
            Face = 9,
            Forehead = 3
        };

        /**
         * Face point groups.
         */
        enum class PointGroup {
            Undefined,
            LeftEye,
            RightEye,
            Cheek,
            Nose,
            OuterMouth,
            InnerMouth,
            LeftBrow,
            RightBrow,
            Face,
            Forehead
        };

        /*
         * All points are defined with respect to the coordinate system of the image.
         * The coordinate system has the origin at the top-left corner of the image, with
         * X going from left to right and Y going from top to bottom.
         */
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Eye)> leftEye2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Eye)> rightEye2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Cheek)> cheek2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Nose)> nose2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::OuterMouth)> outerMouth2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::InnerMouth)> innerMouth2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Brow)> leftBrow2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Brow)> rightBrow2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Face)> face2D;
        std::array<cv::Point2f, static_cast<size_t>(PointCount::Forehead)> forehead2D;

        /**
         * A boolean indicating whether the mouth is open or not.
         * @see estimateIsMouthOpen
         */
        bool isMouthOpen;

        /**
         * The width of the image that the face points belongs to.
         */
        int imageWidth;

        /**
         * The height of the image that the face points belongs to.
         */
        int imageHeight;

        /**
         * The face orientation angles (in radians).
         * Angle 0: Rotation about horizontal axis through sides of head (origin = 0, face up = -)
         * Angle 1: Rotation about vertical axis (origin = 0, turn right = +)
         * Angle 2: Rotation about axis from front to back of head (origin = 0, clockwise = -)
         */
        cv::Point3f faceRotation;

        /**
         * Function for enumerating through all the face points. The lambda passed in is called for every
         * face point, with pt and group being the current face point and group respectively, and index being the
         * index within the current group, beginning at 0. The point is passed as reference and can be modified.
         */
        void enumeratePoints(
                std::function<void(cv::Point2f &pt, PointGroup group, int index)> func);

        /**
         * Function for enumerating through all the face points (read-only).
         */
        void enumeratePoints(
                std::function<void(const cv::Point2f &pt, PointGroup group, int index)> func) const;

        /**
         * Estimates whether the mouth is open or not and modifies the isMouthOpen variable accordingly.
         */
        void estimateIsMouthOpen();

        // Helper methods
        int getTrueEyeWidth(bool isLeft) const;
        int getTrueEyeHeight(bool isLeft) const;
        int getTrueMouthWidth() const;
        int getTrueMouthHeight() const;
        int getNoseHeight() const;
        int getFaceWidth() const;
        int getFaceHeight() const;
        float getCheekSize(cv::Point2f origin, bool isLeft) const;
        cv::Rect getLeftEyeBox() const;
        cv::Rect getRightEyeBox() const;
        cv::Rect getMouthBox() const;

    protected:
        void copyFrom(const BaseFacePoints &pts);
    };

    /** @}*/
}

#pragma GCC visibility pop
