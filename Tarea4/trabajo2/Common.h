
#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>

/*
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
*/

#include "Angel-yjc.h"

/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>*/

using namespace std;

namespace graphics
{

    struct Vec3
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;

        Vec3()
        {
            this->x = 0;
            this->y = 0;
            this->z = 0;        
        }

        Vec3( GLfloat x, GLfloat y, GLfloat z )
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        
        Vec3 operator+( const Vec3& other )
        {
            return Vec3( this->x + other.x,
                              this->y + other.y,
                              this->z + other.z );
        }

        GLfloat* getBuffer()
        {
            GLfloat* r= new GLfloat[3];
            r[0]=x;
            r[1]=y;
            r[2]=z;
            return r;
        }

        Vec3 operator-( const Vec3& other )
        {
            return Vec3( this->x - other.x,
                              this->y - other.y,
                              this->z - other.z );
        }

        void normalize()
        {
            GLfloat _len = sqrt( x * x + y * y + z * z );
            x = x / _len;
            y = y / _len;
            z = z / _len;
        }

        Vec3 getNormal()
        {

            GLfloat _len = sqrt( x * x + y * y + z * z );
            float _x = x / _len;
            float _y = y / _len;
            float _z = z / _len;
            Vec3 r(_x,_y,_z);
            return r;
        }
        GLfloat getLen()
        {

            GLfloat _len = sqrt( x * x + y * y + z * z );
            return _len;
        }

        Vec3 operator*( float s )
        {
            return Vec3( this->x * s,
                              this->y * s,
                              this->z * s );
        }

        void scale( float sx, float sy, float sz )
        {
            x *= sx; y *= sy; z *= sz;
        }

        static Vec3 cross( const Vec3& v1,
                            const Vec3& v2 )
        {
            Vec3 _res;

            _res.x = v1.y * v2.z - v1.z * v2.y;
            _res.y = v1.z * v2.x - v1.x * v2.z;
            _res.z = v1.x * v2.y - v1.y * v2.x;

            return _res;
        }
        static Vec3 dot( const Vec3& v1,
                            const Vec3& v2 )
        {
            Vec3 _res;

            _res.x = v1.x * v2.x;
            _res.y = v1.y * v2.y;
            _res.z = v1.z * v2.z;

            return _res;
        }

    };

    struct LTri
    {
        GLuint i1;
        GLuint i2;
        GLuint i3;
    };

    union Ind3
    {
        GLuint buff[3];
        LTri tri;

        Ind3()
        {
            buff[0] = 0;
            buff[1] = 0;
            buff[2] = 0;
        }

        Ind3( GLuint v1, GLuint v2, GLuint v3 )
        {
            buff[0] = v1;
            buff[1] = v2;
            buff[2] = v3;
        }

        void dump()
        {
            cout << "i1: " << buff[0] << " - i2: " << buff[1] << " - i3: " << buff[2] << endl;
        }
    };




}