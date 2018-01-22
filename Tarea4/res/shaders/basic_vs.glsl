#version 120

attribute vec3 pos;
attribute vec3 color;
 
varying vec3 fColor;

void main() 
{
     gl_Position = vec4( pos, 1.0f );
     fColor = color;
}