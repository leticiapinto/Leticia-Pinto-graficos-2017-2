#include "Window.h"
#include "ShaderManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"

#include "MeshBuilder.h"
#include "Mesh.h"
#include "Camera.h"

#include <iostream>
#include <math.h>
#include "stb_image.h"

using namespace std;

graphics::Window mywindow;
graphics::Mesh* myterrain;

vector<graphics::Mesh*> _grids;
GLuint terrainshader, cubemapshader, skyboxshader;

// camera
Camera camera(vec3(0.0f, 0.0f, 0.0f));

float lastX = APP_WIDTH / 2.0f;
float lastY = APP_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

mat4 MView(1.0f);
mat4 MProj(1.0f);

float offsetGridY = -60;
float offsetGridX = -10;
float offsetGridZ = -10;

unsigned int texture1, texture2, texture3, texture4, texture5, texture6, texture7;
unsigned int cubeTexture;
unsigned int cubemapTexture;

const unsigned int SCR_WIDTH = APP_WIDTH;//1280;
const unsigned int SCR_HEIGHT = APP_HEIGHT;



float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


void drawGrids(vec2 origin, float factor, float u, vector<graphics::Mesh*> &_grids)
{

    float unit = u*factor;

    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x  , origin.y  ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 3*unit  , origin.y  ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 6*unit  , origin.y  ) , 2*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 8*unit  , origin.y  ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 11*unit  , origin.y  ) , 3*factor , 3*factor , u , u ));


    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x  , origin.y + 3*unit  ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 11*unit  , origin.y + 3*unit  ) , 3*factor , 3*factor , u , u ));


    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x , origin.y + 6*unit  ) , 3*factor , 2*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 11*unit  , origin.y + 6*unit  ) , 3*factor , 2*factor , u , u ));


    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x , origin.y + 8*unit  ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 11*unit , origin.y + 8*unit  ) , 3*factor , 3*factor , u , u ));


    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x  , origin.y + 11*unit ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 3*unit  , origin.y + 11*unit  ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 6*unit  , origin.y + 11*unit  ) , 2*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 8*unit  , origin.y + 11*unit  ) , 3*factor , 3*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x + 11*unit  , origin.y + 11*unit  ) , 3*factor , 3*factor , u , u ));
}


