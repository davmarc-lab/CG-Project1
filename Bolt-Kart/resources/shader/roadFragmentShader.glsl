#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

uniform vec2 resolution;
uniform float iTime;

vec2 iResolution = vec2(1600, 900);

float smin( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
    if (gl_FragCoord.y <= resolution.y)
    {
        vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
        vec4 red = vec4(1.0, 0, 0, 1.0);
        vec4 gray = vec4(0.5, 0.5, 0.5, 1.0);

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
    else
    {
#define tri(t, scale, shift) ( abs(t * 2. - 1.) - shift ) * (scale)
        vec2 R = iResolution.xy,
             uv = ( fragCoord - .5* R ) / R.y + .5;

        // sun
        float dist = length(uv-vec2(0.5, 0.8));
        float divisions = 6.0;
        float divisionsShift= 0.5;

        float pattern = tri(fract(( uv.y + 0.5)* 20.0), 2.0/  divisions, divisionsShift)- (-uv.y + 0.26) * 0.85;
        float sunOutline = smoothstep( 0.0,-0.015, max( dist - 0.315, -pattern)) ;

        vec3 c = sunOutline * mix(vec3( 4.0, 0.0, 0.2), vec3(1.0, 1.1, 0.0), uv.y);  

        // glow 
        float glow = max(0.0, 1.0 - dist * 1.25);
        glow = min(glow * glow * glow, 0.325);
        c += glow * vec3(1.5, 0.3, (sin(iTime)+ 1.0)) * 1.1;

        vec2 planeuv = uv;
        FragColor = vec4(c, 1.0);
    }
}