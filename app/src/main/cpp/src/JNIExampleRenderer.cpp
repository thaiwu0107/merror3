//
//  DemoSDK
//  Copyright © 2016 ModiFace Inc. All rights reserved.
//

#include "jni.h"
#include <android/native_window_jni.h>

#include "CameraHelper.hpp"

#include "MFEMakeupSDK.hpp"

#include <MakeupRendererSDK.hpp>
#include <FaceTrackerSDK.hpp>
#import "ExampleLooks.hpp"

#include "GLFramebuffer.hpp"
#include "GLProgram.hpp"
#include "GLTexture.hpp"

#include <GLES2/gl2.h>


#define PARAMS JNIEnv *env, jobject thiz

#define SHADER_STRING(text) #text

static const std::string kPassthroughFragmentShader = SHADER_STRING
(
        varying highp vec2 textureCoordinate;

        uniform sampler2D inputImageTexture;

        void main()
        {
            gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
        }
);

static const std::string kPassthroughVertexShader = SHADER_STRING
(
        attribute vec4 position;
        attribute vec4 inputTextureCoordinate;

        varying vec2 textureCoordinate;

        void main()
        {
            gl_Position = position;
            textureCoordinate = inputTextureCoordinate.xy;
        }
);

static const std::string kSplitVertexShader = SHADER_STRING
(
        attribute vec4 position;
        attribute vec4 inputTextureCoordinate;

        varying vec2 textureCoordinate;

        void main()
        {
            gl_Position = position;
            textureCoordinate = inputTextureCoordinate.xy;
        }
);

bool showSplit = false;

static const std::string kSplitFragmentShader = SHADER_STRING
(
        varying highp vec2 textureCoordinate;

        uniform sampler2D inputImageTexture;
        uniform sampler2D inputImageTexture2;

        uniform lowp float splitRatio;

        void main()
        {
            if (textureCoordinate.x > splitRatio){
                gl_FragColor = texture2D(inputImageTexture2, textureCoordinate);
            }
            else{
                gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
            }

        }
);

class RendererData {
public:
    std::shared_ptr<MFE::FaceTrackerSDK> tracker;
    std::shared_ptr<MFE::MakeupRendererSDK> makeupRenderer;
    std::string mfeAssetPath;
    unsigned int frameIndex = 0;
    std::shared_ptr<GLFramebuffer> workingFramebuffer = nullptr;
    std::shared_ptr<GLFramebuffer> workingFramebuffer2 = nullptr;
    std::shared_ptr<GLProgram> passthroughShader = nullptr;
    GLuint passthroughPositionAttribute = 0;
    GLuint passthroughTextureCoordinateAttribute = 0;
    GLuint passthroughInputTextureUniform = 0;
    std::shared_ptr<GLProgram> splitShader = nullptr;
    GLuint splitPositionAttribute = 0;
    GLuint splitTextureCoordinateAttribute = 0;
    GLuint splitInputTextureUniform = 0;
    GLuint splitInputTextureUniform2 = 0;
    GLuint splitSplitRatioUniform = 0;
    RendererData(std::shared_ptr<MFE::FaceTrackerSDK> tr, std::shared_ptr<MFE::MakeupRendererSDK> rend, std::string assetPath/*, ExampleLooks::Look look*/) :
            tracker(tr), makeupRenderer(rend), mfeAssetPath(assetPath)/*, currentLook(look)*/{

    }
};

/**
 * Retrieve a reference to the engine data from java
 */
RendererData* getRendererData(JNIEnv *env, jobject thiz) {
    jclass cls = env->GetObjectClass(thiz);
    jmethodID methodId = env->GetMethodID(cls, "getRendererData", "()J");
    jlong result = env->CallLongMethod(thiz, methodId);
    return (RendererData*) result;
}


/**
 * Creates a new Makeup Renderer
 * @return
 */
