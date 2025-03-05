#version 330 core

in vec4 color;
out vec4 FragColor;

in float Time;

#define GRID_SIZE 8.f
#define GRID_SQUARE 60.f
#define GRID_GAP GRID_SQUARE / 2.f + GRID_SIZE / 2.f

#define COLOR_CHANGE_DELAY 10.f

#define SAMPLE_DIST 10.f

float hash1(vec2 p) {
    return fract(sin(p.x*0.129898 + p.y*0.78233) * 43758.5453);
}

vec2 hash2(vec2 p) {
    return fract(sin(p * mat2(0.129898, 0.81314, 0.78233,  0.15926)) * 43758.5453);
}

float voronoiNoise(vec2 p) {
    vec2 cell = floor(p);
    vec2 res = cell;
    float dist = SAMPLE_DIST;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 sample = cell + vec2(x, y);
            float new_dist = length(hash2(sample) + sample - p);

            if (dist > new_dist) {
                dist = new_dist;
                res = sample;
            }
        }
    }
    return hash1(cell);
}

void main() {
    vec3 base = vec3(0.7, 0.3, 0.3);
    vec3 grid = vec3(0.2);
    
    float time = Time;
    // vec2 anim_offset = vec2(sin(time), cos(time));

    vec3 col = base;
    base.x *= (sin(COLOR_CHANGE_DELAY * radians(time)));
    
    float ypattern = mod(gl_FragCoord.x, GRID_SQUARE);
    float xpattern = mod(gl_FragCoord.y, GRID_SQUARE);

    vec2 p = vec2(xpattern, ypattern) / 900 * 200;

    bool yborder = ypattern < (GRID_SQUARE / 2.f) + GRID_GAP + GRID_SIZE && ypattern > (GRID_SQUARE / 2.f) + GRID_GAP - GRID_SIZE;
    bool xborder = xpattern < (GRID_SQUARE / 2.f) + GRID_GAP + GRID_SIZE && xpattern > (GRID_SQUARE / 2.f) + GRID_GAP - GRID_SIZE;

    if (xborder || yborder) {
        col = grid;
    } else {
        col = base * vec3(voronoiNoise(p));
    }

    FragColor = vec4(col, 1);
}
