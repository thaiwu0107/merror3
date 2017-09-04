//
// A class that represents an OpenGL texture.
// Used for demo purposes only.
//

#pragma once

#include <GLES2/gl2.h>
#include <opencv2/opencv.hpp>

class GLTexture {
public:
    GLTexture(cv::Mat image);
    GLTexture(GLuint width, GLuint height);
    ~GLTexture();
    
    GLuint getTextureID() const { return textureID; }
    GLuint getWidth() const { return width; };
    GLuint getHeight() const { return height; };

    // Make non-copyable
    GLTexture(const GLTexture&) = delete;
    GLTexture& operator=(const GLTexture&) = delete;
private:
    GLuint textureID;
    GLuint width, height;
};
