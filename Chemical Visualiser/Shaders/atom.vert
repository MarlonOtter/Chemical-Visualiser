#version 330 core

layout (location = 0) in vec3 aPos;         //postion
layout (location = 1) in vec3 aNormal;      //normals
layout (location = 2) in vec3 aColour;      //vertex colour
layout (location = 3) in vec2 aTexCoord;    //UV coordinates

layout (location = 4) in vec3 aAtomColour; // the colour of the atom based on its element
layout (location = 5) in mat4 aMatrix;  // matrix transformation for that instance



out vec3 crntPos;
out vec3 normals;
out vec3 colour;
out vec2 texCoord;
out vec3 atomColour;

//camera perspective and such
uniform mat4 camMatrix;

//transformations
uniform mat4 meshMat4;

void main()
{
    crntPos = vec3(aMatrix * meshMat4 * vec4(aPos, 1.0f));
    //output the position of the vertex
    
    normals = aNormal;
    colour = aColour;
    texCoord = aTexCoord;
    atomColour = aAtomColour;

    gl_Position = camMatrix * vec4(crntPos, 1.0f);
}