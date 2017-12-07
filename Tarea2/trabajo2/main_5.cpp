#include "LWindow.h"
#include "LShaderManager.h"
#include "LVertexArray.h"
#include "LVertexBuffer.h"
#include "LIndexBuffer.h"

#include "LMeshBuilder.h"
#include "LMesh.h"

#include <iostream>

using namespace std;

bool g_isWireframe = false;
engine::LWindow _window;
engine::LMesh* _sphere;
float _rSphere=1.0f;
vec3 A(3.0f, 1.0f, 5.0f);
vec3 B(-1.0f, 1.0f, -4.0f);
vec3 C(3.5f, 1.0f, -2.5f);
vec3 AB(B-A);
vec3 BC(C-B);
vec3 CA(A-C);
float _speed=0.05f;
float angle=0.0f;
int state=0;
GLuint _pSimpleColor, _pLight;
mat4 _viewMatrix(1.0f);
mat4 _projMatrix(1.0f);
engine::LMesh* _plane;

vec3 _cameraPos;
vec3 _cameraDir;
vec3 _cameraTarget;
vec3 _worldUp( 0.0f, 1.0f, 0.0f );

float xCamera=7.0f, yCamera=3.0f, zCamera=-10.0f;

static int menu_id, menu_enable_lighting_id, menu_shading_id, menu_lighting_id, menu_wireframe_id, menu_lightsource_id;


//options
GLuint _pShadingSphere;

//variables del menu
bool _is_b_activated=false;
bool _ismoving=false;
bool _islighting = false;
bool _issmoothing = false;
bool _iswireframe = false;
bool _isspointsource = false;


void menu(int option)
{
    if(option == 0)
    {
        xCamera = 7.0f; yCamera = 3.0f; zCamera = -10.0f;
    }
    else if(option == 1)
    {
        exit(0);
    }
    glutPostRedisplay();
}

void menu_enable_lighting(int option)
{
    if(option == 0)
    {
        _islighting = false;
    }
    else if( option == 1)
    {
        _islighting = true;
    }
    glutPostRedisplay();
}

void menu_shading(int option)
{

    if(option == 0)//flat
    {
        _issmoothing = false;
    }
    else if( option == 1)//smooth
    {
        _issmoothing = true;
    }

    _sphere->updateNormal(_issmoothing);
    glutPostRedisplay();
}

void menu_lighting(int option)
{

    
    glutPostRedisplay();
}

void menu_wireframe(int option)
{
    if(option == 0)
    {
        cout << "changed to wireframe" << endl;
        _iswireframe = false;
        
    }
    else if( option == 1)
    {
        _iswireframe = true;
        cout << "changed to fill" << endl;
    }
    glutPostRedisplay();
}

void menu_lightsource(int option)
{
    if(option == 0)
    {
        cout << "changed to spot light" << endl;
        _isspointsource = false;
        
    }
    else if( option == 1)
    {
        _isspointsource = true;
        cout << "changed to point source" << endl;
    }
    glutPostRedisplay();
}

void createMenu(void){ 


    //sub menu
    menu_enable_lighting_id = glutCreateMenu(menu_enable_lighting);
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes", 1);

    //sub menu
    menu_shading_id = glutCreateMenu(menu_shading);
    glutAddMenuEntry("Flat shading", 0);
    glutAddMenuEntry("Smooth shading", 1);

    //sub menu
    menu_lighting_id = glutCreateMenu(menu_lighting);
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes", 1);

    //sub menu
    menu_wireframe_id = glutCreateMenu(menu_wireframe);
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("yes", 1);

    //submenu
    menu_lightsource_id = glutCreateMenu(menu_lightsource);
    glutAddMenuEntry("Spot Light",0);
    glutAddMenuEntry("Point Source",1);

    //creating menu
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Default Wiew Point", 0);
    glutAddSubMenu("Enable Lighting", menu_enable_lighting_id);
    glutAddSubMenu("Shading", menu_shading_id);
    glutAddSubMenu("Lighting", menu_lighting_id);
    glutAddSubMenu("Wire Frame", menu_wireframe_id);
    glutAddSubMenu("Light Source", menu_lightsource_id);
    glutAddMenuEntry("Quit", 1);

    glutAttachMenu(GLUT_LEFT_BUTTON);

} 


