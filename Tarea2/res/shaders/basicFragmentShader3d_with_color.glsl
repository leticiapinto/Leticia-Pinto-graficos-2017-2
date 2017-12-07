//#version 330 core
#version 120

varying vec3 fNormal;
varying vec3 vColor;
//out vec4 color;

void main()
{
    //color = vec4( vColor, 1.0f );
    gl_FragColor = vec4( vColor, 1.0f );
}