#include "Window.h"
#include "ShaderManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"

#include "MeshBuilder.h"
#include "Mesh.h"

#include <iostream>

using namespace std;


// texture start declaration
#define ImageWidth  64
#define ImageHeight 64
GLubyte Image[ImageHeight][ImageWidth][4];
unsigned char *data;

#define	stripeImageWidth 32
GLubyte stripeImage[4*stripeImageWidth];
//texture end declaration


bool wireframe_status = false;
graphics::Window mywindow;
graphics::Mesh* mysphere;


float traslation_speed=0.08f;
float sphere_angle=0.0f;
int position_route=0;
float radius_sphere=1.0f;


vec3 APoint(3.0f, 1.0f, 5.0f);
vec3 BPoint(-1.0f, 1.0f, -4.0f);
vec3 CPoint(3.5f, 1.0f, -2.5f);

vec3 DistanceAB(BPoint-APoint);
vec3 DistanceBC(CPoint-BPoint);
vec3 DistanceCA(APoint-CPoint);

GLuint ShaderSimpleColor, ShaderMultiLighting, ShaderTexture;


mat4 MView(1.0f);
mat4 MProj(1.0f);
graphics::Mesh* myplane;

vec3 cameraPosition;
vec3 cameraDirection;
vec3 cameraTarget;
vec3 VRUP( 0.0f, 1.0f, 0.0f );

float xposCamera=7.0f, yposCamera=3.0f, zposCamera=-10.0f;

static int menu_id, m_enable_lighting_id, m_shading_id, m_lighting_id, m_wireframe_id, m_lightsource_id,m_fog_options_id,m_texture_mapped_ground_id;


//options
GLuint _pShadingSphere;

//variables del menu
bool _is_b_activated=false;
bool moving=false;
bool lighting = false;
bool smoothing = false;
bool wireframe = false;
bool spointsource = false;
int equation = 0;
int Texture_app_flag = 0;
unsigned int texture;


typedef struct                       /**** Colormap entry structure ****/
{
    unsigned char  R;          /* Blue value */
    unsigned char  G;         /* Green value */
    unsigned char  B;           /* Red value */
    //unsigned char  rgbReserved;      /* Reserved */
} RGB;


void image_set_up(void)
{
 int i, j, c; 
 
 /* --- Generate checkerboard image to the image array ---*/
  for (i = 0; i < ImageHeight; i++)
    for (j = 0; j < ImageWidth; j++)
      {
       c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));

       if (c == 1) /* white */
	{
        c = 255;  
	 	Image[i][j][0] = (GLubyte) c;
        Image[i][j][1] = (GLubyte) c;
        Image[i][j][2] = (GLubyte) c;

        //data

	}
       else  /* green */
	{
         Image[i][j][0] = (GLubyte) 0;
         Image[i][j][1] = (GLubyte) 150;
         Image[i][j][2] = (GLubyte) 0;

		//data
	}

       Image[i][j][3] = (GLubyte) 255;
       //data
      }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

/*--- Generate 1D stripe image to array stripeImage[] ---*/
  for (j = 0; j < stripeImageWidth; j++) {
     /* When j <= 4, the color is (255, 0, 0),   i.e., red stripe/line.
        When j > 4,  the color is (255, 255, 0), i.e., yellow remaining texture
      */
    stripeImage[4*j] = (GLubyte)    255;
    stripeImage[4*j+1] = (GLubyte) ((j>4) ? 255 : 0);
    stripeImage[4*j+2] = (GLubyte) 0; 
    stripeImage[4*j+3] = (GLubyte) 255;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
/*----------- End 1D stripe image ----------------*/

/*--- texture mapping set-up is to be done in 
      init() (set up texture objects),
      display() (activate the texture object to be used, etc.)
      and in shaders.
 ---*/

}


void MenuELighting(int option)
{
	if (option == 0)
	{
		lighting = false;
	}
	else if (option == 1)
	{
		lighting = true;
	}
	glutPostRedisplay();
}

void MenuTypeShading(int option)
{

	if (option == 0)//flat
	{
		smoothing = false;
	}
	else if (option == 1)//smooth
	{
		smoothing = true;
	}

	mysphere->ReCalcNormal(smoothing);
	glutPostRedisplay();
}

