#version 130
//#version 330 core

in vec2 TexCoord; //descomentar para el texture
in float height;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;

out vec4 color;


void main()
{
    color = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    //color =  texture( texture1, TexCoord );
    //color1 = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    //color2= mix(texture(texture1, TexCoord), texture(texture3, TexCoord), 0.2);
    //color = color1 + color2;
    if(height < 0.05)
    {
    	color =  texture( texture1, TexCoord );
    }else if(height < 0.30)
    {
    	color =  texture( texture2, TexCoord );
    }else if(height < 0.50)
    {
    	color =  texture( texture4, TexCoord );
    }else if(height < 0.70)
    {
    	color =  texture( texture6, TexCoord );
    }else
    {
    	color =  texture( texture7, TexCoord );
    }
}