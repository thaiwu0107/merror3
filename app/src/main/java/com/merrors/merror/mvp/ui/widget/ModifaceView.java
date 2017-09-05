package com.merrors.merror.mvp.ui.widget;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.ScriptIntrinsicYuvToRGB;
import android.renderscript.Type;
import android.util.AttributeSet;
import android.widget.Toast;

import com.merrors.merror.ExampleRenderer;
import com.merrors.merror.camera.views.MainActivity;
import com.merrors.merror.app.camera1.AndroidCamera;
import com.merrors.merror.app.utils.AndroidCameraUtils;

/**
 * Created by Jeff on 2017-08-08.
 */

public class ModifaceView extends GLSurfaceView implements Camera.ErrorCallback, AndroidCamera.CameraFailedToOpenListener, AndroidCamera.StartCameraCompletionHandler, Camera.PreviewCallback {

    // Load native libraries on application startup
    static {
        System.loadLibrary("mfemakeupsdkdemo");
    }

    // Renderer class that renders makeup
    ExampleRenderer mRenderer;

    // Camera class that handles all the camera initialization and destruction
    AndroidCamera mCamera = new AndroidCamera();

    public ModifaceView(Context context) {
        super(context);
        init();
    }

    public ModifaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public void init() {
        setEGLContextClientVersion(2);
        setRenderer(mRenderer = new ExampleRenderer(getContext()));
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    boolean usingFrontFacingCamera = true;

    public void startCamera() {
        startCamera(true);
    }

    public void startCamera(final boolean useFrontFacingCamera) {
        mRenderer.clearBitmap();
        usingFrontFacingCamera = useFrontFacingCamera;
        mCamera.start(getContext(), useFrontFacingCamera, false, ModifaceView.this, ModifaceView.this, ModifaceView.this);
    }

    private void stopCamera() {
        mCamera.stopCameraSynchronizedOnUIThread();

        // since the camera has stopped, we can free the surface texture
        mRenderer.postOnDrawEnd(()->{
            if(mSurfaceTexture != null) {
                mSurfaceTexture.release();
                mSurfaceTexture = null;
                GLES20.glDeleteTextures(1, textures, 0);
            }
        });
    }

    SurfaceTexture mSurfaceTexture = null;
    int[] textures = new int[1];
    @Override
    public void onCameraStarted(final Camera camera) {
        // create a surface texture to set in the camera in the renderer GL thread
        // and then start the camera preview
        mRenderer.postOnDrawStart(()->{
            if(mSurfaceTexture != null) {
                mSurfaceTexture.release();
                mSurfaceTexture = null;
                GLES20.glDeleteTextures(1, textures, 0);
            }

            GLES20.glGenTextures(1, textures, 0);
            mSurfaceTexture = new SurfaceTexture(textures[0]);
            try {
                camera.setPreviewTexture(mSurfaceTexture);
                camera.setPreviewCallbackWithBuffer(ModifaceView.this);
                camera.startPreview();
            } catch (Exception e){
                e.printStackTrace();
            }
        });

        // request render to make sure that the above runnable is run.
        requestRender();
    }

    CaptureBeforePhotoListener captureBeforePhotoListener;

    public void toggleSplitCompare() {
        mRenderer.toggleSplitCompare();
        requestRender();
    }

    public interface CaptureBeforePhotoListener {
        public void captureBeforePhoto(Bitmap bitmap);
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {

        Camera.Size size = camera.getParameters().getPreviewSize();

        if (captureBeforePhotoListener != null) {

            Bitmap bitmap = Bitmap.createBitmap(size.width, size.height, Bitmap.Config.ARGB_8888);
            Allocation bmData = renderScriptNV21ToRGBA888(getContext(), size.width, size.height, data);
            bmData.copyTo(bitmap);

            bitmap = adjustBitmapOrientation(bitmap);

            captureBeforePhotoListener.captureBeforePhoto(bitmap);
            captureBeforePhotoListener = null;

        }

        // call render to the preview frame
        mRenderer.updateFrameWithCameraFrame(size.width, size.height, data, AndroidCameraUtils.GetCameraOrientation(getContext(), usingFrontFacingCamera));

        // request render to make sure that the rendering is performed.
        requestRender();

        camera.addCallbackBuffer(data);
    }

    /**
     * Adjusts the bitmap based on CameraOrientation
     * (Not sure if this is the best way and best place to do this)
     * @param bitmap from PreviewFrame
     * @return Adjusted bitmap with correct orientation and rotation
     */
    private Bitmap adjustBitmapOrientation(Bitmap bitmap) {
        Matrix matrix = new Matrix();
        //Flip in x if usingFrontCamera
        if (usingFrontFacingCamera) {
            matrix.postScale(-1, 1, bitmap.getWidth()/2f, bitmap.getWidth()/2f);
        }
        switch (AndroidCameraUtils.GetCameraOrientation(getContext(), usingFrontFacingCamera)){
            case ROTATE270_NOFLIP:
                matrix.postRotate(-270);
                break;
            case ROTATE270_FLIP:
                matrix.postRotate(-270);
                break;
            case ROTATE180_NOFLIP:
                matrix.postRotate(-180);
                break;
            case ROTATE180_FLIP:
                matrix.postRotate(-180);
                break;
            case ROTATE90_NOFLIP:
                matrix.postRotate(-90);
                break;
            case ROTATE90_FLIP:
                matrix.postRotate(-90);
                break;
            default:
                break;
        }

        bitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);

        return bitmap;
    }

    private Allocation renderScriptNV21ToRGBA888(Context context, int width, int height, byte[] nv21) {
        RenderScript rs = RenderScript.create(context);
        ScriptIntrinsicYuvToRGB yuvToRgbIntrinsic = ScriptIntrinsicYuvToRGB.create(rs, Element.U8_4(rs));

        Type.Builder yuvType = new Type.Builder(rs, Element.U8(rs)).setX(nv21.length);
        Allocation in = Allocation.createTyped(rs, yuvType.create(), Allocation.USAGE_SCRIPT);

        Type.Builder rgbaType = new Type.Builder(rs, Element.RGBA_8888(rs)).setX(width).setY(height);
        Allocation out = Allocation.createTyped(rs, rgbaType.create(), Allocation.USAGE_SCRIPT);

        in.copyFrom(nv21);

        yuvToRgbIntrinsic.setInput(in);
        yuvToRgbIntrinsic.forEach(out);
        return out;
    }

    /**
     Variables for final draw command in makeupRenderer
     */
    volatile boolean mLastDrawAlreadyFinished;
    final Object mWaitForLastDrawToFinish = new Object();

    @Override
    public void onPause(){
        // first stop the camera, this order is such that the destruction of the surface texture
        // for the camera will be done on the gl thread before pausing the gl thread
        stopCamera();

        // make sure that the makeup renderer is destroyed, since we don't want any gl stuff to remain
        mRenderer.setTurnOffRenderer(true);

        // only do the synchronous waiting for the end of render thread by
        if(mRenderer.hasRenderThreadStarted()) {
            /*
             Flush all commands and destroy the makeupRenderer in gl thread first before pausing.
             */

            // Wait to pause FaceTrackingView after the final render is completed.
            mLastDrawAlreadyFinished = false;
            mRenderer.postOnDrawEnd(()->{
                synchronized (mWaitForLastDrawToFinish) {
                    mLastDrawAlreadyFinished = true;
                    mWaitForLastDrawToFinish.notify();
                }
            });

            // do a request render before pausing to clear the makeupRenderer
            requestRender();

            synchronized (mWaitForLastDrawToFinish) {
                try {
                    if (!mLastDrawAlreadyFinished) mWaitForLastDrawToFinish.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        // notify renderer that the surface view (thus the render thread) is going to be paused
        mRenderer.onSurfaceViewPaused();

        // pause the GLSurfaceView
        super.onPause();
    }

    //todo 這裡可以輸入需要的特定的圖案
    public void startStatic(Bitmap bitmap) {
        mCamera.stopCamera();
        mRenderer.updateFrameWithBitmap(bitmap);
        requestRender();
    }

    @Override
    public void onResume(){

        // resume the GLSurfaceView
        super.onResume();

        // turn the makeupRenderer back on
        mRenderer.setTurnOffRenderer(false);

        // render screen, which also does setup for anything we've destroyed in onPause()
        requestRender();

        // start camera if we can
        if(Build.VERSION.SDK_INT < Build.VERSION_CODES.M || MainActivity.isAllPermissionsGranted(getContext())) {
            if (mRenderer.getBitmap() == null) startCamera();
        }
    }

    @Override
    public void onError(final int error, Camera camera) {
        this.post(()->Toast.makeText(getContext(), "Camera error: " + error, Toast.LENGTH_SHORT).show());
    }

    @Override
    public void onCameraFailedToOpen(final Exception e) {
        this.post(()->Toast.makeText(getContext(), "Camera failed to open exception: " + (e != null? e.getMessage() : "null"), Toast.LENGTH_SHORT).show());
    }

    public void removeLip() {
        mRenderer.removeLip();
        requestRender();
    }

    public void addLip(int lipColor, float lipOpacity, float lipGlitter, float lipGloss) {
        mRenderer.addLip(lipColor,lipOpacity,lipGlitter,lipGloss);
        requestRender();
    }

    public void removeFoundation() {
        mRenderer.removeFoundation();
        requestRender();
    }

    public void addFoundation(int color, float opacity, float smoothing) {
        mRenderer.addFoundation(color, opacity, smoothing);
        requestRender();
    }

    public void addEyeliner(int color, int style, float opacity){
        mRenderer.addEyeliner(color,style,opacity);
        requestRender();
    }
    public void removeEyeliner(){
        mRenderer.removeEyeliner();
        requestRender();
    }

    public void addLash(int color, int style, float opacity){
        mRenderer.addLash(color,style,opacity);
        requestRender();
    }
    public void removeLash(){
        mRenderer.removeLash();
        requestRender();
    }

    public void addBlush(int color, int style, float opacity){
        mRenderer.addBlush(color,style,opacity);
        requestRender();
    }
    public void removeBlush(){
        mRenderer.removeBlush();
        requestRender();
    }

    public void addBrow(int color, float opacity){
        mRenderer.addBrow(color,opacity);
        requestRender();
    }
    public void removeBrow(){
        mRenderer.removeBrow();
        requestRender();
    }

    public void addLipliner(int color, float opacity){
        mRenderer.addLipliner(color,opacity);
        requestRender();
    }
    public void removeLipliner(){
        mRenderer.removeLipliner();
        requestRender();
    }

    public void addEyeshadowLid(int color, float opacity, float glitter){
        mRenderer.addEyeshadowLid(color,opacity,glitter);
        requestRender();
    }
    public void removeEyeshadowLid(){
        mRenderer.removeEyeshadowLid();
        requestRender();
    }

    public void addEyeshadowCrease(int color, float opacity, float glitter){
        mRenderer.addEyeshadowCrease(color,opacity,glitter);
        requestRender();
    }
    public void removeEyeshadowCrease(){
        mRenderer.removeEyeshadowCrease();
        requestRender();
    }

    public void addEyeshadowHigh(int color, float opacity, float glitter){
        mRenderer.addEyeshadowHigh(color,opacity,glitter);
        requestRender();
    }
    public void removeEyeshadowHigh(){
        mRenderer.removeEyeshadowHigh();
        requestRender();
    }


    public void captureBeforeBitmap(CaptureBeforePhotoListener captureBeforePhotoListener) {
        Bitmap staticBitmap = mRenderer.getBitmap();
        if (staticBitmap != null) {
            captureBeforePhotoListener.captureBeforePhoto(staticBitmap);
        } else {
            this.captureBeforePhotoListener = captureBeforePhotoListener;
            requestRender();
        }
    }
    public void captureAfterBitmap(ExampleRenderer.CaptureAfterPhotoListener captureListener) {
        mRenderer.setCaptureAfterPhotoListener(captureListener);
        requestRender();
    }
}
