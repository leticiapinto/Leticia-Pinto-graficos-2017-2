//#version 330 core
#version 120

/*
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;
*/
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoord;  //descomentar para el texture

/*
out vec3 FragPos;
out vec3 Normal;
out vec4 Eye;
out vec2 TexCoord; //descomentar para el texture
*/
varying vec3 FragPos;
varying vec3 Normal;
varying vec4 Eye;
varying vec2 TexCoord; //descomentar para el texture

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

void main()
{
    FragPos = vec3(u_tModel * vec4(aPos, 1.0));
    Normal = mat3(u_tModel) * aNormal;  
    gl_Position = u_tProj * u_tView * u_tModel * vec4( aPos, 1.0f );
    Eye = u_tView * u_tModel * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y); //descomentar para el texture
}
