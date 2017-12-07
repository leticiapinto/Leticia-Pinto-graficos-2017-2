

#include "VertexArray.h"




namespace graphics
{


    VertexArray::VertexArray()
    {
        glGenVertexArrays( 1, &m_vertexArrayId );
    }

    VertexArray::~VertexArray()
    {
        for ( VertexBuffer* pBuffer : m_vBuffers )
        {
            delete pBuffer;
        }

        m_vBuffers.clear();
    }

    void VertexArray::addBuffer( VertexBuffer* pBuffer, GLuint attribIndex )
    {
        m_vBuffers.push_back( pBuffer );

        bind();

        pBuffer->bind();

        glEnableVertexAttribArray( attribIndex );
        glVertexAttribPointer( attribIndex, pBuffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0 );

        pBuffer->unbind();

        unbind();
    }

    void VertexArray::bind()
    {
        glBindVertexArray( m_vertexArrayId );
    }

    void VertexArray::unbind()
    {
        glBindVertexArray( 0 );
    }

}