package com.merrors.merror.app.utils;

import android.content.Context;
import android.hardware.Camera;
import android.util.DisplayMetrics;
import android.view.Surface;
import android.view.WindowManager;

import java.util.List;

/**
 Utility class for camera
 */
public class AndroidCameraUtils {

    /**
     * Look for size that matches aspect ratio and screen size the best.
     * */
    public static Camera.Size getOptimalPreviewSize(Context context, Camera.Parameters params, CameraOrientation orientation){
        int screenW;
        int screenH;
        int scale = 1;

        DisplayMetrics dm = new DisplayMetrics();
        WindowManager wmgr = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        if(wmgr != null) {
            wmgr.getDefaultDisplay().getMetrics(dm);
        }

        // We need to swap width and height for 90 and 270 rotations (ccw)
        if (orientation.swapSize()){
            screenW = dm.heightPixels/scale;
            screenH = dm.widthPixels/scale;
        } else {
            screenW = dm.widthPixels/scale;
            screenH = dm.heightPixels/scale;
        }

        // Limit size
        screenW = Math.min(640, screenW);
        screenH = Math.min(480, screenH);

        // We want to match screen aspect ratio as well
        double targetAspectRatio = (double)screenW / (double)screenH;

        // Best score is closest to zeros
        double bestScore = Double.MAX_VALUE;
        Camera.Size bestSize = null;

        List<Camera.Size> sizes = params.getSupportedPreviewSizes();
        for (Camera.Size size : sizes){
            double aspectRatio = (double)size.width / (double)size.height;

            // Compute score
            double score = Math.abs(aspectRatio - targetAspectRatio)/targetAspectRatio +
                    Math.abs((double)(size.width - screenW)/(double)screenW) +
                    Math.abs((double)(size.height - screenH)/(double)screenH);

            // Width and height must be multiple of 4 (to support ALPHA_8)
            boolean isMultiple = ((size.width % 4 == 0) && (size.height % 4 == 0));

            // Update best score
            if (isMultiple && score < bestScore){
                bestScore = score;
                bestSize = size;
            }
        }

        return bestSize;
    }

    /**
     Try to figure out camera orientation in a smart way
     http://developer.android.com/reference/android/hardware/Camera.html#setDisplayOrientation%28int%29
     */
    public static CameraOrientation GetCameraOrientation(Context context, boolean isCameraFrontFacing) {
        CameraOrientation cameraOrientation;
        try{
            final int r;
            int rotation = 0;
            WindowManager windowManager = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE));
            if(windowManager != null) {
                r = windowManager.getDefaultDisplay().getRotation();
                int surfaceRotation;
                switch (r) {
                    case Surface.ROTATION_0:
                        surfaceRotation = 0;
                        break;
                    case Surface.ROTATION_90:
                        surfaceRotation = 90;
                        break;
                    case Surface.ROTATION_180:
                        surfaceRotation = 180;
                        break;
                    default:
                        surfaceRotation = 270;
                        break;
                }
                rotation = surfaceRotation;
            }

            Camera.CameraInfo info;
            info = new Camera.CameraInfo();
            if(isCameraFrontFacing) {
                Camera.getCameraInfo(Camera.CameraInfo.CAMERA_FACING_FRONT, info);
            } else {
                Camera.getCameraInfo(Camera.CameraInfo.CAMERA_FACING_BACK, info);
            }

            int result;
            if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                result = (info.orientation + rotation) % 360;
                result = (360 - result) % 360;
            } else {  // back-facing
                result = (info.orientation - rotation + 360) % 360;
            }

            result = (result + 180) % 360;

            switch(result){
                case 0: cameraOrientation = isCameraFrontFacing ? CameraOrientation.ROTATE0_FLIP
                        : CameraOrientation.ROTATE0_NOFLIP; break;
                case 90: cameraOrientation = isCameraFrontFacing ? CameraOrientation.ROTATE90_FLIP
                        : CameraOrientation.ROTATE90_NOFLIP; break;
                case 180: cameraOrientation = isCameraFrontFacing ? CameraOrientation.ROTATE180_FLIP
                        : CameraOrientation.ROTATE180_NOFLIP; break;
                case 270: cameraOrientation = isCameraFrontFacing ? CameraOrientation.ROTATE270_FLIP
                        : CameraOrientation.ROTATE270_NOFLIP; break;
                default:
                    cameraOrientation = CameraOrientation.ROTATE270_FLIP; break;
            }

        }catch(Exception e){
            cameraOrientation = CameraOrientation.ROTATE270_FLIP;
        }
        return cameraOrientation;
    }

}