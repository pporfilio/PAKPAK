/*
Authors: Aimei Kutt, Parker Porfilio, Anne Kenyon
*/

uniform vec4 eye;
uniform int width;
uniform int height;
uniform vec3 world_eye;
uniform float halfPix;
uniform float F_Z3;
uniform vec4 F_C;
uniform samplerCube CubeMap;
uniform int skybox_enabled;
uniform int reflections_enabled;
uniform int specular_enabled;
uniform int ss_enabled;

varying vec3 vVertex;

const float EPSILON = .001;          //closeness to fractal
const float ITR = 500.0;             //number of iterations along ray
const int DEPTH = 10;
const float BREAK = 4.0;             //fractal escape bound
const float EP = 0.0001;
const float M = 3.0;                 //bounding radius

float magnitude(vec4 v) {
    return sqrt(dot(v, v));
}

vec4 QMultiply(vec4 A, vec4 B) {
    float a,b,c,d;

    if (A == B) {
        a = A.x*B.x - A.y*B.y - A.z*B.z - A.w*B.w;
        b = 2.0*A.x*A.y;
        c = 2.0*A.x*A.z;
        d = 2.0*A.x*A.w;
    }
    else {
        a = A.x*B.x - A.y*B.y - A.z*B.z - A.w*B.w;
        b = A.x*B.y + A.y*B.x + A.z*B.w - A.w*B.z;
        c = A.x*B.z - A.y*B.w + A.z*B.x + A.w*B.y;
        d = A.x*B.w + A.y*B.z - A.z*B.y + A.w*B.x;
    }

    return vec4(a,b,c,d);
}


//You don't pass pointers in GLSL. Instead, you can specify
//that the value is copied into the function and then back out
//when the function is finished. See section 6.1 of the GLSL spec
//I didn't know this until just now. Glad to have found out.

bool isInJulia(vec4 p, inout float dist) {
    vec4 Zn = p;
    vec4 dZn = vec4(1.0,0.0,0.0,0.0);

    bool foundFractal = true;

    for (int i = 0; i < DEPTH; i++) {

        Zn = QMultiply(Zn,Zn) + F_C;

        dZn = QMultiply(Zn, dZn) * 2.;
        dZn.x += 1.0;

        if (dot(Zn,Zn) > float(BREAK)) {
            foundFractal = false;
            break;
        }
    }

   dist = magnitude(Zn) * log(magnitude(Zn)) / (2.0*magnitude(dZn));

    return foundFractal;
}

float IntersectFSphere(vec4 p, vec4 d, float r) {
    float t = -1.0;

    float A = d.x*d.x + d.y*d.y + d.z*d.z;
    float B = 2.0*p.x*d.x + 2.0*p.y*d.y + 2.0*p.z*d.z;
    float C = p.x*p.x + p.y*p.y + p.z*p.z - r*r;

    if (B*B - 4.0*A*C >= 0.0) {
        float t1 = (-B + sqrt(B*B - 4.0*A*C))/(2.0*A);
        float t2 = (-B - sqrt(B*B - 4.0*A*C))/(2.0*A);
        if ((t1 >= 0.0)&&(t2 >= 0.0)) {
            t = min(t1,t2);
        }
        else if ((t1 >= 0.0)&&(t2 < 0.0)) {
            t = t1;
        }
        else if ((t2 >= 0.0)&&(t1 < 0.0)) {
            t = t2;
        }
    }
    return t;
}


bool CalculateIntersection(inout vec4 intersection, inout float dist, vec4 d, vec4 start_p, bool isShadow) {

    float t = 0.0;

    vec4 curPoint;
    curPoint.x = start_p.x;
    curPoint.y = start_p.y;
    curPoint.z = start_p.z;
    curPoint.w = F_Z3;

    dist = 0.0;
    float curDist = 0.0;

    for (int i = 0; i < int(ITR); i++) {

        dist += curDist;

        if (isInJulia(curPoint, curDist)) {
            intersection = curPoint;
            return true;
        }

        t = max(curDist, float(EPSILON));

        curPoint.x += float(t)*d.x;
        curPoint.y += float(t)*d.y;
        curPoint.z += float(t)*d.z;

        if (t < float(EPSILON)) {
            break;
        }

    }
    return false;
}