void loadTexture(unsigned int &idtexture, string filepath)
{
    glGenTextures(1, &idtexture);
    glBindTexture(GL_TEXTURE_2D, idtexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

unsigned int loadTexture2(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void onKeyCallback( unsigned char key, int x, int y )
{
    if (key == 'w' || key =='W')
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (key == 's' || key =='S')
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (key == 'a' || key =='A')
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (key == 'd' || key =='D')
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void onMouseCallback( int button, int state, int x, int y )
{
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            
        }
    }

    if(button == 3)
    {
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        camera.ProcessMouseScroll(2);
        //UP scroll
    }
    if(button == 4)
    {
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        camera.ProcessMouseScroll(-2);
        //DOWN scroll
    }
}

void onMotionCallBack(int x, int y)
{
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

    lastX = x;
    lastY = y;

    camera.ProcessMouseMovement(xoffset, yoffset);
    camera.MouseSensitivity = 1;
}

void onLoadCallBack()
{


    graphics::ShaderManager::create();
    //ShaderSimpleColor = graphics::ShaderManager::INSTANCE->programs["color"];
    terrainshader = graphics::ShaderManager::INSTANCE->programs["terrainshader"];

    //engine::LShaderManager::create();
    //_pSimpleColor = engine::LShaderManager::INSTANCE->programs["basiccolor"];


    
    float factor=128;  
    float u = 0.2;

    float s = u*factor;
    vec2 origin = vec2(-s/2,-s/2);
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin.x, origin.y ) ,factor , factor , u , u ));
    /*    
    vec2 origin = vec2(0,0);
    float unit = factor*u;
    drawGrids( vec2(origin.x, origin.y), factor , u , _grids);


    vec2 origin2 = vec2(origin.x + unit*3, origin.y+ unit*3);
    float unit2 = factor*u/2;
    drawGrids( vec2(origin2.x, origin2.y), factor , u/2 , _grids);


    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin2.x, origin2.y + 7*unit  ) , 8*factor , 1*factor , u , u ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin2.x+ 7*unit , origin2.y  ) , 1*factor , 7*factor , u , u ));


    vec2 origin3 = vec2(origin.x + unit2*9, origin.y+ unit2*9);
    float unit3 = factor*u/4;
    drawGrids( vec2(origin3.x + unit2 , origin3.y+unit2 ), factor , u/4 , _grids);


    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin3.x , origin3.y   ) , 8*factor , 1*factor , unit2/factor , unit2/factor ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin3.x , origin3.y + 1*unit2 ) , 1*factor , 7*factor , unit2/factor , unit2/factor ));
    

     vec2 origin4 = vec2(origin.x + unit3*22, origin.y+ unit3*22);
    float unit4 = factor*u/8;
    drawGrids( vec2(origin4.x + unit3 , origin4.y+unit3 ), factor , u/8 , _grids);

    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin4.x + 1*unit3, origin4.y + 8*unit3  ) , 8*factor , 1*factor , unit3/factor , unit3/factor ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin4.x+ 8*unit3 , origin4.y + 1*unit3  ) , 1*factor , 7*factor , unit3/factor , unit3/factor ));


    vec2 origin5 = vec2(origin.x + unit4*49, origin.y+ unit4*49);
    float unit5 = factor*u/16;
    drawGrids( vec2(origin5.x + unit4 , origin5.y+unit4 ), factor , u/16 , _grids);

    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin5.x , origin5.y   ) , 8*factor , 1*factor , unit4/factor , unit4/factor ));
    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin5.x , origin5.y + 1*unit4 ) , 1*factor , 7*factor , unit4/factor , unit4/factor ));


    vec2 origin6 = vec2(origin.x + unit5*103, origin.y+ unit5*103);
    float unit6 = factor*u/32;

    _grids.push_back(graphics::MeshBuilder::createGridXZ2( vec2(origin6.x , origin6.y  ) , 16*factor , 16*factor , unit6/factor , unit6/factor ));

    */

    offsetGridY = -2;
    
    //offsetGridX = -origin6.x - 8*unit6 + 0.1;
    //offsetGridZ = -origin6.y - 8*unit6 + 0.1;
    
    /*offsetGridX = -(factor*u)/2;
    offsetGridZ = -(factor*u)/2;

    */

    //texture

    loadTexture(texture1, "../res/textures/texture1.jpg");
    loadTexture(texture2, "../res/textures/texture4.jpg");
    loadTexture(texture4, "../res/textures/t8.jpg");
    loadTexture(texture6, "../res/textures/t4.jpg");
    loadTexture(texture7, "../res/textures/texture7.jpg");

    loadTexture(texture3, "../res/textures/t1.jpg"); // NO
    loadTexture(texture5, "../res/textures/t1.jpg"); // NO

    //ourShader.setInt("texture2", 1);


    //skybox ... cubemap

    //Shader shader("6.1.cubemaps.vs", "6.1.cubemaps.fs");
    //Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");


    cubemapshader = graphics::ShaderManager::INSTANCE->programs["cubemapshader"];
    skyboxshader = graphics::ShaderManager::INSTANCE->programs["skyboxshader"];    

    // load textures
    // -------------
    loadTexture(cubeTexture, "../res/textures/texture5.jpg");

    vector<std::string> faces
    {
        "../res/textures/skybox/right.jpg",
        "../res/textures/skybox/left.jpg",
        "../res/textures/skybox/top.jpg",
        "../res/textures/skybox/bottom.jpg",
        "../res/textures/skybox/back.jpg",
        "../res/textures/skybox/front.jpg"
    };
    cubemapTexture = loadCubemap(faces);

   
    // load models
    // -----------
    //shader to load model

    //graphics::Model ourModel("resources/objects/nanosuit/nanosuit.obj");
    //graphics::MeshBuilder::createFromFileObj("../res/textures/tree/Tree.obj");



}


