#pragma once

#include <MFELiveCommon.h>
#include <MFELiveMakeup/Models/MakeupLook.h>

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup MFEMakeupSDK
     *  @{
     */
    
    /**
     This class makeup rendering functionality for OpenGL.
     
     Important: instances of this class must be constructed and destroyed on your OpenGL thread.
     Also, MFE::MFEMakeupSDK_init must be called first.
     */
    class MakeupRendererSDK {
    public:
        /**
         Important: this constructor must be called on the thread that will be
         performing the OpenGL rendering. The OpenGL context must also be already set up on
         that thread before constructing an instance of this class.
         
         Upon constructing this object, the OpenGL state may be modified. See drawMakeupOntoCurrentFramebuffer
         for a list of OpenGL state that may be modified. You may change the OpenGL state back to their original
         values afterwards.
         */
        MakeupRendererSDK();
        
        /**
         Sets the current makeup look to apply.
         The default look is the empty look (no makeup).
         
         Important: this must be called on the rendering thread. The OpenGL state may be modified.
         See drawMakeupOntoCurrentFramebuffer for a list of OpenGL state that may be modified.
         You may change the OpenGL state back to their original values afterwards.
         */
        void setMakeupLook(MFE::MakeupLook makeupLook);
        
        /**
         Updates the drawer with the given face tracking data.
         This should be called every time new tracking data is recieved.
         
         Important: this must be called on the rendering thread. The OpenGL state may be modified.
         See drawMakeupOntoCurrentFramebuffer for a list of OpenGL state that may be modified.
         You may change the OpenGL state back to their original values afterwards.

         Important: this keeps a shallow copy of the tracking data to be used later.
         Make sure that the images and other variables in this tracking data can be persisted.
         */
        void updateWithTrackingData(const MFE::TrackingData &data);
        
        /**
         Draws the current makeup onto the provided OpenGL framebuffer.
         The drawing is performed on the current OpenGL context and thread.
         
         This function assumes that the size of the input framebuffer and texture is the same size
         as the image that corresponds with the provided MFE::TrackingData. Typically, this means
         that we are rendering in the same "image space" as the input frame (such as the camera frame)
         rather than "screen space". For example, if the camera input was 640x480, then typically we
         perform the detection on the camera input at the same size and perform all rendering (including makeup)
         at the same size (640x480). At the final stage, we render it to screen space (such as 1024x768) with
         some scaling and cropping. This is recommended if the camera image is smaller than the screen size,
         as it can significantly improve performance.
         
         Alternatively, it is also possible to render in screen space (performing detection on the original camera input,
         but rendering at screen space). In this case, you can provide the function with a transformation matrix that
         maps from the input frame "image space" to "screen space".
         
         It is also assumed that the premultiplied alpha is being used (although this
         should not matter in most cases, since the output is normally fully opaque).
         
         If no tracking data has yet been provided, or if the tracking data contains no faces,
         then no rendering is performed.
         
         Important: This function does change the current OpenGL state. In particular, the following
         OpenGL state may be changed after calling this function:
         
         glActiveTexture, bound textures, glBlendFunc, glClearColor, glDisable(GL_BLEND),
         bound GL_ARRAY_BUFFER, bound GL_ELEMENT_ARRAY_BUFFER, glViewport, GL_UNPACK_ALIGNMENT,
         active shader, bound GL_FRAMEBUFFER.
         
         You can set them back to their original values after calling this function.
         This function may also allocate new OpenGL buffers and textures.
         
         Important: You must disable any enabled vertex attribs when calling this function, by calling
         glDisableVertexAttribArray.
         
         @param framebufferID The OpenGL framebuffer ID of the framebuffer to draw onto.
         @param inputTextureID The OpenGL texture ID of the input image. This can be the same texture as the one attached to the current framebuffer, or a seperate texture. However, it must be the same dimensions as the texture attached to the framebuffer.
         @param width The width of the input texture in pixels
         @param height The height of the input texture in pixels
         @param transformMatrix A 3x3 transformation matrix (in row major form) to apply when rendering. This can be the identity matrix if no transformation is needed. See the above comments.
         */
        void drawMakeupOntoCurrentFramebuffer(unsigned int framebufferID, unsigned int inputTextureID, unsigned int width, unsigned int height, std::array<float, 9> transformMatrix = makeIdentityMatrix());
        
        /**
         Returns a 3x3 identity matrix, in row-major form.
         */
        static std::array<float, 9> makeIdentityMatrix();
        
    private:
        class MakeupRendererSDKImpl;
        std::shared_ptr<MakeupRendererSDKImpl> impl;
    };
    
    /** @}*/
}

#pragma GCC visibility pop