void MenuLighting(int option)
{


	glutPostRedisplay();
}


void MainMenu(int option)
{
    if(option == 0)
    {
        xposCamera = 7.0f; yposCamera = 3.0f; zposCamera = -10.0f;
    }
    else if(option == 1)
    {
        exit(0);
    }
    glutPostRedisplay();
}



void MenuEWireFrame(int option)
{
    if(option == 0)
    {
        cout << "wireframe" << endl;
        wireframe = false;
        
    }
    else if( option == 1)
    {
        wireframe = true;
        cout << "fill" << endl;
    }
    glutPostRedisplay();
}

void MenuLightSource(int option)
{
    if(option == 0)
    {
        cout << "spot light" << endl;
        spointsource = false;
        
    }
    else if( option == 1)
    {
        spointsource = true;
        cout << "point source" << endl;
    }
    glutPostRedisplay();
}

void MenuFogOptions(int option)
{
	if(option == 0)
    {
        cout << "disable the fog effect" << endl;   
        equation = 0;
    }
    else if( option == 1)
    {
       cout << "enable linear fog effect"<<endl; 
       equation = 1;
    }else if( option == 2)
    {
    	cout << "enable exponential fog effect" <<endl;
    	equation = 2;
    }else if(option == 3)
    {
    	cout << "enable square exponential fog effect" << endl;
    	equation = 3;
    }
    
    glutPostRedisplay();
}

void MenuTextureMappedGround(int option)
{
	if(option == 0)
    {
        cout << "No Texture Mapped Ground" << endl;
        Texture_app_flag = 0;
    }
    else if( option == 1)
    {
        cout << "Yes Texture Mapped Ground" << endl;
        Texture_app_flag = 1;
    }
    glutPostRedisplay();
}

void BuildMenu(void){ 


    //sub menu
    m_enable_lighting_id = glutCreateMenu(MenuELighting);

    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes", 1);

    //sub menu
    m_shading_id = glutCreateMenu(MenuTypeShading);
    
    glutAddMenuEntry("Flat shading", 0);
    glutAddMenuEntry("Smooth shading", 1);

    //sub menu
    m_lighting_id = glutCreateMenu(MenuLighting);

    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("Yes", 1);

    //sub menu
    m_wireframe_id = glutCreateMenu(MenuEWireFrame);
    
    glutAddMenuEntry("No", 0);
    glutAddMenuEntry("yes", 1);

    //submenu
    m_lightsource_id = glutCreateMenu(MenuLightSource);
    
    glutAddMenuEntry("Spot Light",0);
    glutAddMenuEntry("Point Source",1);
    
    //submenu
    m_fog_options_id = glutCreateMenu(MenuFogOptions);
   
   	glutAddMenuEntry("No Fog",0);
   	glutAddMenuEntry("Linear",1);
   	glutAddMenuEntry("Exponential",2);
   	glutAddMenuEntry("Exponential Square",3);
   	
   	//submenu
   	m_texture_mapped_ground_id = glutCreateMenu(MenuTextureMappedGround);
   	glutAddMenuEntry("No",0);
   	glutAddMenuEntry("Yes",1);
    

    //creating menu
    menu_id = glutCreateMenu(MainMenu);
    glutAddMenuEntry("Default Wiew Point", 0);
    glutAddSubMenu("Enable Lighting", m_enable_lighting_id);
    glutAddSubMenu("Shading", m_shading_id);
    //glutAddSubMenu("Lighting", m_lighting_id);
    glutAddSubMenu("Wire Frame", m_wireframe_id);
    glutAddSubMenu("Light Source", m_lightsource_id);
    glutAddSubMenu("Fog Options", m_fog_options_id);
    glutAddSubMenu("Texture Mapped Ground", m_texture_mapped_ground_id);
    glutAddMenuEntry("Quit", 1);

    glutAttachMenu(GLUT_LEFT_BUTTON);

} 


