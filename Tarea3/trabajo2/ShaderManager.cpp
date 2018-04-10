#include "ShaderManager.h"

using namespace std;

namespace graphics
{

    ShaderManager* ShaderManager::INSTANCE = NULL;

    ShaderManager::ShaderManager()
    {
        // Load all shaders available

        GLuint _vShader, _fShader, _program;

        /*
        _vShader = createShader( "../res/shaders/basic_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "../res/shaders/basic_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["basic"] = _program;

        _vShader = createShader( "../res/shaders/basic3d_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "../res/shaders/basic3d_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["basic3d"] = _program;
        */

        
        _vShader = createShader( "../res/shaders/basicVertexShader3d_with_color.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "../res/shaders/basicFragmentShader3d_with_color.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["color"] = _program;

        /*
        _vShader = createShader( "../res/shaders/baseVertexShader3d_with_lighting.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "../res/shaders/baseFragmentShader3d_with_lighting.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["basiclighting"] = _program;
        */
        _vShader = createShader( "../res/shaders/multilight_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "../res/shaders/multilight_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["multilighting"] = _program;

        _vShader = createShader( "../res/shaders/textureshader_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "../res/shaders/textureshader_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["textureshader"] = _program;



    }

    ShaderManager::~ShaderManager()
    {
        release();
    }

    void ShaderManager::release()
    {
        map< string, GLuint >::iterator _it;
        for ( _it = programs.begin(); _it != programs.end(); ++_it )
        {
            glDeleteProgram( _it->second );
        }
    }

    void ShaderManager::create()
    {
        if ( ShaderManager::INSTANCE != NULL )
        {
            delete ShaderManager::INSTANCE;
        }

        ShaderManager::INSTANCE = new ShaderManager();
    }


    GLuint ShaderManager::createShader( const char* filename, int shaderType )
    {
        // Load the shader code into a string ********************************************
        string _shaderStrCode;
        ifstream _shaderFile;

        _shaderFile.exceptions( ifstream::badbit );

        try 
        {
            _shaderFile.open( filename );
            stringstream _shaderStream;
            _shaderStream << _shaderFile.rdbuf();

            _shaderFile.close();

            _shaderStrCode = _shaderStream.str();
        }
        catch ( ... )
        {
            cout << "LShader::createFromFile> failed opening the resource file" << endl;
            return 0;
        }

        const char* _shaderCode_cstr = _shaderStrCode.c_str();
        // *******************************************************************************

        GLint _success;
        GLchar _infoLog[1024];

        GLuint _shaderId = glCreateShader( shaderType );
        glShaderSource( _shaderId, 1, &_shaderCode_cstr, NULL );
        glCompileShader( _shaderId );
        glGetShaderiv( _shaderId, GL_COMPILE_STATUS,
                       &_success );

        if ( !_success )
        {
            glGetShaderInfoLog( _shaderId, 1024, 
                                NULL, _infoLog );
            cout << "Shader: " << filename << endl;
            cout << "Failed to compile shader" << endl;
            cout << "Error: " << _infoLog << endl;

            return 0;
        }

        return _shaderId;
    }

    GLuint ShaderManager::createProgram( GLuint vShaderId, GLuint fShaderId )
    {
        GLint _success;
        GLchar _infoLog[1024];

        GLuint _programId = glCreateProgram();
        glAttachShader( _programId, vShaderId );
        glAttachShader( _programId, fShaderId );
        glLinkProgram( _programId );

        glDetachShader( _programId, vShaderId );
        glDetachShader( _programId, fShaderId );
        glDeleteShader( vShaderId );
        glDeleteShader( fShaderId );

        glGetProgramiv( _programId, GL_LINK_STATUS, &_success );
        if ( !_success )
        {
            glGetProgramInfoLog( _programId, 1024, NULL, _infoLog );
            cout << "LINKING ERROR: " << _infoLog << endl;

            return 0;
        }

        return _programId;
    }

}