static std::shared_ptr<MFE::MakeupRendererSDK> CreateRenderer() {
    std::shared_ptr<MFE::MakeupRendererSDK> renderer = std::make_shared<MFE::MakeupRendererSDK>();
    return renderer;
}

static long JNICALL jniInit(PARAMS, jstring trackerResourcesPath, jstring mfeBundlePath, jobject context, jboolean isFaceTrackerResourceFull) {
    // must call this before using the MFEMakeupSDK
    MFE::MFEMakeupSDK_init(env, context);

    const char *resourcesPathChars = env->GetStringUTFChars(trackerResourcesPath,NULL);
    std::string resourcesPathStr = resourcesPathChars;
    auto faceTracker = std::make_shared<MFE::FaceTrackerSDK>();
    faceTracker->loadResources(resourcesPathStr, isFaceTrackerResourceFull? MFE::FaceTrackerSDK::ResourceSet::Full : MFE::FaceTrackerSDK::ResourceSet::Small);
    env->ReleaseStringUTFChars(trackerResourcesPath,resourcesPathChars);

    auto renderer = CreateRenderer();

    const char *mfeBundlePathChars = env->GetStringUTFChars(mfeBundlePath, NULL);
    std::string mfeBundlePathStr = mfeBundlePathChars;
    renderer->setMakeupLook(ExampleLooks::getLook(mfeBundlePathStr));
    env->ReleaseStringUTFChars(mfeBundlePath, mfeBundlePathChars);

    return (long) new RendererData(faceTracker, renderer, mfeBundlePathStr);
}

static void JNICALL jniConvertCameraFrame(PARAMS,
                                          jint cameraFrameWidth, jint cameraFrameHeight,
                                          jbyteArray cameraFrameData,
                                          jint cameraOrientationCode,
                                          jbyteArray outFrameData) {

    jbyte *pCameraFrameData = env->GetByteArrayElements(cameraFrameData, 0);
    jbyte *pOutFrameData = env->GetByteArrayElements(outFrameData, 0);

    // Input color format (from camera):
    // "myuv" is the color image in the camera's native NV21 YUV 420 "semi-planar" format, which means
    // the first part of the array is the grayscale pixel array, followed by a quarter-sized pixel
    // array that is the U & V color channels interleaved. So if we just want to access a grayscale
    // image, we can get it directly from the 1st part of a YUV420sp semi-planar image without any
    // conversions. But if we want a color image, then we must convert the color format using cvtColor().
    cv::Mat mYUV(cameraFrameHeight + cameraFrameHeight/2, cameraFrameWidth, CV_8UC1, (unsigned char *)pCameraFrameData);

    cv::Mat rgbFrame(cameraFrameHeight, cameraFrameWidth, CV_8UC3);

    cvtColor(mYUV, rgbFrame, CV_YUV420sp2RGB);

    bool outputDimensionSwapped = swapDimensions(cameraOrientationCode);
    int outputWidth = outputDimensionSwapped? cameraFrameHeight : cameraFrameWidth;
    int outputHeight = outputDimensionSwapped? cameraFrameWidth : cameraFrameHeight;

    cv::Mat outputImage(outputHeight, outputWidth, CV_8UC3, (unsigned char *)pOutFrameData);
    correctOrientation(rgbFrame, outputImage, cameraOrientationCode);

    env->ReleaseByteArrayElements(cameraFrameData, pCameraFrameData, JNI_ABORT);
    env->ReleaseByteArrayElements(outFrameData, pOutFrameData, JNI_ABORT);
}

