#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;
layout (location = 2) in vec3 normal;

out vec3 fragmentColor;
out vec3 normalVec;

uniform mat4 projMat;

uniform mat4 modelTransform;

void main()
{
    gl_Position = projMat * modelTransform * vec4(vertexPos, 1.0);
    normalVec = normalize(mat3(modelTransform)*normal);
    fragmentColor = vertexColor;
}