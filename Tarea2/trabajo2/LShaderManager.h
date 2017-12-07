

#pragma once

#include "../Common.h"

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "Angel-yjc.h"

using namespace std;

namespace engine
{

    struct LShaderManager
    {

        private :

        LShaderManager();

        public :

        map< string, GLuint > programs;

        static LShaderManager* INSTANCE;

        static void create();
        ~LShaderManager();
        void release();

        GLuint createShader( const char* filename, int shaderType );
        GLuint createProgram( GLuint vShader, GLuint fShader );


        static void setBool(int ID,const std::string &name, bool value)
        {         
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
        }
        // ------------------------------------------------------------------------
        static void setInt(int ID , const std::string &name, int value)
        { 
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        static void setFloat(int ID, const std::string &name, float value)
        { 
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        static void setVec2(int ID, const std::string &name, const vec2 &value)
        { 
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value.x); 
        }
        static void setVec2(int ID, const std::string &name, float x, float y)
        { 
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
        }
        // ------------------------------------------------------------------------
        static void setVec3(int ID, const std::string &name, const vec3 &value)
        { 
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value.x); 
        }
        static void setVec3(int ID, const std::string &name, float x, float y, float z)
        { 
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
        }
        // ------------------------------------------------------------------------
        static void setVec4(int ID, const std::string &name, const vec4 &value)
        { 
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value.x); 
        }
        static void setVec4(int ID, const std::string &name, float x, float y, float z, float w) 
        { 
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
        }
        // ------------------------------------------------------------------------
        static void setMat2(int ID, const std::string &name, const mat2 &mat)
        {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, &mat[0].x);
        }
        // ------------------------------------------------------------------------
        static void setMat3(int ID, const std::string &name, const mat3 &mat)
        {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, &mat[0].x);
        }
        // ------------------------------------------------------------------------
        static void setMat4(int ID, const std::string &name, const mat4 &mat)
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, &mat[0].x);
        }


    };

}