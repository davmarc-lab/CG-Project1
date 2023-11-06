#version 330 core

in vec4 vertexColor;
uniform vec2 resolution;
uniform int road;
out vec4 FragColor; // Colore del fragment

void main()
{
    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 red = vec4(1.0, 0, 0, 1.0);
    vec4 grey = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 background = vec4(0.87, 0.92, 1.0, 1.0);
    FragColor = vertexColor;

    vec2 fragCoord = gl_FragCoord.xy;

    // Calculate the pattern using modulo with a period of 100 pixels
    float pattern = mod(fragCoord.x, 100.0);

    // Set color based on the pattern
  
    vec4 color;
    if (pattern < 50.0 && (gl_FragCoord.y < 50.0 || (gl_FragCoord.y > 450 && gl_FragCoord.y < 500)))
        // First 50 pixels are red
        color = red;
    else if (pattern >= 10 && (gl_FragCoord.y < 50.0 || (gl_FragCoord.y > 450 && gl_FragCoord.y < 500)))
        // Next 50 pixels are white
        color = white;
    else if (gl_FragCoord.y < 500)
        color = grey;
    else
        color = background;

    FragColor = color;        
}
