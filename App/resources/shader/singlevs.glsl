#version 420 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec4 aVertex;

layout(std140, binding = 0) uniform Matrices {
    mat4 proj;
    float time;
};

out VS_OUT {
    vec4 color;
    vec2 texCoord;
} vs_out;

out float Time;

// shader type
uniform int shaderProgram;

uniform mat4 model;

void defaultVertex() {
    gl_Position = proj * model * vec4(aPos, 1);
    vs_out.color = aColor;
}

void textVertex() {
    gl_Position = proj * vec4(aVertex.xy, 0.f, 1.0f);
    vs_out.texCoord = aVertex.zw;
}

void main() {
    Time = time;
    if (shaderProgram < 3) {
        defaultVertex();
    } else {
        textVertex();
    }
}
