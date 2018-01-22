
#include "MeshBuilder.h"
#include "PerlinNoise.h"
#include <fstream>


using namespace std;

namespace graphics
{

    Mesh* MeshBuilder::createBox( GLfloat width, GLfloat height, GLfloat depth )
    {
        Mesh* _mesh = NULL;

        vector<vec3> _vertices;
        vector<vec3> _normals;
        vector<Ind3> _indices;

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
            _indices.push_back( Ind3( _vertices.size(),
                                       _vertices.size() + 1,
                                       _vertices.size() + 2 ) );

            _indices.push_back( Ind3( _vertices.size(),
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


        _mesh = new Mesh( _vertices, _normals, _indices );

        _mesh->myVertices =_vertices;
        _mesh->myNormals =_normals;

        return _mesh;
    }
    
    Mesh* MeshBuilder::createPlane( vector<vec3> _vertices )
    {
        Mesh* _mesh = NULL;
        vector<vec3> _normals;
        vector<Ind3> _indices;
        _indices.push_back ( Ind3(0 , 1 , 2 ) );
        _indices.push_back ( Ind3(2 , 3 , 0 ) );
        vec3 A=_vertices[0];
        vec3 B=_vertices[1];
        vec3 C=_vertices[2];
        vec3 _n = cross(C-A , C-B);
        for(int i=0; i<_vertices.size(); i++)
        {
            _normals.push_back(_n);
        }
         _mesh = new Mesh( _vertices, _normals, _indices );

        _mesh->myVertices =_vertices;
        _mesh->myNormals =_normals;

        return _mesh;
    }

    Mesh* MeshBuilder::createPlane( vector<vec3> _vertices , vector<vec3> _color )
    {
        Mesh* _mesh = NULL;
        vector<vec3> _normals;
        vector<Ind3> _indices;
        _indices.push_back ( Ind3(0 , 1 , 2 ) );
        _indices.push_back ( Ind3(2 , 3 , 0 ) );

        vec3 A=_vertices[0];
        vec3 B=_vertices[1];
        vec3 C=_vertices[2];
        vec3 _n = cross(C-A , C-B);

        for(int i=0; i<_vertices.size(); i++)
        {
            _normals.push_back(_n);
        }
        //load Textures
         _mesh = new Mesh( _vertices, _normals, _indices, _color );

        _mesh->myVertices =_vertices;
        _mesh->myNormals =_normals;

        return _mesh;
    }

    Mesh* MeshBuilder::createFromFile( const char* filename, vec3 _color )
    {
        Mesh* _mesh = NULL;

        vector<vec3> _vertices;
        vector<vec3> _normals;
        vector<Ind3> _indices;
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
                vector<string> _elemns = MeshBuilder::_split( _line );

                GLfloat _vx = stof( _elemns[0] );
                GLfloat _vy = stof( _elemns[1] );
                GLfloat _vz = stof( _elemns[2] );

                //cout<<_vx<<" "<<_vy<<" "<<_vz<<endl;

                _vertices.push_back( vec3( _vx, _vy, _vz ) );
                _colores.push_back(_color);
                _indices.push_back( Ind3( _indxCounter, 
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

         _mesh = new Mesh( _vertices, _normals, _indices , _colores );

        _mesh->myVertices =_vertices;
        _mesh->myNormals =_normals;

        return _mesh;
    }

    Mesh* MeshBuilder::createTerrain( vec2 vertice , float sizexy, float quads )
    {

        Mesh* _mesh = NULL;
        vector<vec3> _vertices;
        vector<Ind3> _indices;

        float offset_quads =  sizexy/quads; 
        int pass_quads = 0;

        //vertices            
        for( int i = 0; i<quads+1;i++ )
        {
            for( int j = 0 ; j<quads+1 ; j++ )
            {
                float x = i*offset_quads + vertice.x;
                float y = j*offset_quads + vertice.y;
                double z = 0;// perlin.noise( x, 0.2, y);
                _vertices.push_back(vec3 (x , z , y) );
            }
        }

        //indices

        int t = 0;
        for( int i = 0 ; i< _vertices.size() -quads-1; i++)
        {
            t++;
            if( t!=quads+1 )
            {
                int i1 = i;
                int i2 = i + quads + 1;
                int i3 = i + quads + 2;

                int i4 = i3;
                int i5 = i1 + 1;
                int i6 = i1;

                _indices.push_back( Ind3( i1, i2, i3 ) );
                _indices.push_back( Ind3( i4, i5, i6 ) );
            }
            t = t%((int)quads+1);
            
        }


        _mesh = new Mesh( _vertices, _indices);
        _mesh->myVertices =_vertices;

        return _mesh;
    }

    Mesh* MeshBuilder::createTerrainXZ(vec2 _origin, float _sizeX , float _sizeY, float _numQuadsX, float _numQuadsY)
    {
        Mesh* _mesh = NULL;
        vector<Ind3> _indices;
        vector<vec3> _vertices;

        float sizeQuadX = _sizeX/_numQuadsX;
        float sizeQuadY = _sizeY/_numQuadsY;

        for( int i = 0; i<_numQuadsY+1;i++ )
        {
            for( int j = 0 ; j<_numQuadsX+1 ; j++ )
            {
                float x = j*sizeQuadX + _origin.x;
                float y = i*sizeQuadY + _origin.y;
                double z = 0;// perlin.noise( x, 0.2, y);
                _vertices.push_back(vec3 (x , z , y) );

            }
        }

        int t = 0;
        for( int i = 0 ; i< _vertices.size() -_numQuadsX-1; i++)
        {
            t++;
            if( t!=_numQuadsX+1 )
            {
                int i1 = i;
                int i2 = i + _numQuadsX + 1;
                int i3 = i + _numQuadsX + 2;

                int i4 = i3;
                int i5 = i1 + 1;
                int i6 = i1;

                _indices.push_back( Ind3( i1, i2, i3 ) );
                _indices.push_back( Ind3( i4, i5, i6 ) );
            }
            t = t%((int)_numQuadsX+1);
            
        }

        _mesh = new Mesh( _vertices, _indices );
        _mesh->myVertices =_vertices;

        return _mesh;
    }

    Mesh* MeshBuilder::createGridXZ2(vec2 _origin, float _numQuadsX, float _numQuadsY , float sizeX, float sizeY)
    {
        Mesh* _mesh = NULL;
        vector<Ind3> _indices;
        vector<vec3> _vertices;
        vector<vec2> myvertexTexture;

        float pass_x = 1/(_numQuadsX*sizeX);
        float pass_y = 1/(_numQuadsY*sizeY);
        for( int i = 0; i<_numQuadsY+1;i++ )
        {
            for( int j = 0 ; j<_numQuadsX+1 ; j++ )
            {
                float x = j*sizeY + _origin.x;
                float y = i*sizeX + _origin.y;
                double z = 0;// perlin.noise( x, 0.2, y);
                _vertices.push_back(vec3 (x , z , y) );

                //add texture
                //myvertexTexture.push_back(vec2( i*pass_x  ,j*pass_y ) );
            }
        }        

        //cout<<"size:"<<_size<<endl;
        //cout<<"size vert: "<<_vertices.size()<<endl;

        int t = 0;
        for( int i = 0 ; i< _vertices.size() -_numQuadsX-1; i++)
        {
            t++;
            if( t!=_numQuadsX+1 )
            {
                int i1 = i;
                int i2 = i + _numQuadsX + 1;
                int i3 = i + _numQuadsX + 2;

                int i4 = i3;
                int i5 = i1 + 1;
                int i6 = i1;

                _indices.push_back( Ind3( i1, i2, i3 ) );
                _indices.push_back( Ind3( i4, i5, i6 ) );


                myvertexTexture.push_back(vec2( 0.0 ,1.0 ) ); 
                myvertexTexture.push_back(vec2( 0.0 ,0.0 ) );
                myvertexTexture.push_back(vec2( 1.0 ,0.0 ) ); 
                myvertexTexture.push_back(vec2( 1.0 ,1.0 ) );

            }
            t = t%((int)_numQuadsX+1);
            
        }

        _mesh = new Mesh( _vertices, _indices );
        _mesh->myVertices =_vertices;

        _mesh->addTexture2D(myvertexTexture,1);

        return _mesh;
    }


    Mesh* MeshBuilder::createFromFileObj( const char* filename )
    {
        string line;
        ifstream myfile (filename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
              //cout << line << '\n';
              //string str{"the quick brown fox jumps over the lazy dog"};
              vector<string> v= MeshBuilder::_split(line);

              string a = "v";
              string vertex = v[0].c_str();
              if(vertex == a)
              {
                cout<< v[2]<<endl;
              }
              //cout<<endl;
            }
            myfile.close();
        } else{ cout << "Unable to open file";} 
        
    }

}
