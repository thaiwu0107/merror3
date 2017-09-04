package com.merrors.merror;

import android.content.Context;
import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.merrors.merror.app.DCode;
import com.merrors.merror.app.utils.CameraOrientation;

import java.io.File;
import java.lang.ref.WeakReference;
import java.nio.IntBuffer;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.atomic.AtomicBoolean;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * \addtogroup Java
 *   @{
 */

/**
 This class and its corresponding jni class (JNIExampleRenderer.cpp) provides an example of
 how to integrate the makeup rendering into an existing OpenGL pipeline.
 The example code is meant to be as short and simple as possible.
 */
public class ExampleRenderer implements GLSurfaceView.Renderer {

    static {
        System.loadLibrary("mfemakeupsdkdemo");
        registerNatives();
    }
    private long mNativeRendererData;

    // flag that indicates that on the next call to draw frame, we'll destroy the
    private AtomicBoolean mTurnOffRenderer = new AtomicBoolean(false);

    private final Queue<Runnable> mRunOnDrawStart = new LinkedList<>();
    private final Queue<Runnable> mRunOnDrawEnd = new LinkedList<>();

    // camera data
    private int mCurrentFrameWidth = 0, mCurrentFrameHeight = 0;
    private byte[] mCurrentFrameData = null;

    // surface dimensions
    protected int mOutputWidth = 0;
    protected int mOutputHeight = 0;

    // render thread state
    private AtomicBoolean mHasRenderThreadStarted = new AtomicBoolean(false);

    private final WeakReference<Context> mContextRef;

    public ExampleRenderer(Context context) {
        // save the context to be used later for native initialization
        mContextRef = new WeakReference<>(context);
    }

    public void toggleSplitCompare(){
        jniToggleSplit();
    }

    public void addLip(final int lipColor, final float lipOpacity, final float lipGlitter, final float lipGloss) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddLips(lipColor, lipOpacity, lipGlitter, lipGloss);
            }
        });
    }

    public void removeLip() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveLips();
            }
        });
    }

    public void addFoundation(final int color, final float opacity, final float smoothing) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddFoundation(color, opacity, smoothing);
            }
        });
    }

    public void removeFoundation() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveFoundation();
            }
        });
    }

    public void addEyeliner(final int color, final int style, final float opacity) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddEyeliner(color,style,opacity);
            }
        });
    }

    public void removeEyeliner() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveEyeliner();
            }
        });
    }


    public void addLash(final int color, final int style, final float opacity) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddLash(color,style,opacity);
            }
        });
    }

    public void removeLash() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveLash();
            }
        });
    }

    public void addBlush(final int color, final int style, final float opacity) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddBlush(color,style,opacity);
            }
        });
    }

    public void removeBlush() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveBlush();
            }
        });
    }

    public void addBrow(final int color, final float opacity) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddBrow(color,opacity);
            }
        });
    }

    public void removeBrow() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveBrow();
            }
        });
    }

    public void addLipliner(final int color, final float opacity) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddLipliner(color,opacity);
            }
        });
    }

    public void removeLipliner() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveLipliner();
            }
        });
    }

    public void addEyeshadowLid(final int color, final float opacity, final float glitter) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddEyeshadowLid(color,opacity,glitter);
            }
        });
    }

    public void removeEyeshadowLid() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveEyeshadowLid();
            }
        });
    }

    public void addEyeshadowCrease(final int color, final float opacity, final float glitter) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddEyeshadowCrease(color,opacity,glitter);
            }
        });
    }

    public void removeEyeshadowCrease() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveEyeshadowCrease();
            }
        });
    }

    public void addEyeshadowHigh(final int color, final float opacity, final float glitter) {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniAddEyeshadowHigh(color,opacity,glitter);
            }
        });
    }

    public void removeEyeshadowHigh() {
        postOnDrawStart(()->{
            if (mNativeRendererData != 0) {
                jniRemoveEyeshadowHigh();
            }
        });
    }

    private String getDirectoryPath(Context context, String dirName) {
        // no need to copy the assets, since it is assumed that they are already copied in SplashActivity
        File defaultSaveDirectory = context.getFilesDir();
        File location = new File(defaultSaveDirectory, dirName);
        return location.getAbsolutePath();
    }

    private String getTrackerResourcesPath(Context context) {
        return getDirectoryPath(context, DCode.AssetsConfig.TrackerSaveDir);
    }

    private String getMFEBundleResourcesPath(Context context) {
        return getDirectoryPath(context, DCode.AssetsConfig.MFEBundleSaveDir);
    }

    public void setTurnOffRenderer(boolean turnOffRenderer) {
        mTurnOffRenderer.set(turnOffRenderer);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        mHasRenderThreadStarted.set(true);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        mOutputWidth = width;
        mOutputHeight = height;
    }

    CaptureAfterPhotoListener captureAfterPhotoListener;

    public interface CaptureAfterPhotoListener {
        public void captureAfterPhoto(Bitmap bitmap);
    }
    public void setCaptureAfterPhotoListener(CaptureAfterPhotoListener captureAfterPhotoListener) {
        this.captureAfterPhotoListener = captureAfterPhotoListener;
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        if (mNativeRendererData == 0) {
            Context context = mContextRef.get();
            if(context != null) {
                mNativeRendererData = jniInit(getTrackerResourcesPath(context), getMFEBundleResourcesPath(context), context, DCode.AssetsConfig.kUseFullFaceTrackerResources);
            }
        }

        synchronized (mRunOnDrawStart) {
            while (!mRunOnDrawStart.isEmpty()) {
                mRunOnDrawStart.poll().run();
            }
        }

        int w = mOutputWidth;
        int h = mOutputHeight;

        if(mNativeRendererData != 0) {
            if(mTurnOffRenderer.get()) {
                jniDestroyRenderer();

                // we are not gonna draw anything on to screen since we are destroying the makeup makeupRenderer
                GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
            }
            else {
                if(mCurrentFrameData != null && mCurrentFrameWidth > 0 && mCurrentFrameHeight > 0) {

                    jniRenderFrame(mCurrentFrameWidth, mCurrentFrameHeight, mCurrentFrameData, mOutputWidth, mOutputHeight);

                    // reset the frame so we don't draw the same frame over and over again
                    mCurrentFrameData = null;
                    mCurrentFrameWidth = 0;
                    mCurrentFrameHeight = 0;
                }

            }
        }

        if (captureAfterPhotoListener != null) {
            CaptureAfterPhotoListener localCaptureListener = captureAfterPhotoListener;
            captureAfterPhotoListener = null;

            int b[]=new int[(w*h)];
            int bt[]=new int[(w*h)];
            IntBuffer buffer=IntBuffer.wrap(b);
            buffer.position(0);
            GLES20.glReadPixels(0, 0, w, h,GLES20.GL_RGBA,GLES20.GL_UNSIGNED_BYTE, buffer);
            for(int i=0; i<h; i++) {
                //convert openGL image to Android bitmap
                for(int j=0; j<w; j++) {
                    int pix=b[i*w+j];
                    int pb=(pix>>16)&0xff;
                    int pr=(pix<<16)&0x00ff0000;
                    int pix1=(pix&0xff00ff00) | pr | pb;
                    bt[(h-i-1)*w+j]=pix1;
                }
            }

            Bitmap inBitmap = Bitmap.createBitmap(bt, w, h, Bitmap.Config.ARGB_8888);
            localCaptureListener.captureAfterPhoto(inBitmap);
        }

        synchronized (mRunOnDrawEnd) {
            while (!mRunOnDrawEnd.isEmpty()) {
                mRunOnDrawEnd.poll().run();
            }
        }
    }

    /**
     This should be called when the surfaceview onPaused is called, since we don't have an
     onSurfaceDestroyed callback in the renderer interface
     */
    public void onSurfaceViewPaused() {
        mHasRenderThreadStarted.set(false);
    }

    /**
     @return whether the render thread has started
     */
    public boolean hasRenderThreadStarted() {
        return mHasRenderThreadStarted.get();
    }

    public void updateFrameWithCameraFrame(final int width, final int height, byte[] data, final CameraOrientation cameraOrientation) {
        final byte[] dataCopy = data.clone();
        postOnDrawStart(()->{
            if(mNativeRendererData != 0) {
                if(mCurrentFrameData == null || mCurrentFrameData.length != width * height * 3) {
                    mCurrentFrameData = new byte[width * height * 3];
                }

                boolean swapSize = cameraOrientation.swapSize();
                mCurrentFrameWidth =  swapSize? height : width;
                mCurrentFrameHeight = swapSize? width : height;

                jniConvertCameraFrame(width, height, dataCopy, cameraOrientation.getId(), mCurrentFrameData);
            }
        });
    }

    /**
     Run a runnable at the start of onDrawFrame (after jni init)
     @param runnable runnable to run
     */
    public void postOnDrawStart(Runnable runnable) {
        synchronized (mRunOnDrawStart) {
            mRunOnDrawStart.add(runnable);
        }
    }

    /**
     Run a runnable at the end of onDrawFrame
     @param runnable runnable to run
     */
    public void postOnDrawEnd(Runnable runnable) {
        synchronized (mRunOnDrawEnd) {
            mRunOnDrawEnd.add(runnable);
        }
    }

    @Override
    public void finalize() throws Throwable {
        close();
        super.finalize();
    }

    /**
     * Must call this when you want to destroy this class.
     */
    public void close() {
        if (mNativeRendererData != 0) {
            jniDestroy();
            mNativeRendererData = 0;
        }
    }

    public long getRendererData(){
        return mNativeRendererData;
    }

    public static native void registerNatives();

    private native long jniInit(String resourcePath, String bundlePath, Context context, boolean isFaceTrackerResourceSetFull);
    private native void jniDestroy();

    private native void jniConvertCameraFrame(int cameraFrameWidth, int cameraFrameHeight, byte[] cameraFrameData, int cameraOrientationCode, byte[] outputFrameData);

    // Must be called on the render thread
    private native void jniRenderFrame(int frameWidth, int frameHeight, byte[] frameData, int displayWidth, int displayHeight);

    private native void jniResetTracker();

    private native void jniDestroyRenderer();

    private native void jniAddLips(int color, float opacity, float glitter, float gloss);
    private native void jniRemoveLips();

    private native void jniAddFoundation(int color, float opacity, float smoothing);
    private native void jniRemoveFoundation();

    private native void jniAddEyeliner(int color, int style, float opacity);
    private native void jniRemoveEyeliner();

    private native void jniAddLash(int color, int style, float opacity);
    private native void jniRemoveLash();

    private native void jniAddBlush(int color, int style, float opacity);
    private native void jniRemoveBlush();

    private native void jniAddBrow(int color, float opacity);
    private native void jniRemoveBrow();

    private native void jniAddLipliner(int color, float opacity);
    private native void jniRemoveLipliner();

    private native void jniAddEyeshadowLid(int color, float opacity, float glitter);
    private native void jniRemoveEyeshadowLid();

    private native void jniAddEyeshadowCrease(int color, float opacity, float glitter);
    private native void jniRemoveEyeshadowCrease();

    private native void jniAddEyeshadowHigh(int color, float opacity, float glitter);
    private native void jniRemoveEyeshadowHigh();

    private native void jniToggleSplit();
}


/** @}*/
