#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(0, 0, 1, 1);
    
    if (int(gl_FragCoord.y) % 20 >= 0 && int(gl_FragCoord.y) % 20 < 10) discard;
        
    if (gl_FragCoord.x < 354 && gl_FragCoord.y < 354)
        FragColor = vec4(1, 1, 0, 1);
        
    else if (gl_FragCoord.x > 354 && gl_FragCoord.y < 354)
        FragColor = vec4(0, 1, 0, 1);

    else if (gl_FragCoord.x < 354 && gl_FragCoord.y > 354)
        FragColor = vec4(1, 0, 0, 1);
    
}

