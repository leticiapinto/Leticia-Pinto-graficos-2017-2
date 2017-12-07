

#pragma once

#include <GL/glew.h>

namespace graphics
{

    class IndexBuffer
    {

        private :

        GLuint m_bufferId;
        GLuint m_usage;
        GLuint m_size;
        GLuint m_count;

        public :

        IndexBuffer();
        IndexBuffer( GLuint usage );

        ~IndexBuffer();

        void setData( GLuint size, GLuint count, GLuint* pData );
        void bind();
        void unbind();

        GLuint getCount() { return m_count; }

    };



}