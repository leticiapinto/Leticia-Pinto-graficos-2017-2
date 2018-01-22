
#pragma once

#include <GL/glew.h>


namespace graphics
{
    class Texture2D 
    {
        private:

        GLuint t_textureId;
        GLuint t_usage;
        GLuint t_width;
        GLuint t_height;
        unsigned char* t_data;

        public:
        Texture2D();
        Texture2D( GLuint usage);
        ~Texture2D(); 

        void setData2D(int width, int height, unsigned char* data);
        void bind2D();
        void unbind2D();    
    };
    
}