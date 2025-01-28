#version 330 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 normals;
in vec3 colour;
in vec2 texCoord;
in vec3 atomColour;

uniform vec3 camPos;


void main()
{
    //output the colour of the pixel
    FragColor = vec4(atomColour, 1.0f);
}

