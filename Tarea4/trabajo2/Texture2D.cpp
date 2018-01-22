
#include "Texture2D.h"


namespace graphics
{

	//texture
    Texture2D::Texture2D()
    {
        t_usage = GL_REPEAT;
        glGenTextures(1, &t_textureId);
    }

    Texture2D::Texture2D(GLuint usage )
    {
        t_usage = usage;
        glGenTextures(1, &t_textureId);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &t_textureId);
    }
    
    void Texture2D::bind2D()
    {
        glBindTexture(GL_TEXTURE_2D, t_textureId);
    }
    void Texture2D::unbind2D()
    {   
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
