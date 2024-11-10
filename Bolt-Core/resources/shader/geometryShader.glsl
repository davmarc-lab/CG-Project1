#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords;

uniform float magnitude;

vec4 explode(vec4 pos, vec3 normal) {
    // change 0.1f with magnitude
    vec3 direction = normal * 0.1f; 
    return pos + vec4(direction, 0.0);
}

vec3 getNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {
    bool enableExplode = true;
    vec3 normal = getNormal();

    gl_Position = enableExplode ? explode(gl_in[0].gl_Position, normal) : gl_in[0].gl_Position;
    TexCoords = gs_in[0].texCoords;
    EmitVertex();

    gl_Position = enableExplode ? explode(gl_in[1].gl_Position, normal) : gl_in[1].gl_Position;
    TexCoords = gs_in[1].texCoords;
    EmitVertex();

    gl_Position = enableExplode ? explode(gl_in[2].gl_Position, normal) : gl_in[2].gl_Position;
    TexCoords = gs_in[2].texCoords;
    EmitVertex();

    EndPrimitive();
}