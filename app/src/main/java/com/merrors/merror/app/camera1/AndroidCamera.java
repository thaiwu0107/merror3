package com.merrors.merror.app.camera1;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Process;

import com.merrors.merror.app.Helper.CameraHelper;
import com.merrors.merror.app.utils.AndroidCameraUtils;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Helper class for starting and stopping camera on its own thread
 */
public class AndroidCamera {

    public interface CameraFailedToOpenListener {
        void onCameraFailedToOpen(Exception e);
    }

    public interface StartCameraCompletionHandler {
        void onCameraStarted(Camera camera);
    }

    private Camera mCamera = null;
    private final Object cameraLock = new Object();

    // indicate whether this camera should have started or not (regardless of whether the actual camera is started)
    private AtomicBoolean mCameraStarted = new AtomicBoolean(false);

    private Camera setAndGetCamera(Camera camera) {
        synchronized (cameraLock) {
            Camera pCam = mCamera;
            mCamera = camera;
            return pCam;
        }
    }

    private Camera getCamera() {
        synchronized (cameraLock) {
            return mCamera;
        }
    }

    private void setCamera(Camera camera) {
        synchronized (cameraLock) {
            mCamera = camera;
        }
    }

    /**
     * indicate whether this camera should have started or not (regardless of whether the actual camera is started)
     * @return true if this camera should have started, otherwise false.
     */
    public boolean shouldHaveStarted() {
        return mCameraStarted.get();
    }

    public void start(Context context, boolean useFrontFacingCamera, boolean throwRuntimeExceptionOnCameraStartFailed, CameraFailedToOpenListener listener, Camera.ErrorCallback cameraErrorCallback, StartCameraCompletionHandler completionHandler) {
        startCamera(context, useFrontFacingCamera, throwRuntimeExceptionOnCameraStartFailed, listener, cameraErrorCallback, completionHandler);
    }

    private void startCameraPrivate(Context context, boolean useFrontFacingCamera, boolean throwRuntimeExceptionOnCameraStartFailed,
                                    final CameraFailedToOpenListener listener, final Camera.ErrorCallback cameraErrorCallback, final StartCameraCompletionHandler completionHandler) {
        stopCameraPrivate();

        if(!shouldHaveStarted() || context == null) return;

        if(context.checkPermission(Manifest.permission.CAMERA, Process.myPid(), Process.myUid()) != PackageManager.PERMISSION_GRANTED) {
            if(listener != null) {
                listener.onCameraFailedToOpen(new SecurityException("CAMERA permission not granted"));
            }

            if(completionHandler != null) {
                completionHandler.onCameraStarted(null);
            }
            return;
        }

        if(!shouldHaveStarted()) return;

        CameraHelper mCameraHelper = new CameraHelper(context);

        if(!shouldHaveStarted()) return;

        // Open camera
        try {
            if (useFrontFacingCamera) {
                setCamera(mCameraHelper.openFrontCamera());
            } else {
                setCamera(mCameraHelper.openBackCamera());
            }
        }
        catch (final Exception e){
           if(listener != null) {
                listener.onCameraFailedToOpen(e);
            }

            if(completionHandler != null) {
                completionHandler.onCameraStarted(null);
            }
            e.printStackTrace();
            if(throwRuntimeExceptionOnCameraStartFailed) {
                throw new RuntimeException("Camera failed to open");
            }
            return;
        }

        if(!shouldHaveStarted()) return;


        final Camera camera = getCamera();
        // Configure camera and set it to GPUImage
        if (camera != null){
            camera.setErrorCallback(cameraErrorCallback);

            if(!shouldHaveStarted()) return;

            Camera.Parameters mCameraParameters = camera.getParameters();

            if(!shouldHaveStarted()) return;

            // Get best preview size
            Camera.Size bestSize =
                    AndroidCameraUtils.getOptimalPreviewSize(
                            context,
                            mCameraParameters,
                            AndroidCameraUtils.GetCameraOrientation(context, useFrontFacingCamera)
                    );

            if(!shouldHaveStarted()) return;

            // Set preview size to camera
            mCameraParameters.setPreviewSize(bestSize.width, bestSize.height);

            if(!shouldHaveStarted()) return;

            mCameraParameters.setPreviewFormat(ImageFormat.NV21);

            if(!shouldHaveStarted()) return;

            // Commit new setting
            camera.setParameters(mCameraParameters);

            if(!shouldHaveStarted()) return;

            final Camera.Size previewSize = camera.getParameters().getPreviewSize();

            if(!shouldHaveStarted()) return;

            // Need to add this hack because on the S2 the buffer is too small? Even though literally the previous commit the size was fine but now it throws an error.
            final byte[] buffer = new byte[(previewSize.width +50)* (previewSize.height +30)* ImageFormat.getBitsPerPixel(camera.getParameters().getPreviewFormat())/8];

            if(!shouldHaveStarted()) return;

            camera.addCallbackBuffer(buffer);

        }
        else {
            if (listener != null) {
                listener.onCameraFailedToOpen(new NullPointerException("camera null in startCamera"));
            }

            if(completionHandler != null) {
                completionHandler.onCameraStarted(null);
            }

            if(throwRuntimeExceptionOnCameraStartFailed) {
                throw new RuntimeException("Camera null");
            }
            return;
        }

        if(!shouldHaveStarted()) return;

        if(completionHandler != null)
            completionHandler.onCameraStarted(camera);

    }