void traslationSphere()
{
    vec3 distance,from ,to;
    if(state==0)//DE A a B
    {
        from=A;
        to=B;
        distance=AB;
    }
    else if(state==1)
    {
        from=B;
        to=C;
        distance=BC;
    }
    else if(state==2)
    {
        from=C;
        to=A;
        distance=CA;
    }
    
    

    vec3 delta =  normal(distance)*_speed;
    _sphere->pos = _sphere->pos + delta;

    angle = (length(distance)*_speed*4/(_rSphere));
    //cout<<"angle: "<<angle<<endl;

    vec3 axisRot = cross(vec3(0.0f,1.0f,0.0f), normal(distance));

    normalize(axisRot);

    mat4 rotMatrix = Rotate(angle, axisRot.x, axisRot.y, axisRot.z);
    _sphere->m_rotMatrix = rotMatrix * _sphere->m_rotMatrix;

    if(length(_sphere->pos-from)>=length(distance))
    {
        state++;
        if(state>=3)
        {
            state=0;
        }
    }
}

void onKeyCallback( unsigned char key, int x, int y )
{
    if (key == 'w' || key =='W')
    {
        g_isWireframe=!g_isWireframe;
        if(g_isWireframe)
        {
            cout << "changed to wireframe" << endl;
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        else
        {
            cout << "changed to fill" << endl;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    }
    else if( key == 'b' || key== 'B'){
        _is_b_activated = true;
    }
    else if( key == 'x' )
    {
        xCamera--;
    }
    else if( key == 'X' )
    {
        xCamera++;
    }
    else if( key == 'y' )
    {
        yCamera--;
    }
    else if( key == 'Y' )
    {
        yCamera++;
    }
    else if( key == 'z' )
    {
        zCamera--;
    }
    else if( key == 'Z' )
    {
        zCamera++;
    }
}

void onMouseCallback( int button, int state, int x, int y )
{
    if(_is_b_activated)
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            _ismoving = !_ismoving;
        }
    }
}

void onMotionCallBack(int x, int y)
{

}

void onLoadCallBack()
{


    createMenu();

    engine::LShaderManager::create();
    _pSimpleColor = engine::LShaderManager::INSTANCE->programs["basiccolor"];

    //sphere
    _pLight = engine::LShaderManager::INSTANCE->programs["multilighting"];
    

    _sphere = engine::LMeshBuilder::createFromFile( "../res/models/model_sphere_1024.obj", vec3(1.0f, 0.84f, 0.0f) );


    std::vector <vec3> _vertexPlane;
    _vertexPlane.push_back(vec3( 5.0f,0.0f,8.0f ) );
    _vertexPlane.push_back(vec3( 5.0f,0.0f,-4.0f ) );
    _vertexPlane.push_back(vec3( -5.0f,0.0f,-4.0f ) );
    _vertexPlane.push_back(vec3( -5.0f,0.0f,8.0f ) ); 

    std::vector <vec3> _colorPlane;
    vec3 vertexColorPlane = vec3( 0.0f,1.0f,0.0f );
    _colorPlane.push_back(vertexColorPlane);
    _colorPlane.push_back(vertexColorPlane);
    _colorPlane.push_back(vertexColorPlane);
    _colorPlane.push_back(vertexColorPlane);

    _plane = engine::LMeshBuilder::createPlane(  _vertexPlane , _colorPlane );

    _sphere->pos = vec3( 3.0f, 1.0f, 5.0f );

    

}

