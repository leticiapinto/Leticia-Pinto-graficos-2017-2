#version 120
//#version 330 core

//layout ( location = 0 ) in vec3 position;
//layout ( location = 1 ) in vec3 normal;
//layout ( location = 2 ) in vec3 color;

attribute vec3 position;
attribute vec3 normal;
attribute vec3 color;

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

//out vec3 vNormal;
//out vec3 vFragPos;
//out vec3 vColor;

varying vec3 vNormal;
varying vec3 vFragPos;
varying vec3 vColor;

void main()
{
    //gl_Position = u_tProj * u_tView * u_tModel * vec4( position, 1.0f );
    gl_Position = u_tProj * u_tView * u_tModel * vec4( position, 1.0f );
    vFragPos = vec3( u_tModel * vec4( position, 1.0f ) );
    vNormal = mat3(  u_tModel ) * normal;
    vColor = color;
}