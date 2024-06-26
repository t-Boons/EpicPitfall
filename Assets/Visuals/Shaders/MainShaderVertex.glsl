#version 330
in vec4 p;
in vec2 t;
out vec2 u;
out float sc;

void main()
{
    u=t;
    gl_Position=vec4(p.x, p.y, p.z, p.a);
}