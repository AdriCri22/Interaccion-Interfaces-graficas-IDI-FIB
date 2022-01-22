#version 330 core

out vec4 FragColor;
uniform int unifocus;
uniform vec3 colorTronc;
uniform vec3 colorArbre;

void main() {
    if (unifocus == 1) FragColor = vec4(colorTronc, 1.0);        
    else FragColor = vec4(colorArbre, 1.0);        
}