bool JuliaShadow(vec4 p, vec4 d) {

    vec4 intersection;
    float dist1;
    bool intersects = CalculateIntersection(intersection, dist1, d, p, true);

    return intersects;
}

vec4 CalculateNormal(vec4 point, vec4 d, float dist, vec4 start_p) {

    vec4 normal;
    point.w = 0.0;

    vec4 gx1 = point,gx2 = point,gy1 = point,gy2 = point,gz1 = point,gz2 = point;

    gx1.x -= EP;
    gx2.x += EP;
    gy1.y -= EP;
    gy2.y += EP;
    gz1.z -= EP;
    gz2.z += EP;

    for (int i = 0; i < DEPTH; i++) {

        gx1 = QMultiply(gx1,gx1) + F_C;
        gx2 = QMultiply(gx2,gx2) + F_C;
        gy1 = QMultiply(gy1,gy1) + F_C;
        gy2 = QMultiply(gy2,gy2) + F_C;
        gz1 = QMultiply(gz1,gz1) + F_C;
        gz2 = QMultiply(gz2,gz2) + F_C;

    }

    float gradX, gradY, gradZ;
    gradX = magnitude(gx2) - magnitude(gx1);
    gradY = magnitude(gy2) - magnitude(gy1);
    gradZ = magnitude(gz2) - magnitude(gz1);

    normal = vec4(gradX, gradY, gradZ, 0.0);

    return normalize(normal);
}

vec4 CalculateLighting(vec4 p, float dist, vec4 d, vec4 start_p) {

    vec4 n = normalize(CalculateNormal(p, d, dist, start_p));

    vec3 material_ambient, material_diffuse;
    float KA, KD, KS, KR;

    //different coloring for when skybox is disabled
    if ((skybox_enabled == 0)||(reflections_enabled == 0)) {
        KA = .5;
        KD = 2.;
        KS = 2.;
        KR = 1.;
        material_ambient = vec3(.1, .3, .3);
        material_diffuse = vec3(.3, .8, 1.);
    } else {
        KA = .2;
        KD = .5;
        KS = 2.;
        KR = 1.;
        material_ambient = vec3(0., 0., .2);
        material_diffuse = vec3(.3, 0., .5);
    }

    vec4 light_pos = vec4(5., 5., -2., 1.0);
    vec3 light_color = vec3(.2,0.,0.0);

    vec4 light2_pos = vec4(-5., -5., -2., 1.0);
    vec3 light2_color = vec3(.5,.5,.5);

    /*** AMBIENT ***/
    float Ir = KA*material_ambient.x;
    float Ig = KA*material_ambient.y;
    float Ib = KA*material_ambient.z;
    /***************/

    vec4 dir  = normalize(light_pos  - p);
    vec4 dir2 = normalize(light2_pos - p);

    vec4 R =  normalize((n*dot(n,-dir )*2.0 + dir )); //reflection vector
    vec4 R2 = normalize((n*dot(n,-dir2)*2.0 + dir2));

    vec4 V = normalize(d);
    
    float specExp = 50.0;
    vec3 material_specular = vec3(0.5, 0.5, 1.0);

    float light_r;
    float light_g;
    float light_b;

    /*** DIFFUSE ***/
    light_r = KD * material_diffuse.x * (light_color.x * max(0.0, dot(n, dir)) + light2_color.x * max(0.0, dot(n, dir2)));
    light_g = KD * material_diffuse.y * (light_color.y * max(0.0, dot(n, dir)) + light2_color.y * max(0.0, dot(n, dir2)));
    light_b = KD * material_diffuse.z * (light_color.z * max(0.0, dot(n, dir)) + light2_color.z * max(0.0, dot(n, dir2)));
    /***************/

    /*** SPECULAR ***/
    if (specular_enabled == 1) {
        light_r += KS * material_specular.x * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
        light_g += KS * material_specular.y * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
        light_b += KS * material_specular.z * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
    }
    /****************/

    /*
    // *** SHADOW ***
    vec4 p_offset = p + n*.08;
    if (JuliaShadow(p_offset, dir)) {
        light_r *= .5;
        light_g *= .5;
        light_b *= .5;
    } */

    Ir += light_r;
    Ig += light_g;
    Ib += light_b;


    vec4 color = vec4(Ir,Ig,Ib,1.);

    /*** REFLECTION ***/
//    if (reflections_enabled == 1) {
//        vec3 r = reflect(d,n).xyz;
//        color += KR*textureCube( CubeMap, r);

//        float lambertTerm = dot(n,dir);
//        if(lambertTerm > 0.0)
//        {
//                // Specular
//                color += textureCube( CubeMap,r);
//        }
//    }

    return color;

}

