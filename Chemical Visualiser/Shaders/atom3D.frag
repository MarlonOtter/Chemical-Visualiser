#version 330 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 normals;
in vec3 colour;
in vec2 texCoord;
in vec3 atomColour;

uniform vec3 lightColour;
uniform vec3 lightSource;


uniform vec3 camPos;

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.2f;

	// diffuse lighting
	vec3 normal = normalize(normals);
	//light comes from a set vector (point in oposite direction of desired)
	vec3 lightDirection = normalize(lightSource);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//specular lighting
	float specularLight = 0.0f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;

	//outputs pixel colour based on the texture and lighting
	return ((diffuse + ambient) + 1.0f * specular) * vec4(lightColour, 1.0f);
}

void main()
{
    //output the colour of the pixel
    FragColor = vec4(atomColour, 1.0f) * direcLight();
}

