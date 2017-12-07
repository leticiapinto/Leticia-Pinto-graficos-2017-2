

#include "Mesh.h"

using namespace std;

namespace graphics
{

    Mesh::Mesh( const vector<vec3>& vertices, 
                  const vector<vec3>& normals,
                  const vector<Ind3>& indices )
    {
        VertexBuffer* _vBuffer = new VertexBuffer();
        _vBuffer->setData( sizeof( vec3 ) * vertices.size(),
                           3, (GLfloat*) vertices.data() );

        VertexBuffer* _nBuffer = new VertexBuffer();
        _nBuffer->setData( sizeof( vec3 ) * normals.size(),
                           3, (GLfloat*) normals.data() );

        myVertexArray = new VertexArray();
        myVertexArray->addBuffer( _vBuffer, 0 );
        myVertexArray->addBuffer( _nBuffer, 1 );

        myIndexBuffer = new IndexBuffer();
        myIndexBuffer->setData( sizeof( Ind3 ) * indices.size(), 
                                3 * indices.size(), (GLuint*) indices.data() );

        scale = vec3( 1.0f, 1.0f, 1.0f );
        //m_rotMatrix = mat4(1.0f);
        myrotationMatrix = RotateY(0.0f);
    }

    Mesh::Mesh( const vector<vec3>& vertices, 
                  const vector<vec3>& normals,
                  const vector<Ind3>& indices, 
                  const vector<vec3>& colors )
    {

        VertexBuffer* _vBuffer = new VertexBuffer();
        _vBuffer->setData( sizeof( vec3 ) * vertices.size(),
                           3, (GLfloat*) vertices.data() );

        VertexBuffer* _nBuffer = new VertexBuffer();
        _nBuffer->setData( sizeof( vec3 ) * normals.size(),
                           3, (GLfloat*) normals.data() );

         VertexBuffer* _cBuffer = new VertexBuffer();
        _cBuffer->setData( sizeof( vec3 ) * colors.size(),
                           3, (GLfloat*) colors.data() );

        myVertexArray = new VertexArray();
        myVertexArray->addBuffer( _vBuffer, 0 );
        myVertexArray->addBuffer( _nBuffer, 1 );
        myVertexArray->addBuffer( _cBuffer, 2 );



        myIndexBuffer = new IndexBuffer();
        myIndexBuffer->setData( sizeof( Ind3 ) * indices.size(), 
                                3 * indices.size(), (GLuint*) indices.data() );


    
        scale = vec3( 1.0f, 1.0f, 1.0f );
        myrotationMatrix = RotateY(0.0f);
    }


    Mesh::~Mesh()
    {
        delete myVertexArray;
        delete myIndexBuffer;
    }

    mat4 Mesh::getModelMatrix() const
    {
        mat4 _model = mat4( 1.0f );

        _model = Scale( scale.x, scale.y, scale.z )*_model;
        _model = myrotationMatrix * _model;
        _model = Translate( pos.x, pos.y, pos.z )*_model;
        return _model;
    }

    void Mesh::ReCalcNormal(bool isSmoth) const
    {
        if(!isSmoth)
        {
            VertexBuffer* normalBuffer = new VertexBuffer();
            normalBuffer->setData( sizeof( vec3 ) * myNormals.size(),
                               3, (GLfloat*) myNormals.data() );
            myVertexArray->addBuffer( normalBuffer, 1 );
            return;
        }
        vector<vec3> myNewNormals;
        myNewNormals.resize(myVertices.size());


        for(int i=0; i<myVertices.size(); i++)
        {
            vec3 normalAverage=myNormals[i];
            int nNormals=0;
            for(int j=0; j<myVertices.size(); j++)
            {
                if(i!=j)
                {
                    if(myVertices[i].x==myVertices[j].x && myVertices[i].y == myVertices[j].y && myVertices[i].z == myVertices[j].z)
                    {
                        normalAverage = normalAverage + myNormals[j];
                        nNormals++;
                    }
                }
            }
            normalAverage = normalAverage/nNormals;
            myNewNormals[i] = normalAverage;
        }

        //hacemos bind  a las normales
        VertexBuffer* normalBuffer = new VertexBuffer();
        normalBuffer->setData( sizeof( vec3 ) * myNewNormals.size(),
                           3, (GLfloat*) myNewNormals.data() );
        myVertexArray->addBuffer( normalBuffer, 1 );

    }
}