#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color; //flag 1 para color
layout(location = 2) in vec3 normal; //normals

out vec3 outColor; //output a color para el color
out vec3 outNormal; //output a color para la normal
out vec3 outFragPos; //output a color para la pos del fragment

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{

	gl_Position = Projection * View * Model * vec4(pos, 1.0);
	outColor = color; 
	outNormal = mat3(transpose(inverse(Model))) * normal;
	outFragPos = vec3(Model * vec4(pos, 1.0));

}

#shader fragment
#version 330 core

in vec3 outColor;
in vec3 outNormal;
in vec3 outFragPos;


out vec4 FragColor;

uniform vec3 lightcolor;
uniform vec3 lightposition;
uniform vec3 viewposition;

void main()
{
	FragColor = vec4(outColor, 1.0);

	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightcolor;


	//direccion de luz
	vec3 norm = normalize(outNormal);
	vec3 lightdir = normalize(lightposition - outFragPos);
	//luz difusa
	float diff = max(dot(norm, lightdir), 0.0);
	vec3 diffuse = diff * lightcolor;

	//valor luz especular
	float specularstrength = 0.5;
	vec3 viewdir = normalize(viewposition - outFragPos);
	vec3 reflectdir = reflect(lightdir, norm);
	float spec = pow(max(dot(viewdir, reflectdir), 0.0), 32);
	vec3 specular = specularstrength * spec * lightcolor;

	//resultado del color
	vec3 result = (ambient + diffuse + specular) * outColor;
	FragColor = vec4(result, 1.0);


}