#version 330 core

out vec4 FragColor; //output the pixel colour

in vec3 crntPos; //take in position of vertex in world space from vertex shader

uniform vec3 colour;
uniform vec3 camPos;

void main()
{
	// set the colour of the pixel
	FragColor = vec4(colour, 1.0f);
}