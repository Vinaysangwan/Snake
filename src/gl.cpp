#include "pch.h"
#include "gl.h"
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// #############################################################################
//                           Constants
// #############################################################################
constexpr const char* VERT_FILE_PATH = "assets/shaders/quad.vert";
constexpr const char* FRAG_FILE_PATH = "assets/shaders/quad.frag";
constexpr const char* TEXTURE_ATLAS_PATH = "assets/textures/Texture_Atlas.png";

// #############################################################################
//                           Structs
// #############################################################################
struct GLRenderer
{
  GLuint programID;
  GLuint VAO;
  GLuint textureAtlasID;
};

// #############################################################################
//                           Statics
// #############################################################################
static GLRenderer glRenderer;

// #############################################################################
//                           Functions
// #############################################################################
GLuint create_shader_id(const char* shaderPath, GLenum type)
{
  const char* shaderSource = read_file(shaderPath);
  GLuint shaderID = glCreateShader(type);
  glShaderSource(shaderID, 1, &shaderSource, nullptr);
  glCompileShader(shaderID);

  int success;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if(success == GL_FALSE)
  {
    char infoLog[1024];
    glGetShaderInfoLog(shaderID, 1024, nullptr, &infoLog[0]);
    
    const char* shaderName = (type == GL_VERTEX_SHADER) ? "Vertex" : "fragment";

    SN_ERROR("Failed to Compile %s Shader:\n %s", shaderName, infoLog);
    return 0;
  }

  free((void*)shaderSource);
  return shaderID;
}

bool gl_init()
{
  // init programID
  {
    GLuint vertID = create_shader_id(VERT_FILE_PATH, GL_VERTEX_SHADER);
    GLuint fragID = create_shader_id(FRAG_FILE_PATH, GL_FRAGMENT_SHADER);
    if (vertID == 0 || fragID == 0)
    {
      return false;
    }

    glRenderer.programID = glCreateProgram();
    glAttachShader(glRenderer.programID, vertID);
    glAttachShader(glRenderer.programID, fragID);
    glLinkProgram(glRenderer.programID);

    int success;
    glGetProgramiv(glRenderer.programID, GL_LINK_STATUS, &success);
    if(success == GL_FALSE)
    {
      char infoLog[1024];
      glGetProgramInfoLog(glRenderer.programID, 1024, nullptr, &infoLog[0]);
      SN_ASSERT(false, "Failed to Link Program[%u]: %s", glRenderer.programID, infoLog);
      return false;
    }

    glDetachShader(glRenderer.programID, vertID);
    glDetachShader(glRenderer.programID, fragID);
    glDeleteShader(vertID);
    glDeleteShader(fragID);
  }

  // init textureID
  {
    glGenTextures(1, &glRenderer.textureAtlasID);  

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glRenderer.textureAtlasID);

    // Texture data
    int width, height, nChannels;
    unsigned char* data = stbi_load(TEXTURE_ATLAS_PATH, &width, &height, &nChannels, 4);
    if (!data)
    {
      SN_ASSERT(false, "Failed to open the texture atlas: %s", TEXTURE_ATLAS_PATH);
      return false;
    }

    // texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }
  
  // init VAO
  {
    glGenVertexArrays(1, &glRenderer.VAO);
    glBindVertexArray(glRenderer.VAO);
  }

  // use program
  glUseProgram(glRenderer.programID);

  return true;
}

void gl_render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void gl_cleanup()
{
  glBindVertexArray(0);
  glUseProgram(0);

  glDeleteVertexArrays(1, &glRenderer.VAO);
  glDeleteProgram(glRenderer.programID);
}
