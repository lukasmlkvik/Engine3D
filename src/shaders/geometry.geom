#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 fragmentColor;
out vec3 normalVec;

in DATA
{
    vec3 fragmentColor;
    vec3 normalVec;
} data_in[];

uniform mat4 projMat;

void main()
{
    vec3 vector0 = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 vector1 = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    vec4 surfaceNormal = vec4(normalize(cross(vector0,vector1)),0.0)*0.1;

    gl_Position = projMat * (gl_in[0].gl_Position + surfaceNormal);
    normalVec = data_in[0].normalVec;
    fragmentColor = data_in[0].fragmentColor;
    EmitVertex();

    gl_Position =  projMat * (gl_in[1].gl_Position + surfaceNormal);
    normalVec = data_in[1].normalVec;
    fragmentColor = data_in[1].fragmentColor;
    EmitVertex();

    gl_Position =  projMat * (gl_in[2].gl_Position + surfaceNormal);
    normalVec = data_in[2].normalVec;
    fragmentColor = data_in[2].fragmentColor;
    EmitVertex();

    EndPrimitive();
}