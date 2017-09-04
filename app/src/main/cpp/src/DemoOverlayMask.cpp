//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#include "DemoOverlayMask.hpp"

DemoOverlayMask::DemoOverlayMask() {
    // just a solid rectangle mask
    dynamicMaskImage = cv::Mat(10, 10, CV_8UC1, cv::Scalar(255));
}

DemoOverlayMask::~DemoOverlayMask() {

}

void DemoOverlayMask::updateWithTrackingData(const MFE::TrackingData &data) {
    // setup where the vertices of the mask should be (in image coordinate
    MFE::BaseFacePoints points = data.facePoints;

    auto facePoints = points.face2D;
    auto foreheadPoints = points.forehead2D;

    float faceWidth = points.getFaceWidth();

    cv::Point2f bottomLeft(facePoints[0].x, foreheadPoints[1].y);
    cv::Point2f topLeft(bottomLeft.x, bottomLeft.y - faceWidth * 0.12f);
    cv::Point2f bottomRight(facePoints[0].x + faceWidth, bottomLeft.y);
    cv::Point2f topRight(bottomRight.x, topLeft.y);

    // set the vertices of the mask
    vertices[0] = bottomLeft.x;
    vertices[1] = bottomLeft.y;
    vertices[2] = topLeft.x;
    vertices[3] = topLeft.y;
    vertices[4] = bottomRight.x;
    vertices[5] = bottomRight.y;
    vertices[6] = topRight.x;
    vertices[7] = topRight.y;

}

std::shared_ptr<MFE::MakeupMask> DemoOverlayMask::clone() const {
    std::shared_ptr<DemoOverlayMask> copy = std::make_shared<DemoOverlayMask>(*this);
    return copy;
}







