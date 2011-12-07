//uniform samplerCube CubeMap;
//varying vec3 ray;
uniform vec4 eye;
uniform sampler2D tex;
uniform int width;
uniform int height;
uniform float film_to_world[16];
uniform vec3 world_eye;
const vec4 F_C = vec4(-.15, .16, .5, -.6);
const float F_Z3 = .1;

float magnitude(vec4 v) {
    return sqrt(dot(v, v));
}

float intersectFSphere(vec4 p, vec3 d, float r) {
    float t = -1.0;

    float A = d.x*d.x + d.y*d.y + d.z*d.z;
    float B = 2.0*p.x*d.x + 2.0*p.y*d.y + 2.0*p.z*d.z;
    float C = p.x*p.x + p.y*p.y + p.z*p.z - r*r;


    if (B*B - 4.0*A*C >= 0.0) {
        float t1 = (-B + sqrt(B*B - 4.0*A*C))/(2.0*A);
        float t2 = (-B - sqrt(B*B - 4.0*A*C))/(2.0*A);
        if ((t1 >= 0.0) && (t2 >= 0.0)) {
            t = min(t1, t2);
        }
        else if ((t1 >= 0.0) && (t2 < 0.0)) {
            t = t1;
        }
        else if ((t2 >= 0.0) && (t1 < 0.0)) {
            t = t2;
        }
    }
    return t;
}


vec4 julia(vec4 Zn, int depth);


vec3 shootOneFractalRay(vec3 d, vec4 start_p) {
    float M = (1.0 + sqrt(1.0 + 4.0*magnitude(F_C))) * 0.5;
    float step_size = .05;

    float t = intersectFSphere(start_p, d, M*M - F_Z3*F_Z3);

    bool foundFractal = false;

    if (t > 1.0) {
        while ((!foundFractal) && (t < 40.0)) {
            vec4 curPoint = vec4(0.0);
            curPoint.xyz = start_p.xyz + t*d.xyz;
            curPoint.w = F_Z3;

            vec4 juliaFractal = julia(curPoint, 40);

            t = t + step_size;

            if (magnitude(juliaFractal) < 8.0) {
                foundFractal = true;
            }
        }
    } else {
        return vec3(1.0-t, 0, 0);
    }
    if (foundFractal) {
        return vec3(0, 1, 0);
    } else {
        return vec3(0, 0, 0);
    }

}


vec4 julia(vec4 Zn, int depth) {

    for (int i = 0; i < depth; i++) {
        float A = Zn.x*Zn.x - Zn.y*Zn.y - Zn.z*Zn.z - Zn.w*Zn.w + F_C.x;
        float B = 2.0*Zn.x*Zn.y + F_C.y;
        float C = 2.0*Zn.x*Zn.z + F_C.z;
        float D = 2.0*Zn.x*Zn.w + F_C.w;

        Zn = vec4(A,B,C,D);
        if (magnitude(Zn) > 8.0) {
            break;
        }
    }
    return Zn;
}

void main (void)
{
    vec4 final_color = vec4(0,0,0,0);

    vec4 sample = texture2D(tex, gl_TexCoord[0].st);

    int row = int(float(height)*sample.r);
    int col = int(float(width)*sample.g);

    vec4 p_film = vec4((2.0*float(col) / float(width)) - 1.0,
                       1.0 - ((2.0*float(row)) / float(height)),
                       -1.0,
                       1.0);

    mat4 film_to_world_transform = mat4(film_to_world[0],
                                            film_to_world[1],
                                            film_to_world[2],
                                            film_to_world[3],
                                            film_to_world[4],
                                            film_to_world[5],
                                            film_to_world[6],
                                            film_to_world[7],
                                            film_to_world[8],
                                            film_to_world[9],
                                            film_to_world[10],
                                            film_to_world[11],
                                            film_to_world[12],
                                            film_to_world[13],
                                            film_to_world[14],
                                            film_to_world[15]);

    vec4 p_film_world = film_to_world_transform * p_film;

    vec4 ray = normalize(p_film_world - vec4(world_eye, 1.0));

    //vec4 world_ray = film_to_world_transform * vec4(ray, 0);
    //world_ray = normalize(world_ray);
    //final_color.rgb = shootOneFractalRay(world_ray.xyz, vec4(world_eye, 1));

    final_color.xyz = shootOneFractalRay(ray.xyz, vec4(world_eye, 1));



//    final_color.b = sample.r;
//    final_color.r = sample.g;


//    gl_FragColor = sample;

    //final_color.b = 1.0;

    gl_FragColor = final_color;
}