static void JNICALL jniRenderFrame(PARAMS, jint frameWidth, jint frameHeight, jbyteArray frameData,
                                            jint displayWidth, jint displayHeight) {

    jbyte *pFrameData = env->GetByteArrayElements(frameData, 0);

    cv::Mat frame(frameHeight, frameWidth, CV_8UC3, (unsigned char *)pFrameData);

    // need to deep copy the frame data,
    // since the renderer kept the data in memory for later use (even after this function is done)
    cv::Mat inputImage;
    frame.copyTo(inputImage);

    RendererData* data = getRendererData(env, thiz);

    // check for existence of makeupRenderer too, since it may be destroyed
    if(data) {

        // reinitialize makeupRenderer if it has been destroyed
        if(!data->makeupRenderer) {
            data->makeupRenderer = CreateRenderer();

            MFE::MakeupLook look = ExampleLooks::getLook(data->mfeAssetPath);
            data->makeupRenderer->setMakeupLook(look);
        }

        GLTexture inputTexture(inputImage);

        // Create passthrough shader if it doesn't exist
        if(!data->passthroughShader) {
            // Create the passthrough shader
            data->passthroughShader = std::make_shared<GLProgram>(kPassthroughVertexShader, kPassthroughFragmentShader);
            data->passthroughShader->addAttribute("position");
            data->passthroughShader->addAttribute("inputTextureCoordinate");
            data->passthroughShader->link();

            data->passthroughPositionAttribute = data->passthroughShader->attributeIndex("position");
            data->passthroughTextureCoordinateAttribute = data->passthroughShader->attributeIndex("inputTextureCoordinate");
            data->passthroughInputTextureUniform = data->passthroughShader->uniformIndex("inputImageTexture");
        }

        if (!data->splitShader){
            data->splitShader = std::make_shared<GLProgram>(kSplitVertexShader, kSplitFragmentShader);
            data->splitShader->addAttribute("position");
            data->splitShader->addAttribute("inputTextureCoordinate");
            data->splitShader->link();

            data->splitPositionAttribute = data->splitShader->attributeIndex("position");
            data->splitTextureCoordinateAttribute = data->splitShader->attributeIndex("inputTextureCoordinate");
            data->splitInputTextureUniform = data->splitShader->uniformIndex("inputImageTexture");
            data->splitInputTextureUniform2 = data->splitShader->uniformIndex("inputImageTexture2");
            data->splitSplitRatioUniform = data->splitShader->uniformIndex("splitRatio");
        }

        // Create the framebuffer if it doesn't exist
        if (!data->workingFramebuffer) {
            data->workingFramebuffer = std::make_shared<GLFramebuffer>(inputImage.cols, inputImage.rows);
        }

        if (!data->workingFramebuffer2) {
            data->workingFramebuffer2 = std::make_shared<GLFramebuffer>(inputImage.cols, inputImage.rows);
        }

        // The input image should remain the same size on subsequent calls to renderWithInputImage
        assert(data->workingFramebuffer->getFramebufferTexture().getWidth() == inputImage.cols);
        assert(data->workingFramebuffer->getFramebufferTexture().getHeight() == inputImage.rows);


        // Detect the face
        // This is done on the same thread as the rendering in order ensure that the tracking is synchronized
        // with the frame. This detection can also be done on a different thread, but a more complex threading
        // design will be needed. The updateWithTrackingData must be called on the rendering thread however.
        {
            MFE::TrackingData trackingData;
            trackingData.hasFacePoints = data->tracker->track(inputImage, trackingData.facePoints);
            trackingData.image = inputImage;
            trackingData.isBGR = false;
            trackingData.frameID = data->frameIndex++;

            data->makeupRenderer->updateWithTrackingData(trackingData);
        }

        // Render the input picture to working framebuffer
        {
            data->workingFramebuffer->activate();

            // Draw into the working framebuffer
            glClearColor(1.0, 0, 0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw the working framebuffer into the output layer
            data->passthroughShader->use();

            static const GLfloat textureCoordinates[] = {
                    0.0f, 0.0f,
                    1.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
            };

            static const GLfloat vertices[] = {
                    -1.0f, -1.0f,
                    1.0f, -1.0f,
                    -1.0f,  1.0f,
                    1.0f,  1.0f,
            };

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, inputTexture.getTextureID());
            glUniform1i(data->passthroughInputTextureUniform, 4);

            glEnableVertexAttribArray(data->passthroughPositionAttribute);
            glEnableVertexAttribArray(data->passthroughTextureCoordinateAttribute);

            glVertexAttribPointer(data->passthroughPositionAttribute, 2, GL_FLOAT, 0, 0, vertices);
            glVertexAttribPointer(data->passthroughTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, textureCoordinates);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(data->passthroughPositionAttribute);
            glDisableVertexAttribArray(data->passthroughTextureCoordinateAttribute);
        }

        if (showSplit)
        {
            data->workingFramebuffer2->activate();

            // Draw into the working framebuffer
            glClearColor(1.0, 0, 0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw the working framebuffer into the output layer
            data->passthroughShader->use();

            static const GLfloat textureCoordinates[] = {
                    0.0f, 0.0f,
                    1.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
            };

            static const GLfloat vertices[] = {
                    -1.0f, -1.0f,
                    1.0f, -1.0f,
                    -1.0f,  1.0f,
                    1.0f,  1.0f,
            };

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, inputTexture.getTextureID());
            glUniform1i(data->passthroughInputTextureUniform, 5);

            glEnableVertexAttribArray(data->passthroughPositionAttribute);
            glEnableVertexAttribArray(data->passthroughTextureCoordinateAttribute);

            glVertexAttribPointer(data->passthroughPositionAttribute, 2, GL_FLOAT, 0, 0, vertices);
            glVertexAttribPointer(data->passthroughTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, textureCoordinates);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(data->passthroughPositionAttribute);
            glDisableVertexAttribArray(data->passthroughTextureCoordinateAttribute);
        }

        // Render the makeup onto the working buffer

        {
            data->workingFramebuffer->activate();

            std::array<float, 9> transform = {
                    1, 0, 0,
                    0, 1, 0,
                    0, 0, 1
            };

            const GLTexture &bufferTexture = data->workingFramebuffer->getFramebufferTexture();
            data->makeupRenderer->drawMakeupOntoCurrentFramebuffer(data->workingFramebuffer->getFramebufferID(), bufferTexture.getTextureID(), bufferTexture.getWidth(), bufferTexture.getHeight(), transform);
        }

        // Render to display (draw workingFramebuffer -> displayFramebuffer)
        {

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, displayWidth, displayHeight);

            // Clear it
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw the working framebuffer
//            data->passthroughShader->use();
            data->splitShader->use();

            static const GLfloat textureCoordinates[] = {
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    0.0f, 0.0f,
                    1.0f, 0.0f,
            };

            // Calculate vertices required for fill mode
            const GLTexture &workingFramebufferTexture = data->workingFramebuffer->getFramebufferTexture();

            float workingFramebufferAspect = (float)workingFramebufferTexture.getWidth() / workingFramebufferTexture.getHeight();
            float displayAspect = (float)displayWidth / displayHeight;

            float scale = 1.0;

            if (workingFramebufferAspect > displayAspect) {
                // Fit to height
                scale = (float)displayHeight / workingFramebufferTexture.getHeight();
            }
            else {
                // Fit to width
                scale = (float)displayWidth / workingFramebufferTexture.getWidth();
            }

            float offsetX = displayWidth / 2.0 - scale * workingFramebufferTexture.getWidth() / 2.0;
            float offsetY = displayHeight / 2.0 - scale * workingFramebufferTexture.getHeight() / 2.0;

            float vertexOffsetX = 2 * offsetX / displayWidth;
            float vertexOffsetY = 2 * offsetY / displayHeight;

            static const GLfloat vertices[] = {
                    -1.0f + vertexOffsetX, -1.0f + vertexOffsetY,
                    1.0f - vertexOffsetX, -1.0f + vertexOffsetY,
                    -1.0f + vertexOffsetX,  1.0f - vertexOffsetY,
                    1.0f - vertexOffsetX,  1.0f - vertexOffsetY,
            };

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, data->workingFramebuffer->getFramebufferTexture().getTextureID());
//            glUniform1i(data->passthroughInputTextureUniform, 4);
            glUniform1i(data->splitInputTextureUniform, 4);

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, data->workingFramebuffer2->getFramebufferTexture().getTextureID());
            glUniform1i(data->splitInputTextureUniform2, 5);

            if (showSplit){
                glUniform1f(data->splitSplitRatioUniform, 0.5f);
            } else{
                glUniform1f(data->splitSplitRatioUniform, 1.0f);
            }

            GLenum  error = glGetError();

