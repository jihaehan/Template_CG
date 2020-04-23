#version 400
in vec3 vColourPass[];
in vec2 vTexCoordPass[];
out vec3 vColour;
out vec2 vTexCoord;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
void main() {
for(int i = 0; i < 3; i++) {
gl_Position = gl_in[i].gl_Position;
vColour = vColourPass[i];
vTexCoord = vTexCoordPass[i];
EmitVertex();
}
EndPrimitive();
}