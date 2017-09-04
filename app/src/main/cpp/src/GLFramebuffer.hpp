//
// A class that represents an OpenGL framebuffer.
// Used for demo purposes only.
//

#pragma once

#include "GLTexture.hpp"

class GLFramebuffer {
public:
    GLFramebuffer(GLuint width, GLuint height);
    ~GLFramebuffer();
    
    GLuint getFramebufferID() const { return framebufferID; };
    const GLTexture &getFramebufferTexture() const { return framebufferTexture; };
    
    void activate() const;
    
    // Make non-copyable
    GLFramebuffer(const GLFramebuffer&) = delete;
    GLFramebuffer& operator=(const GLFramebuffer&) = delete;
private:
    GLuint framebufferID;
    GLTexture framebufferTexture;
};
