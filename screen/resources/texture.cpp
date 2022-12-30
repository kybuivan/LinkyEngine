#include "texture.h"
#include "drivers/gl_context/opengl_texture.h"

Ref<Texture2D> Texture2D::Create(const std::string&path)
{
    return CreateRef<OpenGLTexture2D>(path);
}

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
{
    return CreateRef<OpenGLTexture2D>(width, height);
}