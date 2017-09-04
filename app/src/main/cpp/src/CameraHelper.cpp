//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#include "CameraHelper.hpp"

/** Perform in-place transformation if dst mat has correct size.
 *  E.g. src is (10 x 5)...
 *  - If you want to ROTATE0_FLIP, then set dst to be (10 x 5) and in-place will be used.
 *  - If you want to ROTATE90_FLIP, then set dst to be (5 x 10) and in-place will be used. */
void correctOrientation(cv::Mat &src, cv::Mat &dst, int orientationCode){
    switch (orientationCode){
        case ROTATE0_FLIP:
            // Flip along X-axis
            cv::flip(src, dst, 0);
            break;
        case ROTATE90_NOFLIP:
            // Transpose then flip along X-axis
        	cv::transpose(src, dst);
            cv::flip(dst, dst, 0);
            break;
        case ROTATE90_FLIP:
            // Transpose
        	cv::transpose(src, dst);
            break;
        case ROTATE180_NOFLIP:
            // Flip along X-axis and Y-axis
            cv::flip(src, dst, -1);
            break;
        case ROTATE180_FLIP:
            // Flip along Y-axis
            cv::flip(src, dst, 1);
            break;
        case ROTATE270_NOFLIP:
            // Transpose then flip along Y-axis
        	cv::transpose(src, dst);
            cv::flip(dst, dst, 1);
            break;
        case ROTATE270_FLIP:
            // Transpose then flip along X-axis and Y-axis
        	cv::transpose(src, dst);
            cv::flip(dst, dst, -1);
            break;
        default:
            // No transpose, no flipping
            src.copyTo(dst);
            break;
    }
}

bool swapDimensions(int orientationCode) {
    switch (orientationCode){
        case ROTATE0_FLIP:
            return false;
        case ROTATE90_NOFLIP:
            return true;
        case ROTATE90_FLIP:
            return true;
        case ROTATE180_NOFLIP:
            return false;
        case ROTATE180_FLIP:
            return false;
        case ROTATE270_NOFLIP:
            return true;
        case ROTATE270_FLIP:
            return true;
        default:
            return false;
    }
}