void FuncTraslationSphere()
{
    vec3 distance,from ,to;
    if(position_route==0)
    {
        from=APoint;
        to=BPoint;
        distance=DistanceAB;
    }
    else if(position_route==1)
    {
        from=BPoint;
        to=CPoint;
        distance=DistanceBC;
    }
    else if(position_route==2)
    {
        from=CPoint;
        to=APoint;
        distance=DistanceCA;
    }
    
    

    vec3 dt =  normal(distance)*traslation_speed;
    mysphere->pos = mysphere->pos + dt;

    sphere_angle = (traslation_speed*length(distance)*4/(radius_sphere));
    vec3 axisRotation = cross(vec3(0.0f,1.0f,0.0f), normal(distance));
    normalize(axisRotation);

    mat4 matrixRotation = Rotate(sphere_angle, axisRotation.x, axisRotation.y, axisRotation.z);
    mysphere->myrotationMatrix = matrixRotation * mysphere->myrotationMatrix;

    if(length(mysphere->pos-from)>=length(distance))
    {
        position_route++;
        if(position_route>=3)
        {
            position_route=0;
        }
    }
}

void onKeyCallback( unsigned char key, int x, int y )
{

    switch (key)
   { 
        case 'b':   
            _is_b_activated = true;
            break;
        case 'B':
            _is_b_activated = true;
            break;
        case 'x':  
            xposCamera--;
            break;
        case 'X':
            xposCamera++;
            break;
        case 'y':
            yposCamera--;
            break; 
        case 'Y':
            yposCamera++;
            break;
        case 'z':
            zposCamera--;
            break; 
        case 'Z':
            zposCamera++;
            break;            
        default:
            break;
    }
}

void onMouseCallback( int button, int position_route, int x, int y )
{
    if(_is_b_activated)
    if (button == GLUT_RIGHT_BUTTON) {
        if (position_route == GLUT_DOWN) {
            moving = !moving;
        }
    }
}

void onMotionCallBack(int x, int y)
{

}

