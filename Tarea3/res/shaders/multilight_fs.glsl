//#version 330 core
#version 330
//out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};


varying vec3 FragPos;
varying vec3 Normal;
varying vec4 Eye;
varying vec2 TexCoord; //descomentar para el texture
/*
in vec3 FragPos;
in vec3 Normal;
in vec4 Eye;
in vec2 TexCoord; //descomentar para el texture
*/
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;
uniform vec3 globalLightColor;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform bool isspointsource;
uniform int equation;
//uniform sampler2D texture_2D;  //descomentar para el texture
uniform sampler2D texture1;
uniform int Texture_app_flag;     //descomentar para cuando pase los comandos para la textura




const float fogDensity = 0.1f;
const vec4 fogColor = vec4(0.5, 0.5, 0.5, 1.0);

out vec4 FragColor; //comentar para 120


// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float getFogFactor(float fogCoord, int equation);
//vec4 Texture2D(int Texture_app_flag, vec4 color);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
 
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    if(isspointsource){
        result += CalcPointLight(pointLight, norm, FragPos, viewDir);
    }else{
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    }

    result = result + globalLightColor*material.ambient;
    float fogCoord = abs(Eye.z/Eye.w);
	//color = mix(col, fogColor, getFogFactor(fogCoord));	
	
	vec4 color = vec4(result, 1.0);
	
	if(equation != 0)
	{
		color = mix(color, fogColor, getFogFactor(fogCoord, equation));	
	}
	
	
	//FragColor = vec4(result, 1.0);
    gl_FragColor = color; 
    //gl_FragColor = Texture(Texture_app_flag, color); //descomentar para 120
    //FragColor = Texture2D(Texture_app_flag, color); //comentar para 120
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

//Get Fog Factor of the Fragment
float getFogFactor(float fogCoord, int equation)
{
	float result = 0.0;
	float start = 0.000001;
	float end = 18.0;
	//Linear
	if (equation == 1){
		//result = (end-fogCoord)/(start); 
		result = (end-fogCoord)/(end -start);
	}//Exponential Equation 01
	else if (equation == 2){
		result = exp(-fogDensity*fogCoord);
	} // Exponential Equation 02
	else{
		result = exp(-pow(fogDensity*fogCoord, 2.0)); 
	}
	result = 1.0f - clamp(result, 0.0f, 1.0f);
	return result;
}
/*
vec4 Texture2D(int Texture_app_flag, vec4 color)
{
    vec4 fColor;
    if (Texture_app_flag == 0)
    {
        fColor = color;
    }
    else if (Texture_app_flag == 2)
    {
        fColor = texture( texture1, TexCoord );
    }
    else if (Texture_app_flag == 1)
    {
        fColor = color * texture( texture1, TexCoord );
    }
    return fColor;    
}
*/


