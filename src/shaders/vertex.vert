#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;
layout (location = 2) in vec3 normal;

out DATA
{
    vec3 fragmentColor;
    vec3 normalVec;
} data_out;

uniform mat4 projMat;

uniform mat4 modelTransform;

void main()
{
    gl_Position = modelTransform * vec4(vertexPos, 1.0);
    data_out.normalVec = normalize(mat3(modelTransform)*normal);
    data_out.fragmentColor = vertexColor;
}