void onLoadCallBack()
{


    BuildMenu();

    graphics::ShaderManager::create();
    ShaderSimpleColor = graphics::ShaderManager::INSTANCE->programs["color"];

    //sphere
    ShaderMultiLighting = graphics::ShaderManager::INSTANCE->programs["multilighting"];

    //plane 
    ShaderTexture = graphics::ShaderManager::INSTANCE->programs["textureshader"];
    

    mysphere = graphics::MeshBuilder::createFromFile( "../res/models/model_sphere_1024.obj", vec3(1.0f, 0.84f, 0.0f) );
    mysphere->pos = vec3( 3.0f, 1.0f, 5.0f );

    std::vector <vec3> myvertexplane;
    myvertexplane.push_back(vec3( 5.0f,0.0f,8.0f ) );
    myvertexplane.push_back(vec3( 5.0f,0.0f,-4.0f ) );
    myvertexplane.push_back(vec3( -5.0f,0.0f,-4.0f ) );
    myvertexplane.push_back(vec3( -5.0f,0.0f,8.0f ) ); 

    std::vector <vec3> mycolorPlane;
    vec3 vertexColorPlane = vec3( 0.0f,1.0f,0.0f );
    mycolorPlane.push_back(vertexColorPlane);
    mycolorPlane.push_back(vertexColorPlane);
    mycolorPlane.push_back(vertexColorPlane);
    mycolorPlane.push_back(vertexColorPlane);

    //cargar funciones de la textura a usar en nuestro programa  
    image_set_up();
    /*
    vector<vec2> myvertexTexture;
    myvertexTexture.push_back(vec2(1.0f, 1.0f));
    myvertexTexture.push_back(vec2(1.0f, 0.0f));
    myvertexTexture.push_back(vec2(0.0f, 0.0f));
    myvertexTexture.push_back(vec2(0.0f, 1.0f));
    */

    vector<vec2> myvertexTexture;
    float unitImg=1.0f/6.0f;//image is 6*6
    float checkUnitW = 12;
    float checkUnitH = 10;
    myvertexTexture.push_back(vec2( checkUnitW * unitImg , checkUnitW * unitImg ) );
    myvertexTexture.push_back(vec2( checkUnitW * unitImg, 0.0f ) );
    myvertexTexture.push_back(vec2( (checkUnitW - checkUnitH) * unitImg, 0.0f ) );
    myvertexTexture.push_back(vec2( (checkUnitW - checkUnitH) * unitImg, checkUnitW * unitImg ) );
    

    //load textures before creating myplane
    //myplane = graphics::MeshBuilder::createPlane(  myvertexplane , mycolorPlane, mytexture );
    myplane = graphics::MeshBuilder::createPlane(  myvertexplane , mycolorPlane);
    myplane->addTexture2D(myvertexTexture);


	
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (Image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
   

}

void onLoopCallBack()
{

    //****camera control
    cameraPosition=vec3( xposCamera, yposCamera, zposCamera );
    cameraDirection=vec3( -7.0f, -3.0f, 10.0f );
    cameraTarget = cameraPosition + cameraDirection;

    MView = LookAt(  cameraPosition, cameraTarget,VRUP );
    MProj = Perspective(  45.0f , (GLfloat) mywindow.width() / mywindow.height(),  0.5f, 100.0f );
    
    //*******************

    //sphere
    
    if(lighting)
    {
        _pShadingSphere = ShaderMultiLighting;
    }
    else
    {
        _pShadingSphere= ShaderSimpleColor;
    }


    if(wireframe)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    
    
    glUseProgram( _pShadingSphere);
    graphics::ShaderManager::setMat4(_pShadingSphere, "u_tModel", mysphere->getModelMatrix());
    graphics::ShaderManager::setMat4(_pShadingSphere, "u_tView", MView);
    graphics::ShaderManager::setMat4(_pShadingSphere, "u_tProj", MProj);


    graphics::ShaderManager::setVec3(_pShadingSphere, "viewPos", cameraPosition);
    //material params
    graphics::ShaderManager::setVec3(_pShadingSphere, "material.ambient", vec3(0.2f,0.2f,0.2f));
    graphics::ShaderManager::setVec3(_pShadingSphere, "material.diffuse", vec3(1.0f,0.84f,0.0f));
    graphics::ShaderManager::setVec3(_pShadingSphere, "material.specular", vec3(1.0f,0.84f,0.0f));
    graphics::ShaderManager::setFloat(_pShadingSphere, "material.shininess", 125.0f);

    //source light params
    graphics::ShaderManager::setVec3(_pShadingSphere, "dirLight.ambient", vec3(0.0f,0.0f,0.0f));
    graphics::ShaderManager::setVec3(_pShadingSphere, "dirLight.diffuse", vec3(0.8f,0.8f,0.8f));
    graphics::ShaderManager::setVec3(_pShadingSphere, "dirLight.specular", vec3(0.2f,0.2f,0.2f));

    //Global light
    graphics::ShaderManager::setVec3(_pShadingSphere, "globalLightColor", vec3(1.0f,1.0f,1.0f));

    //point light
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "pointLight.ambient", vec3(0.0f,0.0f,0.0f));
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "pointLight.diffuse", vec3(1.0f,1.0f,1.0f));
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "pointLight.specular", vec3(1.0f,1.0f,1.0f));
    vec3 posPointLight(-14.0f,12.0f,-3.0f);
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "pointLight.position", posPointLight);
    graphics::ShaderManager::setFloat(_pShadingSphere, "pointLight.constant", 2.0f);
    graphics::ShaderManager::setFloat(_pShadingSphere, "pointLight.linear", 0.01f);
    graphics::ShaderManager::setFloat(_pShadingSphere, "pointLight.quadratic", 0.001f);

    //spot light
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "spotLight.ambient", vec3(0.0f,0.0f,0.0f));
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "spotLight.diffuse", vec3(1.0f,1.0f,1.0f));
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "spotLight.specular", vec3(1.0f,1.0f,1.0f));
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "spotLight.position", posPointLight);
    graphics::ShaderManager::setVec3(ShaderMultiLighting, "spotLight.direction", normalize( vec3(-6.0f, 0.0f, -4.5f) - posPointLight));
    graphics::ShaderManager::setFloat(_pShadingSphere, "spotLight.cutOff", DegreesToRadians * 20.0f);
    graphics::ShaderManager::setFloat(_pShadingSphere, "spotLight.outerCutOff", DegreesToRadians * 15.0f);
    graphics::ShaderManager::setFloat(_pShadingSphere, "spotLight.constant", 2.0f);
    graphics::ShaderManager::setFloat(_pShadingSphere, "spotLight.linear", 0.01f);
    graphics::ShaderManager::setFloat(_pShadingSphere, "spotLight.quadratic", 0.001f);

    graphics::ShaderManager::setBool(ShaderMultiLighting, "isspointsource", spointsource);  
    
    


    //light direction

    graphics::ShaderManager::setVec3(_pShadingSphere, "dirLight.direction", vec3(-0.638813, -0.238667, 0.738211));

    mysphere->getVertexArray()->bind();
    mysphere->getIndexBuffer()->bind();

    glDrawElements( GL_TRIANGLES, mysphere->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, 0 );

    if(moving)
    {
        FuncTraslationSphere();
    }
    

    mysphere->getVertexArray()->unbind();
    mysphere->getIndexBuffer()->unbind();

    glUseProgram( 0 );
   
    // Drawing plane
    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram( ShaderTexture );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    graphics::ShaderManager::setMat4(ShaderTexture, "u_tModel", myplane->getModelMatrix());
    graphics::ShaderManager::setMat4(ShaderTexture, "u_tView", MView);
    graphics::ShaderManager::setMat4(ShaderTexture, "u_tProj", MProj);


     graphics::ShaderManager::setVec3(ShaderTexture, "viewPos", cameraPosition);
     
    //los parametros del material
    graphics::ShaderManager::setVec3(ShaderTexture, "material.ambient", vec3(0.2f,0.2f,0.2f));
    graphics::ShaderManager::setVec3(ShaderTexture, "material.diffuse", vec3(0.0f,1.0f,0.0f));
    graphics::ShaderManager::setVec3(ShaderTexture, "material.specular", vec3(0.0f,0.0f,0.0f));
    graphics::ShaderManager::setFloat(ShaderTexture, "material.shininess", 125.0f);

    //los parametros de la fuente de  luz
    graphics::ShaderManager::setVec3(ShaderTexture, "dirLight.ambient", vec3(0.0f,0.0f,0.0f));
    graphics::ShaderManager::setVec3(ShaderTexture, "dirLight.diffuse", vec3(0.8f,0.8f,0.8f));
    graphics::ShaderManager::setVec3(ShaderTexture, "dirLight.specular", vec3(0.2f,0.2f,0.2f));

    //luz global
    graphics::ShaderManager::setVec3(ShaderTexture, "globalLightColor", vec3(1.0f,1.0f,1.0f));

    //point light
    vec3 diff_specSourceLight = vec3(1.0f,1.0f,1.0f);
    graphics::ShaderManager::setVec3(ShaderTexture, "pointLight.ambient", vec3(0.0f,0.0f,0.0f));
    graphics::ShaderManager::setVec3(ShaderTexture, "pointLight.diffuse", diff_specSourceLight);
    graphics::ShaderManager::setVec3(ShaderTexture, "pointLight.specular", diff_specSourceLight);
    posPointLight=vec3(-14.0f,12.0f,-3.0f);

    graphics::ShaderManager::setVec3(ShaderTexture, "pointLight.position", posPointLight);
    graphics::ShaderManager::setFloat(ShaderTexture, "pointLight.constant", 2.0f);
    graphics::ShaderManager::setFloat(ShaderTexture, "pointLight.linear", 0.01f);
    graphics::ShaderManager::setFloat(ShaderTexture, "pointLight.quadratic", 0.001f);

    //spot light
    graphics::ShaderManager::setVec3(ShaderTexture, "spotLight.ambient", vec3(0.0f,0.0f,0.0f));
    graphics::ShaderManager::setVec3(ShaderTexture, "spotLight.diffuse", diff_specSourceLight);
    graphics::ShaderManager::setVec3(ShaderTexture, "spotLight.specular", diff_specSourceLight);
    graphics::ShaderManager::setVec3(ShaderTexture, "spotLight.position", posPointLight);
    graphics::ShaderManager::setVec3(ShaderTexture, "spotLight.direction", normalize( vec3(-6.0f, 0.0f, -4.5f) - posPointLight));
    graphics::ShaderManager::setFloat(ShaderTexture, "spotLight.cutOff", DegreesToRadians * 20.0f);
    graphics::ShaderManager::setFloat(ShaderTexture, "spotLight.outerCutOff", DegreesToRadians * 15.0f);
    graphics::ShaderManager::setFloat(ShaderTexture, "spotLight.constant", 2.0f);
    graphics::ShaderManager::setFloat(ShaderTexture, "spotLight.linear", 0.01f);
    graphics::ShaderManager::setFloat(ShaderTexture, "spotLight.quadratic", 0.001f);

    graphics::ShaderManager::setVec3(ShaderTexture, "dirLight.direction", vec3(-0.638813, -0.238667, 0.738211));

	//fog
	graphics::ShaderManager::setInt(ShaderTexture, "equation", equation);   

	//load texture  ********************************************************************
	//i & 0x8
    /*
	for(int i = 0; i < ImageHeight; i++)
	{
		for(int j = 0; j < ImageWidth; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				cout << ((Image[i][j][k])&0x8)<<" ";
			}
			cout<<"-";
		}
		cout<<endl;
	}*/

	graphics::ShaderManager::setInt(ShaderTexture, "Texture_app_flag", Texture_app_flag);

	//load texture  ********************************************************************
	

    myplane->getVertexArray()->bind();
    myplane->getIndexBuffer()->bind();

    glDrawElements( GL_TRIANGLES, 
                    myplane->getIndexBuffer()->getCount(), 
                    GL_UNSIGNED_INT, 0 );

    myplane->getVertexArray()->unbind();
    myplane->getIndexBuffer()->unbind();

    glUseProgram( 0 );
    
    //dibujar los ejes
    glUseProgram( ShaderSimpleColor );

    graphics::ShaderManager::setMat4(ShaderSimpleColor, "u_tModel", mat4(1.0f));
    graphics::ShaderManager::setMat4(ShaderSimpleColor, "u_tView", MView);
    graphics::ShaderManager::setMat4(ShaderSimpleColor, "u_tProj", MProj);


    graphics::VertexArray* VAOAxis = new graphics::VertexArray();
    graphics::VertexBuffer* VBOAxis = new graphics::VertexBuffer();
    graphics::VertexBuffer* VBCAxis = new graphics::VertexBuffer();
    vector<vec3> axisVertex;
    vector<vec3> colorVertexAxis;
    axisVertex.push_back(vec3( 0.0f,0.0f,0.0f ) );
    axisVertex.push_back(vec3( 100.0f,0.0f,0.0f ) );
    colorVertexAxis.push_back(vec3(1.0f,0.0f,0.0f));
    colorVertexAxis.push_back(vec3(1.0f,0.0f,0.0f));

    axisVertex.push_back(vec3( 0.0f,0.0f,0.0f ) );
    axisVertex.push_back(vec3( 0.0f,100.0f,0.0f ) );
    colorVertexAxis.push_back(vec3(1.0f,0.0f,1.0f));
    colorVertexAxis.push_back(vec3(1.0f,0.0f,1.0f));

    axisVertex.push_back(vec3( 0.0f,0.0f,0.0f ) );
    axisVertex.push_back(vec3( 0.0f,0.0f,100.0f ) );
    colorVertexAxis.push_back(vec3(0.0f,0.0f,1.0f));
    colorVertexAxis.push_back(vec3(0.0f,0.0f,1.0f));

    VBOAxis->setData( sizeof( vec3 ) * axisVertex.size(),3, (GLfloat*) axisVertex.data() );
    VBCAxis->setData( sizeof( vec3 ) * colorVertexAxis.size(),3, (GLfloat*) colorVertexAxis.data() );

    VAOAxis->addBuffer( VBOAxis, 0 );
    VAOAxis->addBuffer( VBCAxis, 2 );
    

    VAOAxis->bind();
    VBOAxis->bind();
    VBCAxis->bind();

    glDrawArrays(GL_LINES, 0, 6);

    VAOAxis->unbind();
    VBOAxis->unbind();
    VBCAxis->unbind();

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
