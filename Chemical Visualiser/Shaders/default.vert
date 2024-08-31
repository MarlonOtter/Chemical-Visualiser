#version 330 core

layout (location = 0) in vec3 aPos;         //postion
layout (location = 1) in vec3 aNormal;      //normals
layout (location = 2) in vec3 aColour;      //vertex colour
layout (location = 3) in vec2 aTexCoord;    //UV coordinates

out vec3 crntPos;
out vec3 normals;
out vec3 colour;
out vec2 texCoord;

uniform mat4 camMatrix;

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 model;

void main()
{
    crntPos = vec3(model * vec4(aPos, 1.0f));
    //output the position of the vertex
    
    normals = aNormal;
    colour = aColour;
    texCoord = aTexCoord;

    gl_Position = camMatrix * vec4(crntPos, 1.0f);
}