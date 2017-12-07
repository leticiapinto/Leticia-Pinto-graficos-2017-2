

#include "VertexBuffer.h"



namespace graphics
{



    VertexBuffer::VertexBuffer()
    {
        m_usage = GL_STATIC_DRAW;
        glGenBuffers( 1, &m_bufferId );
    }

    VertexBuffer::VertexBuffer( GLuint usage )
    {
        m_usage = usage;
        glGenBuffers( 1, &m_bufferId );
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferId );
    }

    void VertexBuffer::setData( GLuint size, GLuint componentCount, GLfloat* pData )
    {
        m_componentCount = componentCount;
        glBindBuffer( GL_ARRAY_BUFFER, m_bufferId );
        glBufferData( GL_ARRAY_BUFFER, size, pData, m_usage );
    }

    void VertexBuffer::bind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_bufferId );
    }

    void VertexBuffer::unbind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }


}