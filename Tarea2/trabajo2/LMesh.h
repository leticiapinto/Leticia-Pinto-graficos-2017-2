
#pragma once

#include "LCommon.h"
#include "LVertexBuffer.h"
#include "LVertexArray.h"
#include "LIndexBuffer.h"

using namespace std;


namespace engine
{



    class LMesh
    {

        private :

        LVertexArray* m_vertexArray;
        LIndexBuffer* m_indexBuffer;

        
        public :

        vector<vec3> m_vertices;
        vector<vec3> m_normals;
        vec3 pos;
        //vec3 rot;
        mat4 m_rotMatrix;
        vec3 scale;
        void hi(int b);

        LMesh( const vector<vec3>& vertices, 
               const vector<vec3>& normals,
               const vector<LInd3>& indices );

        LMesh( const vector<vec3>& vertices, 
               const vector<vec3>& normals,
               const vector<LInd3>& indices,
               const vector<vec3>& colors 
            );

        ~LMesh();
        mat4 getModelMatrix() const;
        LVertexArray* getVertexArray() const { return m_vertexArray; }
        LIndexBuffer* getIndexBuffer() const { return m_indexBuffer; }

        void updateNormal(bool isSmoth) const;


    };

}
