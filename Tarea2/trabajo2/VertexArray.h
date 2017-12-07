

#pragma once

#include <GL/glew.h>
#include <vector>

#include "VertexBuffer.h"

using namespace std;

namespace graphics
{


    class VertexArray
    {

        private :

        GLuint m_vertexArrayId;

        vector<VertexBuffer*> m_vBuffers;

        public :

        VertexArray();
        ~VertexArray();

        void addBuffer( VertexBuffer* pBuffer, GLuint attribIndex );
        void bind();
        void unbind();


    };





}