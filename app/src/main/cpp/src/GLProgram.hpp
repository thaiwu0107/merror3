//
// A class that represents an OpenGL shader program.
// Used for demo purposes only.
//

#pragma once

#include <GLES2/gl2.h>
#include <string>
#include <vector>

class GLProgram {
public:
    /**
     * Constructor:
     * Creates a gl program, then compiles the shader strings in gl shaders,
     * then attaches the shaders to the gl program
     *
     * NOTE: This class can ONLY be created, destroyed and used in a gl thread
     */
    GLProgram(const std::string &vertexString, const std::string &fragmentString);
    
    /**
     * Destructor:
     * Delete the gl shaders from the gl program, then delete the gl program
     *
     * NOTE: This class can ONLY be created, destroyed and used in a gl thread
     */
    virtual ~GLProgram();
    
    /**
     * Checks if the attribute is already bound, if not, bind the attribute name to the program
     *
     * NOTE: This class can ONLY be created, destroyed and used in a gl thread
     */
    void addAttribute(const std::string &attributeName);
    
    /**
     * Returns the index of the vertex attribute specified by the attribute name
     *
     * NOTE: This class can ONLY be created, destroyed and used in a gl thread
     */
    GLuint attributeIndex(const std::string &attributeName);
    
    /**
     * Returns the location of the uniform variable specified by the uniform name
     *
     * NOTE: This class can ONLY be created, destroyed and used in a gl thread
     */
    GLuint uniformIndex(const std::string &uniformName);
    
    /**
     * Links the program, and if successful, delete the shaders from the program.
     * Returns true if program is successfully linked, otherwise returns false.
     */
    bool link();
    
    /**
     * Installs the gl program as part of current rendering state.
     */
    void use();
    
    /**
     * Checks to see if the gl program can be executed in the current opengl state.
     * This information is stored in the programLog variable
     */
    void validate();
    
    /**
     * True if the gl program is initialized, otherwise False.
     */
    bool initialized;
    
    /**
     * Information of the vertex shader after it has been compiled
     */
    std::string vertexShaderLog;
    
    /**
     * Information of the fragment shader after it has been compiled
     */
    std::string fragmentShaderLog;
    
    /**
     * Information of the gl program (see validate())
     */
    std::string programLog;
    
    // Make non-copyable
    GLProgram(const GLProgram&) = delete;
    GLProgram& operator=(const GLProgram&) = delete;
protected:
    std::vector<std::string> attributes;
    std::vector<std::string> uniforms;
    GLuint program, vertShader, fragShader;
    
    bool compileShader(GLuint *shader, GLenum type, std::string shaderString);
};
