#include "GLProgram.hpp"

#include <iostream>

typedef void (*GLInfoFunction)(GLuint program, GLenum pname, GLint *params);

typedef void (*GLLogFunction)(GLuint program, GLsizei bufsize, GLsizei *length,
GLchar *infolog);

GLProgram::GLProgram(const std::string &vShaderString, const std::string &fShaderString) {
    initialized = false;
    
    program = glCreateProgram();
    
    vertShader = 0;
    fragShader = 0;
    
    // since compiling shader handles its own errors, we don't need to check the gl errors after
    // these two calls of compileShaders
    if (!compileShader(&vertShader, GL_VERTEX_SHADER, vShaderString)) {
        std::cout << "Failed to compile vertex shader" << std::endl;
        abort();
    }
    
    if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, fShaderString)) {
        std::cout << "Failed to compile fragment shader" << std::endl;
        abort();
    }
    
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
}

GLProgram::~GLProgram() {
    if (vertShader) {
        glDeleteShader(vertShader);
    }
    vertShader = 0;
    
    if (fragShader) {
        glDeleteShader(fragShader);
    }
    fragShader = 0;
    
    if (program) {
        glDeleteProgram(program);
    }
    program = 0;
}

bool GLProgram::compileShader(GLuint *shader, GLenum type, std::string shaderString) {
    
    GLint status;
    const GLchar *source;
    
    source = shaderString.c_str();
    if (!source) {
        std::cout << "Failed to load vertex shader" << std::endl;
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    
    if (status != GL_TRUE) {
        GLint logLength;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *) malloc(logLength);
            glGetShaderInfoLog(*shader, logLength, &logLength, log);
            if (shader == &vertShader) {
                vertexShaderLog = std::string(log);
            }
            else {
                fragmentShaderLog = std::string(log);
            }
            
            free(log);
        }
    }
    
    return status == GL_TRUE;
}

void GLProgram::addAttribute(const std::string &attributeName) {
    auto location = std::find(attributes.begin(), attributes.end(), attributeName);
    if (location == attributes.end()) {
        attributes.push_back(attributeName);
        glBindAttribLocation(program,
                             (GLuint) (attributes.size() - 1),
                             attributeName.c_str());
    }
}

GLuint GLProgram::attributeIndex(const std::string &attributeName) {
    auto iter = std::find(attributes.begin(), attributes.end(), attributeName);
    
    if (iter == attributes.end()) {
        return 0;
    }
    else {
        return (GLuint)std::distance(attributes.begin(), iter);
    }
}

GLuint GLProgram::uniformIndex(const std::string &uniformName) {
    GLint loc = glGetUniformLocation(program, uniformName.c_str());
    return loc;
}

bool GLProgram::link() {
    // since this has status checking, we don't need to check GL errors after link and
    // get program status
    GLint status;
    
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        return false;
    
    if (vertShader) {
        glDeleteShader(vertShader);
        vertShader = 0;
    }
    if (fragShader) {
        glDeleteShader(fragShader);
        fragShader = 0;
    }
    
    initialized = true;
    
    return true;
}

void GLProgram::use() {
    glUseProgram(program);
}

void GLProgram::validate() {
    // this is a validation, so don't check GL errors here
    GLint logLength;
    
    glValidateProgram(program);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *) malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        programLog = std::string(log);
        free(log);
    }
}
