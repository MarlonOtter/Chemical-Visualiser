#version 330 core

out vec4 FragColor; //output the pixel colour

in vec3 crntPos; //take in position of vertex in world space from vertex shader

uniform vec3 colour;
uniform vec3 camPos;

void main()
{

	// set the colour of the pixel
	// this makes the lines taper off futher away from the camera
	float maxDist = 10.;
	float dist = length(crntPos - camPos);
	float mult = clamp(1/dist * maxDist, 0, 1);
	
	vec4 col = max(vec4(colour, 1.0f) * mult, vec4(0.15f, 0.15f, 0.15f, 1.0f));

	//this doesn't draw the pixel if it is the colour of the background or it should be replaced by an axis line
	if (col.r <= 0.15f || (crntPos.x == 0 && crntPos.z >= 0) || (crntPos.z == 0 && crntPos.x >= 0)) discard;
	FragColor = col;
}