uniform vec4 eye;
uniform sampler2D tex;
uniform int width;
uniform int height;
uniform float film_to_world[16];
uniform vec3 world_eye;
uniform float F_Z3;

const vec4 F_C = vec4(-.15, .16, .5, -.6);

const float EPSILON = .001;          //closeness to fractal
const float ITR = 300;               //number of iterations along ray
const int DEPTH = 12;                //number of fractal iterations
const float BREAK = 4.0;             //fractal escape bound
const float ep = .0001;              //for normal
const float M = 3.0;                 //bounding radius

float magnitude(vec4 v) {
    return sqrt(dot(v, v));
}


// vec4 is a built-in type and isn't a C++ template.
//vec4 QMultiply(vec4<REAL> A, vec4<REAL> B) {
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

    //    QGLShader::compile: "ERROR: 0:59: '*' :  wrong operand types no operation '*' exists that takes a left-hand operand of type 'const int' and a right operand of type 'float' (or there is no acceptable conversion)
    //    dist = magnitude(Zn) * log(magnitude(Zn)) / (2*magnitude(dZn));
    dist = magnitude(Zn) * log(magnitude(Zn)) / (2.0*magnitude(dZn));

    return foundFractal;
}

float IntersectFSphere(vec4 p, vec4 d, float r) {
    float t = -1.0;

    float A = d.x*d.x + d.y*d.y + d.z*d.z;
    float B = 2.0*p.x*d.x + 2.0*p.y*d.y + 2.0*p.z*d.z;
    float C = p.x*p.x + p.y*p.y + p.z*p.z - r*r;

    if (B*B - 4*A*C >= 0.0) {
        float t1 = (-B + sqrt(B*B - 4*A*C))/(2*A);
        float t2 = (-B - sqrt(B*B - 4*A*C))/(2*A);
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


//bool CalculateIntersection(vec4<REAL> &intersection, float &dist, vec4<REAL> d, vec4<REAL> start_p) {
bool CalculateIntersection(inout vec4 intersection, inout float dist, vec4 d, vec4 start_p, bool isShadow) {

    //QGLShader::compile: "ERROR: 0:71: '=' :  cannot convert from 'const int' to 'float'

    float t = 0.0;

    vec4 curPoint;
//    QGLShader::compile: "ERROR: 0:70: '=' :  cannot convert from 'const int' to 'float'
//  Casting is a function. I should have mentioned that.


    curPoint.x = start_p.x;
    curPoint.y = start_p.y;
    curPoint.z = start_p.z;
    curPoint.w = F_Z3;

    //QGLShader::compile: "ERROR: 0:85: '<' :  wrong operand types no operation '<' exists that takes a left-hand operand of type 'int' and a right operand of type 'const float' (or there is no acceptable conversion)
    //I hope/assume int() casts by truncating as in C
    //for (int i = 0; i < ITR; i++) {

    dist = 0.0;
    float curDist = 0.0;

    for (int i = 0; i < int(ITR); i++) {

        dist += curDist;

        if (isInJulia(curPoint, curDist)) {
            intersection = curPoint;
            return true;
        }

        //QGLShader::compile: "ERROR: 0:102: ')' : syntax error syntax error

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

/// ***********
//removed third parameter because it was a CS123Light struct
/// ***********
bool JuliaShadow(vec4 p, vec4 d) {

    vec4 intersection;
    float dist1;
    bool intersects = CalculateIntersection(intersection, dist1, d, p, true);

    return intersects;
}

vec4 CalculateNormal(vec4 point, vec4 d, float dist, vec4 start_p) {

    vec4 normal;
    point.w = 0.0;

    vec4 gx1,gx2,gy1,gy2,gz1,gz2;
    gx1 = point - vec4(ep,0.0,0.0,0.0);
    gx2 = point + vec4(ep,0.0,0.0,0.0);
    gy1 = point - vec4(0.0,ep,0.0,0.0);
    gy2 = point + vec4(0.0,ep,0.0,0.0);
    gz1 = point - vec4(0.0,0.0,ep,0.0);
    gz2 = point + vec4(0.0,0.0,ep,0.0);

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

    vec3 material_ambient = vec3(0., 0., .5);
    vec3 material_diffuse = vec3(.5, 0., 0.);
    vec3 material_specular = vec3(1., 1., 1.);

    vec4 light_pos = vec4(5., 5., -2., 1.0);
    vec3 light_color = vec3(.8,.8,0.0);

    vec4 light2_pos = vec4(-5., -5., -2., 1.0);
    vec3 light2_color = vec3(.8, .8, 0.);

    float KA = 1.;
    float KD = 1.;
    float KS = 1.;

    float Ir = KA*material_ambient.x;
    float Ig = KA*material_ambient.y;
    float Ib = KA*material_ambient.z;

    vec4 dir  = normalize(light_pos  - p);
    vec4 dir2 = normalize(light2_pos - p);

    vec4 R =  normalize((n*dot(n,-dir )*2.0 + dir )); //reflection vector
    vec4 R2 = normalize((n*dot(n,-dir2)*2.0 + dir2));

    vec4 V = normalize(d);
    float specExp = 5.0;

    float light_r;
    float light_g;
    float light_b;

    light_r = KD * material_diffuse * (light_color.x * max(0.0, dot(n, dir)) + light2_color.x * max(0.0, dot(n, dir2)));
    light_g = KD * material_diffuse * (light_color.y * max(0.0, dot(n, dir)) + light2_color.y * max(0.0, dot(n, dir2)));
    light_b = KD * material_diffuse * (light_color.z * max(0.0, dot(n, dir)) + light2_color.z * max(0.0, dot(n, dir2)));

    light_r += KS * material_specular.x * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
    light_g += KS * material_specular.y * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
    light_b += KS * material_specular.z * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));

    /// ***********
    //removed third parameter because it was a CS123Light struct
    /// ***********

    /*
    vec4 p_offset = p + n*.08;
    if (JuliaShadow(p_offset, dir)) {
        light_r *= .5;
        light_g *= .5;
        light_b *= .5;
    }*/

    Ir += light_r;
    Ig += light_g;
    Ib += light_b;


    vec4 color = vec4(Ir,Ig,Ib,0);

    //color = n;

    return color;
}

void main (void) {

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

    vec4 start_p = vec4(world_eye, 1.0);
    vec4 ray = normalize(p_film_world - start_p);

    float t = IntersectFSphere(start_p, ray, M);

    if (dot(start_p,start_p) < M + 7.0) { //weird thing happens where screen is black for a while;
        t = 0.0;
    }

    final_color = vec4(0.,0.,0.,1.);

    if (t != -1.0) {
        start_p = start_p + ray * t;

        vec4 intersection = vec4(0.0,0.0,0.0,0.0);
        float dist = 0.0;

        //QGLShader::compile: "ERROR: 0:332: 'CalculateIntersection' : no matching overloaded function found
        //means mismatched parameters. third parameter needed to change from ray.xyz to ray, since it wanted a vec4
        if (CalculateIntersection(intersection, dist, ray, start_p, false)) {
            final_color = CalculateLighting(intersection, dist, ray, start_p);
        }
    }

    gl_FragColor = final_color;
}
