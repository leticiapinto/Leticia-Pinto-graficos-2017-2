
#include "LMeshBuilder.h"

using namespace std;

namespace engine
{

    LMesh* LMeshBuilder::createBox( GLfloat width, GLfloat height, GLfloat depth )
    {
        LMesh* _mesh = NULL;

        vector<vec3> _vertices;
        vector<vec3> _normals;
        vector<LInd3> _indices;

        vector<vec3> _normalsSource;

        _normalsSource.push_back( vec3(  0,  0,  1 ) );
        _normalsSource.push_back( vec3(  0,  0, -1 ) );
        _normalsSource.push_back( vec3(  0,  1,  0 ) );
        _normalsSource.push_back( vec3(  0, -1,  0 ) );
        _normalsSource.push_back( vec3(  1,  0,  0 ) );
        _normalsSource.push_back( vec3( -1,  0,  0 ) );

        vec3 _scale( 0.5 * width, 0.5 * height, 0.5 * depth );

        // for each face, compute the vertices that form ...
        // the face perpendicular to that normal
        for ( int q = 0; q < _normalsSource.size(); q++ )
        {
            vec3 _n = _normalsSource[q];

            // form a tri perpendicular right hand system
            vec3 _s1 = vec3( _n.y, _n.z, _n.x );
            vec3 _s2 = cross( _n, _s1 );

            // Add the indices accordingly
            _indices.push_back( LInd3( _vertices.size(),
                                       _vertices.size() + 1,
                                       _vertices.size() + 2 ) );

            _indices.push_back( LInd3( _vertices.size(),
                                       _vertices.size() + 2,
                                       _vertices.size() + 3 ) );

            // Generate each vertex of each face ...
            // according to these vectors
            vec3 _v;

            _v = _n - _s1 - _s2;

            scale(_v, _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

            _v = _n - _s1 + _s2;
            scale( _v, _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

            _v = _n + _s1 + _s2;
            scale(_v, _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

            _v = _n + _s1 - _s2;
            scale(_v, _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

        }


        _mesh = new LMesh( _vertices, _normals, _indices );

        _mesh->m_vertices =_vertices;
        _mesh->m_normals =_normals;

        return _mesh;
    }
    
    LMesh* LMeshBuilder::createPlane( vector<vec3> _vertices )
    {
        LMesh* _mesh = NULL;
        vector<vec3> _normals;
        vector<LInd3> _indices;
        _indices.push_back ( LInd3(0 , 1 , 2 ) );
        _indices.push_back ( LInd3(2 , 3 , 0 ) );
        vec3 A=_vertices[0];
        vec3 B=_vertices[1];
        vec3 C=_vertices[2];
        vec3 _n = cross(C-A , C-B);
        for(int i=0; i<_vertices.size(); i++)
        {
            _normals.push_back(_n);
        }
         _mesh = new LMesh( _vertices, _normals, _indices );

        _mesh->m_vertices =_vertices;
        _mesh->m_normals =_normals;

        return _mesh;
    }

    LMesh* LMeshBuilder::createPlane( vector<vec3> _vertices , vector<vec3> _color )
    {
        LMesh* _mesh = NULL;
        vector<vec3> _normals;
        vector<LInd3> _indices;
        _indices.push_back ( LInd3(0 , 1 , 2 ) );
        _indices.push_back ( LInd3(2 , 3 , 0 ) );

        vec3 A=_vertices[0];
        vec3 B=_vertices[1];
        vec3 C=_vertices[2];
        vec3 _n = cross(C-A , C-B);

        for(int i=0; i<_vertices.size(); i++)
        {
            _normals.push_back(_n);
        }
         _mesh = new LMesh( _vertices, _normals, _indices, _color );

        _mesh->m_vertices =_vertices;
        _mesh->m_normals =_normals;

        return _mesh;
    }


    LMesh* LMeshBuilder::createFromFile( const char* filename, vec3 _color )
    {
        LMesh* _mesh = NULL;

        vector<vec3> _vertices;
        vector<vec3> _normals;
        vector<LInd3> _indices;
        vector<vec3> _colores;

        ifstream _fileHandle( filename );
        string _line;

        getline( _fileHandle, _line );

        int _numFaces = stoi( _line );

        int _indxCounter = 0;

        for ( int q = 0; q < _numFaces; q++ )
        {
            getline( _fileHandle, _line );

            int _numVertices = stoi( _line );

            for ( int p = 0; p < _numVertices; p++ )
            {
                getline( _fileHandle, _line );
                vector<string> _elemns = LMeshBuilder::_split( _line );

                GLfloat _vx = stof( _elemns[0] );
                GLfloat _vy = stof( _elemns[1] );
                GLfloat _vz = stof( _elemns[2] );

                //cout<<_vx<<" "<<_vy<<" "<<_vz<<endl;

                _vertices.push_back( vec3( _vx, _vy, _vz ) );
                _colores.push_back(_color);
                _indices.push_back( LInd3( _indxCounter, 
                                           _indxCounter + 1, 
                                           _indxCounter + 2 ) );
                _indxCounter += 3;
            }

        }
        for(int i=0; i<_vertices.size(); i+=3)
        {
            vec3 A=_vertices[i];
            vec3 B=_vertices[i+1];
            vec3 C=_vertices[i+2];
            vec3 _n = cross(C-A , C-B);

            _normals.push_back(_n);
            _normals.push_back(_n);
            _normals.push_back(_n);
        }

         _mesh = new LMesh( _vertices, _normals, _indices , _colores );

        _mesh->m_vertices =_vertices;
        _mesh->m_normals =_normals;

        return _mesh;
    }






}