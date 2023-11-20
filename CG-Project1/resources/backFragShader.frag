#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

uniform vec2 resolution;

void main()
{
    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 red = vec4(1.0, 0, 0, 1.0);
    vec4 gray = vec4(0.5, 0.5, 0.5, 1.0);

    vec2 fragCoord = gl_FragCoord.xy;

    // Calculate the pattern using module with a period of 100 pixels
    float pattern = mod(fragCoord.x, 100.0);

    // Set color based on the pattern
  
    vec4 color;

    float ylimitRoad = resolution.y- 50;
    float ylimit = resolution.y;
    
        if (pattern < 50.0 && (gl_FragCoord.y < 50.0 || (gl_FragCoord.y > ylimitRoad && gl_FragCoord.y < ylimit)))
        // First 50 pixels are red
        color = red;
    else if (pattern >= 10 && (gl_FragCoord.y < 50.0 || (gl_FragCoord.y > ylimitRoad && gl_FragCoord.y < ylimit)))
        // Next 50 pixels are white
        color = white;
    else if (gl_FragCoord.y < ylimit)
        color = gray;

    FragColor = color;
}