void main (void) {

    /*
        Slightly easier way to get a world-space position on the film plane:
        - Declare a varying in the shaders (i.e. 'varying vec3 film_plane')
        - In the vertex shader, set film_plane to the vertex's world space position
        - Use film_plane from the fragment shader

        OpenGL will automatically interpolate between the vertex positions you
        explicitly set in the vertex shader, so the value you look up in the
        fragment shader should be the real world-space position on the film plane
    */

    vec4 start_p = vec4(world_eye, 1.0);

    vec4 p_film = vec4(vVertex.x, vVertex.y, vVertex.z, 1.0);
    vec4 p_film1 = vec4(vVertex.x - halfPix, vVertex.y + halfPix, vVertex.z, 1.0);
    vec4 p_film2 = vec4(vVertex.x + halfPix, vVertex.y + halfPix, vVertex.z, 1.0);
    vec4 p_film3 = vec4(vVertex.x + halfPix, vVertex.y - halfPix, vVertex.z, 1.0);
    vec4 p_film4 = vec4(vVertex.x - halfPix, vVertex.y - halfPix, vVertex.z, 1.0);

    vec4 ray = normalize(p_film - start_p);
    vec4 ray1 = normalize(p_film1 - start_p);
    vec4 ray2 = normalize(p_film2 - start_p);
    vec4 ray3 = normalize(p_film3 - start_p);
    vec4 ray4 = normalize(p_film4 - start_p);

    float t = IntersectFSphere(start_p, ray, M);

    if (dot(start_p,start_p) < M + 7.0) {
        t = 0.0;
    }

    vec4 final_color = vec4(0.,0.,0.,0.);

    if (t != -1.0) {
        start_p = start_p + ray * t;

        vec4 intersection = vec4(0.0,0.0,0.0,0.0);
        float dist = 0.0;

        if (CalculateIntersection(intersection, dist, ray, start_p, false)) {
            final_color = CalculateLighting(intersection, dist, ray, start_p);

            if (ss_enabled == 1) {
                vec4 intersection1 = vec4(0.0,0.0,0.0,0.0);
                vec4 intersection2 = vec4(0.0,0.0,0.0,0.0);
                vec4 intersection3 = vec4(0.0,0.0,0.0,0.0);
                vec4 intersection4 = vec4(0.0,0.0,0.0,0.0);
                float dist1 = 0.0;
                float dist2 = 0.0;
                float dist3 = 0.0;
                float dist4 = 0.0;
                bool inter1 = CalculateIntersection(intersection1, dist1, ray1, start_p, false);
                bool inter2 = CalculateIntersection(intersection2, dist2, ray2, start_p, false);
                bool inter3 = CalculateIntersection(intersection3, dist3, ray3, start_p, false);
                bool inter4 = CalculateIntersection(intersection4, dist4, ray4, start_p, false);

                float A = .4;
                float B = (1.0-A)/4.;

                final_color = A*final_color;

                if (inter1) {
                    vec4 color1 = CalculateLighting(intersection1, dist1, ray1, start_p);
                    final_color += B*color1;
                }
                if (inter2) {
                    vec4 color2 = CalculateLighting(intersection2, dist2, ray2, start_p);
                    final_color += B*color2;
                }
                if (inter3) {
                    vec4 color3 = CalculateLighting(intersection3, dist3, ray3, start_p);
                    final_color += B*color3;
                }
                if (inter4) {
                    vec4 color4 = CalculateLighting(intersection4, dist4, ray4, start_p);
                    final_color += B*color4;
                }

            }
        }
    }

    //useful for debugging. Please don't remove [Parker]
    //final_color = vec4(-vVertex.x, -vVertex.y, -vVertex.z, 1);

    gl_FragColor = final_color;
}