void onLoopCallBack()
{

    //****camera control

    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame)*0.005;
    lastFrame = currentFrame;

    MView = camera.GetViewMatrix();
    MProj = Perspective(camera.Zoom, (float)APP_WIDTH / (float)APP_HEIGHT, 0.1f, 100.0f);

    

    glUseProgram( terrainshader );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    graphics::ShaderManager::setMat4(terrainshader, "u_tView", MView);
    graphics::ShaderManager::setMat4(terrainshader, "u_tProj", MProj);
    graphics::ShaderManager::setVec3(terrainshader, "viewPos", camera.Position);


     
    /*
    myterrain->getVertexArray()->bind();
    myterrain->getIndexBuffer()->bind();
    glDrawElements( GL_TRIANGLES, 
                    myterrain->getIndexBuffer()->getCount(), 
                    GL_UNSIGNED_INT, 0 );

    myterrain->getVertexArray()->unbind();
    myterrain->getIndexBuffer()->unbind();
    glUseProgram( 0 );*/
    
    for( int i = 0 ; i<_grids.size(); i++ )
    {
        //_grids[i]->pos.x = camera.Position.x + offsetGridX;
        _grids[i]->pos.y = camera.Position.y + offsetGridY;
        //_grids[i]->pos.z = camera.Position.z + offsetGridZ;

        graphics::ShaderManager::setMat4(terrainshader, "u_tModel", _grids[i]->getModelMatrix());

        _grids[i]->getVertexArray()->bind();
        _grids[i]->getIndexBuffer()->bind();
        glDrawElements( GL_TRIANGLES, _grids[i]->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, 0 );
        _grids[i]->getVertexArray()->unbind();
        _grids[i]->getIndexBuffer()->unbind();
    }

    glUniform1i(glGetUniformLocation(terrainshader, "texture1"), 0); // set it manually
    glUniform1i(glGetUniformLocation(terrainshader, "texture2"), 1); // set it manually
    glUniform1i(glGetUniformLocation(terrainshader, "texture3"), 2); // set it manually
    glUniform1i(glGetUniformLocation(terrainshader, "texture4"), 3); // set it manually
    glUniform1i(glGetUniformLocation(terrainshader, "texture5"), 4); // set it manually
    glUniform1i(glGetUniformLocation(terrainshader, "texture6"), 5); // set it manually
    glUniform1i(glGetUniformLocation(terrainshader, "texture7"), 6); // set it manually



    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, texture6);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, texture7);

    glUseProgram( 0 );
    

    glUseProgram( terrainshader );

    graphics::ShaderManager::setMat4(terrainshader, "u_tModel", mat4(1.0f));
    graphics::ShaderManager::setMat4(terrainshader, "u_tView", MView);
    graphics::ShaderManager::setMat4(terrainshader, "u_tProj", MProj);


    graphics::VertexArray* _vertexArrayAxis = new graphics::VertexArray();
    graphics::VertexBuffer* _vertexBufferAxis = new graphics::VertexBuffer();
    graphics::VertexBuffer* _vertexBufferColorAxis = new graphics::VertexBuffer();
    vector<vec3> _verticesAxis;
    vector<vec3> _verticesColorAxis;
    _verticesAxis.push_back(vec3( 0.0f,0.0f,0.0f ) );
    _verticesAxis.push_back(vec3( 100.0f,0.0f,0.0f ) );
    _verticesColorAxis.push_back(vec3(1.0f,0.0f,0.0f));
    _verticesColorAxis.push_back(vec3(1.0f,0.0f,0.0f));

    _verticesAxis.push_back(vec3( 0.0f,0.0f,0.0f ) );
    _verticesAxis.push_back(vec3( 0.0f,100.0f,0.0f ) );
    _verticesColorAxis.push_back(vec3(1.0f,0.0f,1.0f));
    _verticesColorAxis.push_back(vec3(1.0f,0.0f,1.0f));

    _verticesAxis.push_back(vec3( 0.0f,0.0f,0.0f ) );
    _verticesAxis.push_back(vec3( 0.0f,0.0f,100.0f ) );
    _verticesColorAxis.push_back(vec3(0.0f,0.0f,1.0f));
    _verticesColorAxis.push_back(vec3(0.0f,0.0f,1.0f));

    _vertexBufferAxis->setData( sizeof( vec3 ) * _verticesAxis.size(),3, (GLfloat*) _verticesAxis.data() );
    _vertexBufferColorAxis->setData( sizeof( vec3 ) * _verticesColorAxis.size(),3, (GLfloat*) _verticesColorAxis.data() );

    _vertexArrayAxis->addBuffer( _vertexBufferAxis, 0 );
    _vertexArrayAxis->addBuffer( _vertexBufferColorAxis, 2 );

    _vertexArrayAxis->bind();
    _vertexBufferAxis->bind();
    _vertexBufferColorAxis->bind();

    glDrawArrays(GL_LINES, 0, 6);

    _vertexArrayAxis->unbind();
    _vertexBufferAxis->unbind();
    _vertexBufferColorAxis->unbind();

    glUseProgram( 0 );


    glUseProgram( skyboxshader );

     // skybox VAO
    /*
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    graphics::ShaderManager::setInt(skyboxshader, "skybox", 0);
     // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    
    mat4 view2 = mat4WithUpperLeftMat3_2( upperLeftMat3 (camera.GetViewMatrix())) ; // remove translation from the view matrix

    graphics::ShaderManager::setMat4(skyboxshader, "view", view2);
    graphics::ShaderManager::setMat4(skyboxshader, "projection", MProj);
    
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
    
    //glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    //glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVAO);
    */

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    graphics::ShaderManager::setInt(skyboxshader, "skybox", 0);
    graphics::ShaderManager::setInt(cubemapshader, "skybox", 0);


    //view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
    mat4 view2 = mat4WithUpperLeftMat3_2( upperLeftMat3 (camera.GetViewMatrix())) ; // remove translation from the view matrix
    //skyboxShader.setMat4("view", view);
    //skyboxShader.setMat4("projection", projection);
    graphics::ShaderManager::setMat4(skyboxshader, "view", view2);
    graphics::ShaderManager::setMat4(skyboxshader, "projection", MProj);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default


    glUseProgram( 0 );
    
    //cubemap -------------------------------------------------------------------------------------------------------------------------------

    glUseProgram( cubemapshader);

    unsigned int cubeVAO, cubeVBO;
    graphics::ShaderManager::setInt(cubemapshader, "texture1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);



    // cube VAO
    /*
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    */

    //unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    mat4 model2;
    view2 = camera.GetViewMatrix();
    mat4 projection2 = Perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    graphics::ShaderManager::setMat4(cubemapshader, "model", model2);
    graphics::ShaderManager::setMat4(cubemapshader, "view", view2);
    graphics::ShaderManager::setMat4(cubemapshader, "projection", projection2);
    graphics::ShaderManager::setVec3(cubemapshader, "cameraPos", camera.Position);



    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);





   
    glUseProgram( 0 );

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVAO);


}

void onReshapeCallBack(int width, int height)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor( CLEAR_COLOR );
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, width, height);
}

void onIdleCallBack()
{
    glutPostRedisplay();
}


int main( int argc, char *argv[] )
{

    mywindow.registerKeyCallback( onKeyCallback );
    mywindow.registerMouseCallback( onMouseCallback );
    mywindow.registerMotionCallback( onMotionCallBack );
    mywindow.registerLoopCallback( onLoopCallBack );
    mywindow.registerReshapeCallback( onReshapeCallBack );
    mywindow.registerIdleCallback( onIdleCallBack );
    mywindow.registerLoadCallback( onLoadCallBack );
    mywindow.init();
    return 0;
}
