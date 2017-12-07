
#pragma once

#include "Common.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

using namespace std;


namespace graphics
{
    class Mesh
    {

        private :

        VertexArray* myVertexArray;
        IndexBuffer* myIndexBuffer;

        
        public :

        vector<vec3> myVertices;
        vector<vec3> myNormals;
        vec3 pos;
        //vec3 rot;
        mat4 myrotationMatrix;
        vec3 scale;
        void hi(int b);

        Mesh( const vector<vec3>& vertices, 
               const vector<vec3>& normals,
               const vector<Ind3>& indices );

        Mesh( const vector<vec3>& vertices, 
               const vector<vec3>& normals,
               const vector<Ind3>& indices,
               const vector<vec3>& colors 
            );

        ~Mesh();
        mat4 getModelMatrix() const;
        VertexArray* getVertexArray() const { return myVertexArray; }
        IndexBuffer* getIndexBuffer() const { return myIndexBuffer; }

        void ReCalcNormal(bool smoothing) const;


    };

}
