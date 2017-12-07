

#include "LMesh.h"

using namespace std;

namespace engine
{

    LMesh::LMesh( const vector<vec3>& vertices, 
                  const vector<vec3>& normals,
                  const vector<LInd3>& indices )
    {
        LVertexBuffer* _vBuffer = new LVertexBuffer();
        _vBuffer->setData( sizeof( vec3 ) * vertices.size(),
                           3, (GLfloat*) vertices.data() );

        LVertexBuffer* _nBuffer = new LVertexBuffer();
        _nBuffer->setData( sizeof( vec3 ) * normals.size(),
                           3, (GLfloat*) normals.data() );

        m_vertexArray = new LVertexArray();
        m_vertexArray->addBuffer( _vBuffer, 0 );
        m_vertexArray->addBuffer( _nBuffer, 1 );

        m_indexBuffer = new LIndexBuffer();
        m_indexBuffer->setData( sizeof( LInd3 ) * indices.size(), 
                                3 * indices.size(), (GLuint*) indices.data() );

        scale = vec3( 1.0f, 1.0f, 1.0f );
        //m_rotMatrix = mat4(1.0f);
        m_rotMatrix = RotateY(0.0f);
    }

    LMesh::LMesh( const vector<vec3>& vertices, 
                  const vector<vec3>& normals,
                  const vector<LInd3>& indices, 
                  const vector<vec3>& colors )
    {

        LVertexBuffer* _vBuffer = new LVertexBuffer();
        _vBuffer->setData( sizeof( vec3 ) * vertices.size(),
                           3, (GLfloat*) vertices.data() );

        LVertexBuffer* _nBuffer = new LVertexBuffer();
        _nBuffer->setData( sizeof( vec3 ) * normals.size(),
                           3, (GLfloat*) normals.data() );

         LVertexBuffer* _cBuffer = new LVertexBuffer();
        _cBuffer->setData( sizeof( vec3 ) * colors.size(),
                           3, (GLfloat*) colors.data() );

        m_vertexArray = new LVertexArray();
        m_vertexArray->addBuffer( _vBuffer, 0 );
        m_vertexArray->addBuffer( _nBuffer, 1 );
        m_vertexArray->addBuffer( _cBuffer, 2 );



        m_indexBuffer = new LIndexBuffer();
        m_indexBuffer->setData( sizeof( LInd3 ) * indices.size(), 
                                3 * indices.size(), (GLuint*) indices.data() );


    
        scale = vec3( 1.0f, 1.0f, 1.0f );
        m_rotMatrix = RotateY(0.0f);
    }


    LMesh::~LMesh()
    {
        delete m_vertexArray;
        delete m_indexBuffer;
    }

    mat4 LMesh::getModelMatrix() const
    {
        mat4 _model = mat4( 1.0f );

        _model = Scale( scale.x, scale.y, scale.z )*_model;
        _model = m_rotMatrix * _model;
        _model = Translate( pos.x, pos.y, pos.z )*_model;
        return _model;
    }

    void LMesh::updateNormal(bool isSmoth) const
    {
        if(!isSmoth)
        {
            LVertexBuffer* _nBuffer = new LVertexBuffer();
            _nBuffer->setData( sizeof( vec3 ) * m_normals.size(),
                               3, (GLfloat*) m_normals.data() );
            m_vertexArray->addBuffer( _nBuffer, 1 );
            return;
        }
        vector<vec3> _newNormals;
        _newNormals.resize(m_vertices.size());


        for(int i=0; i<m_vertices.size(); i++)
        {
            vec3 normalAverage=m_normals[i];
            int nNormals=0;
            for(int j=0; j<m_vertices.size(); j++)
            {
                if(i!=j)
                {
                    if(m_vertices[i].x==m_vertices[j].x && m_vertices[i].y == m_vertices[j].y && m_vertices[i].z == m_vertices[j].z)
                    {
                        normalAverage = normalAverage + m_normals[j];
                        nNormals++;
                    }
                }
            }
            normalAverage = normalAverage/nNormals;
            _newNormals[i] = normalAverage;
        }

        //hacemos bind  a las normales
        LVertexBuffer* _nBuffer = new LVertexBuffer();
        _nBuffer->setData( sizeof( vec3 ) * _newNormals.size(),
                           3, (GLfloat*) _newNormals.data() );
        m_vertexArray->addBuffer( _nBuffer, 1 );

    }
}