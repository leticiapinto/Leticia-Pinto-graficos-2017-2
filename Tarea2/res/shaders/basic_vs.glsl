#version 120

attribute vec2 pos;
 
varying vec3 fColor;

void main() 
{
     gl_Position = vec4( pos, 0.0f, 1.0f );
     fColor = gl_Color;
}