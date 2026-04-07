#include "pch.h"
#include "gl.h"
#include "utils.h"
#include "renderer.h"
#include "config.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// #############################################################################
//                           Constants
// #############################################################################
constexpr const char* VERT_FILE_PATH = "assets/shaders/quad.vert";
constexpr const char* FRAG_FILE_PATH = "assets/shaders/quad.frag";
constexpr const char* TEXTURE_ATLAS_PATH = "assets/textures/Texture_Atlas.png";
constexpr const char* FONT_FILE_PATH = "assets/fonts/04B_30.ttf";

// #############################################################################
//                           Structs
// #############################################################################
struct GLRenderer
{
  GLuint programID;
  GLuint VAO;
  GLuint textureAtlasID;
  GLuint transformSSBOID;
  GLuint fontAtlasID;

  // uniform locations
  GLint location_projectionMatrix;
  GLint location_viewMatrix;
};

// #############################################################################
//                           Statics
// #############################################################################
static GLRenderer glRenderer;

// #############################################################################
//                           Internal Functions
// #############################################################################
GLint get_uniform_location(GLuint programID, const char* name)
{
  GLint location = glGetUniformLocation(programID, name);
  if (location < 0)
  {
    SN_ERROR("Failed to Get Uniform Location: %s in Shader Program[%u]", name, programID);
  }

  return location;
}

GLuint create_shader_id(const char* shaderPath, GLenum type)
{
  const char* sharedSource = read_file("src/shared_header.h");
  const char* shaderSource = read_file(shaderPath);
  const char* code[]
  {
    "#version 430 core\r\n",
    sharedSource,
    shaderSource
  };

  // for (int i=0; i<3; ++i)
  // {
  //   printf("%s", code[i]);
  // }

  // printf("\n");

  GLuint shaderID = glCreateShader(type);
  glShaderSource(shaderID, sizeof(code) / sizeof(code[0]), code, nullptr);
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
  free((void*)sharedSource);
  return shaderID;
}

bool load_font(const char* fontFilePath, int fontSize)
{
  // init library
  FT_Library ftLib;
  if (FT_Init_FreeType(&ftLib))
  {
    SN_ERROR("Failed to Init Free type Lib");
    return false;
  }

  // init face
  FT_Face face;
  if (FT_New_Face(ftLib, fontFilePath, 0, &face))
  {
    SN_ERROR("Failed to open the font file Path: %s", FONT_FILE_PATH);
    return false;
  }
  FT_Set_Pixel_Sizes(face, 0, fontSize);

  int padding = 2;
  int row = 0;
  int col = padding;

  const int TEXTURE_WIDTH = 512;
  char textureBuffer[TEXTURE_WIDTH * TEXTURE_WIDTH];
  for (FT_ULong glyphIdx = 0; glyphIdx < 217; ++glyphIdx)
  {
    FT_UInt glyphIndex = FT_Get_Char_Index(face, glyphIdx);
    FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
    {
      SN_ERROR("Failed to load Glyph render: %d", glyphIdx);
      return false;
    }

    if (col + face->glyph->bitmap.width + padding >= 512)
    {
      col = padding;
      row += fontSize;
    }

    // Font Height
    renderState.fontHeight = 
      max(int(face->size->metrics.ascender - face->size->metrics.descender) >> 6, 
          renderState.fontHeight);

    for (unsigned int y = 0; y < face->glyph->bitmap.rows; ++y)
    {
      for (unsigned int x = 0; x < face->glyph->bitmap.width; ++x)
      {
        textureBuffer[(row + y) * TEXTURE_WIDTH + col + x] =
            face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x];
      }
    }

    // Glyphs
    Glyph& glyph = renderState.glyphs[glyphIdx];
    glyph.textureCoords = {col, row};
    glyph.size = 
    { 
      (int)face->glyph->bitmap.width, 
      (int)face->glyph->bitmap.rows
    };
    glyph.advance = 
    {
      (float)(face->glyph->advance.x >> 6), 
      (float)(face->glyph->advance.y >> 6)
    };
    glyph.offset =
    {
      (float)face->glyph->bitmap_left,
      (float)face->glyph->bitmap_top,
    };

    col += face->glyph->bitmap.width + padding;
  }

  // init font textureID
  {
    glGenTextures(1, &glRenderer.fontAtlasID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, glRenderer.fontAtlasID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEXTURE_WIDTH, TEXTURE_WIDTH, 0, GL_RED, GL_UNSIGNED_BYTE, (char*)textureBuffer);
  }

  return true;
}

// #############################################################################
//                           Functions
// #############################################################################
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

  // init VAO
  {
    glGenVertexArrays(1, &glRenderer.VAO);
    glBindVertexArray(glRenderer.VAO);
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
  
  // Init transformSSBOID
  {
    glGenBuffers(1, &glRenderer.transformSSBOID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glRenderer.transformSSBOID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transform) * MAX_TRANSFORM_COUNT, nullptr, GL_DYNAMIC_DRAW);
  }

  // load fonts
  {
    if(!load_font(FONT_FILE_PATH, 24))
    {
      return false;
    }
  }

  // Set Uniform Locations
  {
    glRenderer.location_projectionMatrix = get_uniform_location(glRenderer.programID, "uProjectionMatrix");
    glRenderer.location_viewMatrix = get_uniform_location(glRenderer.programID, "uViewMatrix");
  }

  // Set Unifom Data
  {
    glUseProgram(glRenderer.programID);

    Mat4 projectionMatrix = projection_orthographic_matrix(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glUniformMatrix4fv(glRenderer.location_projectionMatrix, 1, GL_FALSE, &projectionMatrix[0]);
    
    glUseProgram(0);
  }

  // use program
  glUseProgram(glRenderer.programID);

  return true;
}

void gl_render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Send View Matrix to GPU
  {
    Mat4 viewMatrix = view_matrix(renderState.gameCamera.pos, renderState.gameCamera.size, renderState.gameCamera.rot, WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniformMatrix4fv(glRenderer.location_viewMatrix, 1, GL_FALSE, &viewMatrix[0]);
  }

  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Transform) * renderState.transformCount, &renderState.transforms[0]);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderState.transformCount);
  renderState.transformCount = 0;
}

void gl_cleanup()
{
  glBindVertexArray(0);
  glUseProgram(0);

  glDeleteVertexArrays(1, &glRenderer.VAO);
  glDeleteProgram(glRenderer.programID);
}