//            glEnableVertexAttribArray(data->passthroughPositionAttribute);
//            glEnableVertexAttribArray(data->passthroughTextureCoordinateAttribute);
//
//            glVertexAttribPointer(data->passthroughPositionAttribute, 2, GL_FLOAT, 0, 0, vertices);
//            glVertexAttribPointer(data->passthroughTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, textureCoordinates);
//            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//            glDisableVertexAttribArray(data->passthroughPositionAttribute);
//            glDisableVertexAttribArray(data->passthroughTextureCoordinateAttribute);

            glEnableVertexAttribArray(data->splitPositionAttribute);
            glEnableVertexAttribArray(data->splitTextureCoordinateAttribute);

            glVertexAttribPointer(data->splitPositionAttribute, 2, GL_FLOAT, 0, 0, vertices);
            glVertexAttribPointer(data->splitTextureCoordinateAttribute, 2, GL_FLOAT, 0, 0, textureCoordinates);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(data->splitPositionAttribute);
            glDisableVertexAttribArray(data->splitTextureCoordinateAttribute);
        }
    }

    env->ReleaseByteArrayElements(frameData, pFrameData, JNI_ABORT);
}

static void JNICALL jniResetTracker(PARAMS) {
    RendererData* data = getRendererData(env, thiz);
    if(data) {
        data->tracker->reset();
    }
}