void onLoopCallBack()
{
    //****camera control
    _cameraPos=vec3( xCamera, yCamera, zCamera );
    _cameraDir=vec3( -7.0f, -3.0f, 10.0f );
    _cameraTarget = _cameraPos + _cameraDir;

    _viewMatrix = LookAt(  _cameraPos, _cameraTarget,_worldUp );
    _projMatrix = Perspective(  45.0f , (GLfloat) _window.width() / _window.height(),  0.5f, 100.0f );
    
    //*******************

    //sphere
    
    if(_islighting)
    {
        _pShadingSphere = _pLight;
    }
    else
    {
        _pShadingSphere= _pSimpleColor;
    }


    if(_iswireframe)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    
    glUseProgram( _pShadingSphere);
    engine::LShaderManager::setMat4(_pShadingSphere, "u_tModel", _sphere->getModelMatrix());
    engine::LShaderManager::setMat4(_pShadingSphere, "u_tView", _viewMatrix);
    engine::LShaderManager::setMat4(_pShadingSphere, "u_tProj", _projMatrix);


    engine::LShaderManager::setVec3(_pShadingSphere, "viewPos", _cameraPos);
    //material params
    engine::LShaderManager::setVec3(_pShadingSphere, "material.ambient", vec3(0.2f,0.2f,0.2f));
    engine::LShaderManager::setVec3(_pShadingSphere, "material.diffuse", vec3(1.0f,0.84f,0.0f));
    engine::LShaderManager::setVec3(_pShadingSphere, "material.specular", vec3(1.0f,0.84f,0.0f));
    engine::LShaderManager::setFloat(_pShadingSphere, "material.shininess", 125.0f);

    //source light params
    engine::LShaderManager::setVec3(_pShadingSphere, "dirLight.ambient", vec3(0.0f,0.0f,0.0f));
    engine::LShaderManager::setVec3(_pShadingSphere, "dirLight.diffuse", vec3(0.8f,0.8f,0.8f));
    engine::LShaderManager::setVec3(_pShadingSphere, "dirLight.specular", vec3(0.2f,0.2f,0.2f));

    //Global light
    engine::LShaderManager::setVec3(_pShadingSphere, "globalLightColor", vec3(1.0f,1.0f,1.0f));

    //point light
    engine::LShaderManager::setVec3(_pLight, "pointLight.ambient", vec3(0.0f,0.0f,0.0f));
    engine::LShaderManager::setVec3(_pLight, "pointLight.diffuse", vec3(1.0f,1.0f,1.0f));
    engine::LShaderManager::setVec3(_pLight, "pointLight.specular", vec3(1.0f,1.0f,1.0f));
    vec3 posPointLight(-14.0f,12.0f,-3.0f);
    engine::LShaderManager::setVec3(_pLight, "pointLight.position", posPointLight);
    engine::LShaderManager::setFloat(_pShadingSphere, "pointLight.constant", 2.0f);
    engine::LShaderManager::setFloat(_pShadingSphere, "pointLight.linear", 0.01f);
    engine::LShaderManager::setFloat(_pShadingSphere, "pointLight.quadratic", 0.001f);

    //spot light
    engine::LShaderManager::setVec3(_pLight, "spotLight.ambient", vec3(0.0f,0.0f,0.0f));
    engine::LShaderManager::setVec3(_pLight, "spotLight.diffuse", vec3(1.0f,1.0f,1.0f));
    engine::LShaderManager::setVec3(_pLight, "spotLight.specular", vec3(1.0f,1.0f,1.0f));
    engine::LShaderManager::setVec3(_pLight, "spotLight.position", posPointLight);
    engine::LShaderManager::setVec3(_pLight, "spotLight.direction", normalize( vec3(-6.0f, 0.0f, -4.5f) - posPointLight));
    engine::LShaderManager::setFloat(_pShadingSphere, "spotLight.cutOff", DegreesToRadians * 20.0f);
    engine::LShaderManager::setFloat(_pShadingSphere, "spotLight.outerCutOff", DegreesToRadians * 15.0f);
    engine::LShaderManager::setFloat(_pShadingSphere, "spotLight.constant", 2.0f);
    engine::LShaderManager::setFloat(_pShadingSphere, "spotLight.linear", 0.01f);
    engine::LShaderManager::setFloat(_pShadingSphere, "spotLight.quadratic", 0.001f);

    engine::LShaderManager::setBool(_pLight, "isspointsource", _isspointsource);    


    //light direction

    engine::LShaderManager::setVec3(_pShadingSphere, "dirLight.direction", vec3(-0.638813, -0.238667, 0.738211));

    _sphere->getVertexArray()->bind();
    _sphere->getIndexBuffer()->bind();

    glDrawElements( GL_TRIANGLES, _sphere->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, 0 );

    if(_ismoving)
    {
        traslationSphere();
    }
    

    _sphere->getVertexArray()->unbind();
    _sphere->getIndexBuffer()->unbind();

    glUseProgram( 0 );
    


    // Drawing plane
    glUseProgram( _pLight );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    engine::LShaderManager::setMat4(_pLight, "u_tModel", _plane->getModelMatrix());
    engine::LShaderManager::setMat4(_pLight, "u_tView", _viewMatrix);
    engine::LShaderManager::setMat4(_pLight, "u_tProj", _projMatrix);


     engine::LShaderManager::setVec3(_pLight, "viewPos", _cameraPos);
    //los parametros del material
    engine::LShaderManager::setVec3(_pLight, "material.ambient", vec3(0.2f,0.2f,0.2f));
    engine::LShaderManager::setVec3(_pLight, "material.diffuse", vec3(0.0f,1.0f,0.0f));
    engine::LShaderManager::setVec3(_pLight, "material.specular", vec3(0.0f,0.0f,0.0f));
    engine::LShaderManager::setFloat(_pLight, "material.shininess", 125.0f);

    //los parametros de la fuente de  luz
    engine::LShaderManager::setVec3(_pLight, "dirLight.ambient", vec3(0.0f,0.0f,0.0f));
    engine::LShaderManager::setVec3(_pLight, "dirLight.diffuse", vec3(0.8f,0.8f,0.8f));
    engine::LShaderManager::setVec3(_pLight, "dirLight.specular", vec3(0.2f,0.2f,0.2f));

    //luz global
    engine::LShaderManager::setVec3(_pLight, "globalLightColor", vec3(1.0f,1.0f,1.0f));

    //point light
    vec3 diff_specSourceLight = vec3(1.0f,1.0f,1.0f);
    engine::LShaderManager::setVec3(_pLight, "pointLight.ambient", vec3(0.0f,0.0f,0.0f));
    engine::LShaderManager::setVec3(_pLight, "pointLight.diffuse", diff_specSourceLight);
    engine::LShaderManager::setVec3(_pLight, "pointLight.specular", diff_specSourceLight);
    posPointLight=vec3(-14.0f,12.0f,-3.0f);

    engine::LShaderManager::setVec3(_pLight, "pointLight.position", posPointLight);
    engine::LShaderManager::setFloat(_pLight, "pointLight.constant", 2.0f);
    engine::LShaderManager::setFloat(_pLight, "pointLight.linear", 0.01f);
    engine::LShaderManager::setFloat(_pLight, "pointLight.quadratic", 0.001f);

    //spot light
    engine::LShaderManager::setVec3(_pLight, "spotLight.ambient", vec3(0.0f,0.0f,0.0f));
    engine::LShaderManager::setVec3(_pLight, "spotLight.diffuse", diff_specSourceLight);
    engine::LShaderManager::setVec3(_pLight, "spotLight.specular", diff_specSourceLight);
    engine::LShaderManager::setVec3(_pLight, "spotLight.position", posPointLight);
    engine::LShaderManager::setVec3(_pLight, "spotLight.direction", normalize( vec3(-6.0f, 0.0f, -4.5f) - posPointLight));
    engine::LShaderManager::setFloat(_pLight, "spotLight.cutOff", DegreesToRadians * 20.0f);
    engine::LShaderManager::setFloat(_pLight, "spotLight.outerCutOff", DegreesToRadians * 15.0f);
    engine::LShaderManager::setFloat(_pLight, "spotLight.constant", 2.0f);
    engine::LShaderManager::setFloat(_pLight, "spotLight.linear", 0.01f);
    engine::LShaderManager::setFloat(_pLight, "spotLight.quadratic", 0.001f);

    engine::LShaderManager::setVec3(_pLight, "dirLight.direction", vec3(-0.638813, -0.238667, 0.738211));



    _plane->getVertexArray()->bind();
    _plane->getIndexBuffer()->bind();

    glDrawElements( GL_TRIANGLES, 
                    _plane->getIndexBuffer()->getCount(), 
                    GL_UNSIGNED_INT, 0 );

    _plane->getVertexArray()->unbind();
    _plane->getIndexBuffer()->unbind();

    glUseProgram( 0 );
    
    //dibujar los ejes
    glUseProgram( _pSimpleColor );

    engine::LShaderManager::setMat4(_pSimpleColor, "u_tModel", mat4(1.0f));
    engine::LShaderManager::setMat4(_pSimpleColor, "u_tView", _viewMatrix);
    engine::LShaderManager::setMat4(_pSimpleColor, "u_tProj", _projMatrix);


    engine::LVertexArray* _vertexArrayAxis = new engine::LVertexArray();
    engine::LVertexBuffer* _vertexBufferAxis = new engine::LVertexBuffer();
    engine::LVertexBuffer* _vertexBufferColorAxis = new engine::LVertexBuffer();
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

    _window.registerKeyCallback( onKeyCallback );
    _window.registerMouseCallback( onMouseCallback );
    _window.registerMotionCallback( onMotionCallBack );
    _window.registerLoopCallback( onLoopCallBack );
    _window.registerReshapeCallback( onReshapeCallBack );
    _window.registerIdleCallback( onIdleCallBack );
    _window.registerLoadCallback( onLoadCallBack );
    _window.init();
    return 0;
}