

#pragma once

#include <GL/glew.h>


namespace graphics
{

    // For now, just single vertex buffer per attribute
    // Can be generalized to handle one buffer with different ...
    // attributes and different layouts

    class VertexBuffer
    {

        private :

        GLuint m_bufferId;
        GLuint m_usage;
        GLuint m_componentCount;

        public :


        VertexBuffer();
        VertexBuffer( GLuint usage );

        ~VertexBuffer();

        void setData( GLuint size, GLuint componentCount, GLfloat* pData );
        void bind();
        void unbind();

        GLuint getComponentCount() { return m_componentCount; }
    };

}