static void JNICALL jniDestroyRenderer(PARAMS) {
    RendererData* data = getRendererData(env, thiz);
    if(data) {
        data->makeupRenderer = nullptr;
        data->workingFramebuffer = nullptr;
        data->passthroughShader = nullptr;
        data->splitShader = nullptr;
        data->workingFramebuffer2 = nullptr;
    }
    showSplit = false;
}

MFE::MakeupLook setCurrentLook;

static void switchLook(PARAMS) {
    RendererData* data = getRendererData(env, thiz);
    if(data) {

        setCurrentLook = ExampleLooks::getLook(data->mfeAssetPath);
        if(data->makeupRenderer) data->makeupRenderer->setMakeupLook(setCurrentLook);
    }
}

static void JNICALL jniAddLips(PARAMS, int color, float opacity, float glitter, float gloss) {
    ExampleLooks::addLips(color,opacity,glitter,gloss);
    switchLook(env,thiz);
}

static void JNICALL jniRemoveLips(PARAMS) {
    ExampleLooks::removeLips();
    switchLook(env,thiz);
}

static void JNICALL jniAddFoundation(PARAMS, int color, float opacity, float smoothing) {
    ExampleLooks::addFoundation(color,opacity,smoothing);
    switchLook(env,thiz);
}

static void JNICALL jniRemoveFoundation(PARAMS) {
    ExampleLooks::removeFoundation();
    switchLook(env,thiz);
}

