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

void main()
{
    //output the colour of the pixel
    FragColor = vec4(1.0, 0.0f, 1.0f, 1.0f);
}

