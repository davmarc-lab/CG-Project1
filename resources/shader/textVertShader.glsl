#version 420 core

layout(location = 0) in vec4 vertex;

out VS_OUT {
    vec2 texCoord;
} vs_out;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
    mat4 proj;
};

void main() {
    gl_Position = proj * vec4(vertex.xy, 0., 1.0f);
    vs_out.texCoord = vertex.zw;
}
