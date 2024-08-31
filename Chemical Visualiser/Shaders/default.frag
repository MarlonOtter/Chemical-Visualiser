#version 330 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 normals;
in vec3 colour;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec3 lightColour;

uniform vec3 camPos;

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.2f;

	// diffuse lighting
	vec3 normal = normalize(normals);
	//light comes from a set vector (point in oposite direction of desired)
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.5f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//specular lighting
	float specularLight = 0.8f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;

	//outputs pixel colour based on the texture and lighting
	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord) * specular) * vec4(lightColour, 1.0f);
}

void main()
{
    //output the colour of the pixel
    //FragColor = vec4(colour, 1.0f);
    FragColor = direcLight();
}

