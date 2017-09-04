//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#pragma once

#include <opencv2/opencv.hpp>

enum{
    ROTATE0_NOFLIP      = 0,
    ROTATE0_FLIP        = 1,
    ROTATE90_NOFLIP     = 2,
    ROTATE90_FLIP       = 3,
    ROTATE180_NOFLIP    = 4,
    ROTATE180_FLIP      = 5,
    ROTATE270_NOFLIP    = 6,
    ROTATE270_FLIP      = 7
};

void correctOrientation(cv::Mat &src, cv::Mat &dst, int orientationCode);
bool swapDimensions(int orientationCode);
