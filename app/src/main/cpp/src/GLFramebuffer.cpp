#import "GLFramebuffer.hpp"

GLFramebuffer::GLFramebuffer(GLuint width, GLuint height) : framebufferTexture(width, height) {
    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    
    glBindTexture(GL_TEXTURE_2D, framebufferTexture.getTextureID());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture.getTextureID(), 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);
}

void GLFramebuffer::activate() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, framebufferTexture.getWidth(), framebufferTexture.getHeight());
}

GLFramebuffer::~GLFramebuffer() {
    glDeleteFramebuffers(1, &framebufferID);
}