static void JNICALL jniAddEyeliner(PARAMS, int color, int style, float opacity){
    ExampleLooks::addEyeliner(color,style,opacity);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveEyeliner(PARAMS){
    ExampleLooks::removeEyeliner();
    switchLook(env,thiz);
}

static void JNICALL jniAddLash(PARAMS, int color, int style, float opacity){
    ExampleLooks::addLash(color,style,opacity);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveLash(PARAMS){
    ExampleLooks::removeLash();
    switchLook(env,thiz);
}

static void JNICALL jniAddBlush(PARAMS, int color, int style, float opacity){
    ExampleLooks::addBlush(color,style,opacity);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveBlush(PARAMS){
    ExampleLooks::removeBlush();
    switchLook(env,thiz);
}

static void JNICALL jniAddBrow(PARAMS, int color, float opacity){
    ExampleLooks::addBrow(color,opacity);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveBrow(PARAMS){
    ExampleLooks::removeBrow();
    switchLook(env,thiz);
}

static void JNICALL jniAddLipliner(PARAMS, int color, float opacity){
    ExampleLooks::addLipliner(color,opacity);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveLipliner(PARAMS){
    ExampleLooks::removeLipliner();
    switchLook(env,thiz);
}

static void JNICALL jniAddEyeshadowLid(PARAMS, int color, float opacity, float glitter){
    ExampleLooks::addEyeshadowLid(color,opacity,glitter);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveEyeshadowLid(PARAMS){
    ExampleLooks::removeEyeshadowLid();
    switchLook(env,thiz);
}

static void JNICALL jniAddEyeshadowCrease(PARAMS, int color, float opacity, float glitter){
    ExampleLooks::addEyeshadowCrease(color,opacity,glitter);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveEyeshadowCrease(PARAMS){
    ExampleLooks::removeEyeshadowCrease();
    switchLook(env,thiz);
}

static void JNICALL jniAddEyeshadowHigh(PARAMS, int color, float opacity, float glitter){
    ExampleLooks::addEyeshadowHigh(color,opacity,glitter);
    switchLook(env,thiz);
}
static void JNICALL jniRemoveEyeshadowHigh(PARAMS){
    ExampleLooks::removeEyeshadowHigh();
    switchLook(env,thiz);
}

static void JNICALL jniToggleSplit(PARAMS){
    if (showSplit){
        showSplit = false;
    } else {
        showSplit = true;
    }
}

extern "C" JNIEXPORT void JNICALL jniDestroy(PARAMS) {
    RendererData* data = getRendererData(env, thiz);
    if(data) {
        delete data;
    }
}

#define REG(x, p) {#x, p, (void*)&x},
static JNINativeMethod methods[] = {

        REG(jniInit, "(Ljava/lang/String;Ljava/lang/String;Landroid/content/Context;Z)J")
        REG(jniDestroy, "()V")

        REG(jniConvertCameraFrame, "(II[BI[B)V")
        REG(jniRenderFrame, "(II[BII)V")

        REG(jniResetTracker, "()V")

        REG(jniDestroyRenderer, "()V")

        REG(jniAddLips, "(IFFF)V")
        REG(jniRemoveLips, "()V")
        REG(jniAddFoundation, "(IFF)V")
        REG(jniRemoveFoundation, "()V")
        REG(jniAddEyeliner, "(IIF)V")
        REG(jniRemoveEyeliner, "()V")
        REG(jniAddLash, "(IIF)V")
        REG(jniRemoveLash, "()V")
        REG(jniAddBlush, "(IIF)V")
        REG(jniRemoveBlush, "()V")
        REG(jniAddBrow, "(IF)V")
        REG(jniRemoveBrow, "()V")
        REG(jniAddLipliner, "(IF)V")
        REG(jniRemoveLipliner, "()V")
        REG(jniAddEyeshadowLid, "(IFF)V")
        REG(jniRemoveEyeshadowLid, "()V")
        REG(jniAddEyeshadowCrease, "(IFF)V")
        REG(jniRemoveEyeshadowCrease, "()V")
        REG(jniAddEyeshadowHigh, "(IFF)V")
        REG(jniRemoveEyeshadowHigh, "()V")

        REG(jniToggleSplit, "()V")
};

extern "C" JNIEXPORT void JNICALL
Java_com_merrors_merror_ExampleRenderer_registerNatives(JNIEnv* env, jclass cls){
    int numMethods;
    static int registered = 0;

    if(registered)
        return;

    numMethods = sizeof(methods) / sizeof(methods[0]);
    int r;
    if ((r = env->RegisterNatives(cls, methods, numMethods)) < 0)
    {
    } else {
        registered = 1;
    }
}