    private void stopCameraPrivate() {
        final Camera camera = setAndGetCamera(null);
        if(camera != null) {
            camera.stopPreview();
            camera.setPreviewCallback(null);
            camera.release();
        }
    }

    public void close() {
        stopCamera();
    }


    private CameraHandlerThread mThread = null;
    private void startCamera(final Context context,
                               final boolean useFrontFacingCamera,
                               final boolean throwRuntimeExceptionOnCameraStartFailed,
                               final CameraFailedToOpenListener listener,
                               final Camera.ErrorCallback cameraErrorCallback,
                               final StartCameraCompletionHandler completionHandler) {
        if(mThread == null) {
            mThread = new CameraHandlerThread();
        }
        synchronized (mThread) {
            mThread.startCameraInThread(context, useFrontFacingCamera,
                    throwRuntimeExceptionOnCameraStartFailed, listener, cameraErrorCallback, completionHandler);
        }
    }

    public void stopCamera() {
        if(mThread == null) {
            mThread = new CameraHandlerThread();
        }
        synchronized (mThread) {
            mThread.stopCameraInThread();
        }
    }

    public void stopCameraSynchronizedOnUIThread() {
        if(mThread == null) {
            mThread = new CameraHandlerThread();
        }
        synchronized (mThread) {
            // need to set this flag so that the next time startCamera is called, it'll start properly
            mCameraStarted.getAndSet(false);

            // it appears that releasing the camera does some calls to the main thread, so we cannot
            //  block the main thread as it'll cause a deadlock. Thus, we just stop the camera directly.
            stopCameraPrivate();
        }
    }

    private class CameraHandlerThread extends HandlerThread {
        Handler mHandler = null;

        CameraHandlerThread() {
            super("CameraHandlerThread");
            start();
            mHandler = new Handler(getLooper());
        }

        void startCameraInThread(final Context context,
                         final boolean useFrontFacingCamera,
                         final boolean throwRuntimeExceptionOnCameraStartFailed,
                         final CameraFailedToOpenListener listener,
                         final Camera.ErrorCallback cameraErrorCallback,
                         final StartCameraCompletionHandler completionHandler) {
            mCameraStarted.getAndSet(true);
            mHandler.post(()->startCameraPrivate(context, useFrontFacingCamera,
                    throwRuntimeExceptionOnCameraStartFailed, listener, cameraErrorCallback, completionHandler));
        }

        void stopCameraInThread() {
            mCameraStarted.getAndSet(false);
            mHandler.post(AndroidCamera.this::stopCameraPrivate);
        }
    }
}