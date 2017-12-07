
#pragma once

#include "LCommon.h"

#include "LMesh.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace engine
{

    struct LMeshBuilder
    {

        private :

        static vec3 _computeFaceNormal( vec3 _v1, vec3 _v2, vec3 _v3 )
        {
            vec3 _res;

            /*
            vec3 _v12 = _v1 - _v2;
            vec3 _v23 = _v3 - _v2;*/

            _res = cross( _v2-_v3, _v2-_v1 );

            return _res;
        }

        //static void _computeNormalsSingle();
        //static void _computeNormalsAvg();

        static vector<string> _split( const string &txt )
        {
            vector<string> _res;
                    
            int pos = txt.find( ' ' );
            if ( pos == std::string::npos )
            {
                _res.push_back( txt );
                return _res;
            }

            int initpos = 0;

            while ( pos != std::string::npos )
            {
                _res.push_back( txt.substr( initpos, pos - initpos + 1 ) );
                initpos = pos + 1;

                pos = txt.find( ' ', initpos );
            }

            _res.push_back( txt.substr( initpos, std::min( pos, (int) txt.size() ) - initpos + 1 ) );
                    
            return _res;
        }

        public :

        static LMesh* createBox( GLfloat width, GLfloat height, GLfloat depth );
        static LMesh* createPlane( GLfloat width, GLfloat depth );
        static LMesh* createPlane( vector<vec3> vertices );
        static LMesh* createPlane( vector<vec3> vertices , vector<vec3> color );
        static LMesh* createFromFile( const char* filename , vec3 color);

    };



}