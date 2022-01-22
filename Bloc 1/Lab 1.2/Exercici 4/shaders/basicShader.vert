#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 fcolor;

void main()  {
    gl_Position = vec4 (vertex, 1.0);
    fcolor = color;
}
