#shader vertex
#version 330 core

layout(location = 0) in vec3 position; //flag 0 para position
layout(location = 1) in vec3 color; //flag 1 para color
out vec3 ourColor; //output a color para el fragmentColor

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(position, 1.0f);
	//ourColor = color; // se setea color como input del siguiente vertice
};

#shader fragment
#version 330 core

out vec4 fragmentColor;
in vec3 ourColor;

void main()
{
	fragmentColor = vec4(1.0f,1.0f,1.0f,1.0f);
	//fragmentColor = vec4(1.0f,0.0f,0.0f, 1.0f);
};