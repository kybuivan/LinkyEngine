#include "opengl_texture.h"
#include <backends/imgui_impl_opengl3_loader.h>
#include <iostream>
#include <stb_image.h>

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    :m_Path(path)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(path.c_str(), &image_width, &image_height, NULL, 4);
    if (image_data)
    {
        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
    #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    #endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);
        m_RendererID = image_texture;
        m_Width = image_width;
        m_Height = image_height;
    }
    else
    {
        //Texture load failure!
    }
}

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    :m_Width(width), m_Height(height), m_Path("")
{
    GLenum internalFormat = GL_RGBA8;
    m_Format = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size)
{
    uint32_t channelNum = m_Format == GL_RGBA ? 4 : 3;
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}