

#include "IndexBuffer.h"





namespace graphics
{


    IndexBuffer::IndexBuffer()
    {
        m_usage = GL_STATIC_DRAW;
        glGenBuffers( 1, &m_bufferId );
    }

    IndexBuffer::IndexBuffer( GLuint usage )
    {
        m_usage = usage;
        glGenBuffers( 1, &m_bufferId );
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferId );
    }

    void IndexBuffer::setData( GLuint size, GLuint count, GLuint* pData )
    {
        m_size = size;
        m_count = count;

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_bufferId );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, pData, m_usage );
    }

    void IndexBuffer::bind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_bufferId );
    }

    void IndexBuffer::unbind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }


}