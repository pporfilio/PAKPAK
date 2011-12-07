uniform vec4 eye;
uniform sampler2D tex;
uniform int width;
uniform int height;
uniform float film_to_world[16];
uniform vec3 world_eye;
const vec4 F_C = vec4(-.15, .16, .5, -.6);
const float F_Z3 = .1;

const float EPSILON = 0.01;
const float ITR = 5.0 / EPSILON;
const int DEPTH = 30;
const int BREAK = 4;

float magnitude(vec4 v) {
    return sqrt(dot(v, v));
}


// vec4 is a built-in type and isn't a C++ template.
//vec4 QMultiply(vec4<REAL> A, vec4<REAL> B) {
vec4 QMultiply(vec4 A, vec4 B) {
    float a = A.x*B.x - A.y*B.y - A.z*B.z - A.w*B.w;
    float b = A.x*B.y + A.y*B.x + A.z*B.w - A.w*B.z;
    float c = A.x*B.z - A.y*B.w + A.z*B.x + A.w*B.y;
    float d = A.x*B.w + A.y*B.z - A.z*B.y + A.w*B.x;

    return vec4(a,b,c,d);
}


//You don't pass pointers in GLSL. Instead, you can specify
//that the value is copied into the function and then back out
//when the function is finished. See section 6.1 of the GLSL spec
//I didn't know this until just now. Glad to have found out.
//bool isInJulia(vec4 p, float &dist) {
bool isInJulia(vec4 p, inout float dist) {
    vec4 Zn = p;
    vec4 dZn = vec4(1.0,0.0,0.0,0.0);

    bool foundFractal = true;

    for (int i = 0; i < DEPTH; i++) {

        Zn = QMultiply(Zn,Zn) + F_C;

        dZn = QMultiply(Zn, dZn) * 2.;
        dZn.x += 1.0;

        //QGLShader::compile: "ERROR: 0:52: '>' :  wrong operand types no operation '>' exists that takes a left-hand operand of type 'float' and a right operand of type 'const int' (or there is no acceptable conversion)
        if (magnitude(Zn) > float(BREAK)) {
            foundFractal = false;
            break;
        }
    }

    //QGLShader::compile: "ERROR: 0:59: '*' :  wrong operand types no operation '*' exists that takes a left-hand operand of type 'const int' and a right operand of type 'float' (or there is no acceptable conversion)
//    dist = magnitude(Zn) * log(magnitude(Zn)) / (2*magnitude(dZn));
    dist = magnitude(Zn) * log(magnitude(Zn)) / (2.0*magnitude(dZn));

    return foundFractal;
}


//bool CalculateIntersection(vec4<REAL> &intersection, float &dist, vec4<REAL> d, vec4<REAL> start_p) {
bool CalculateIntersection(inout vec4 intersection, inout float dist, vec4 d, vec4 start_p) {

    //QGLShader::compile: "ERROR: 0:71: '=' :  cannot convert from 'const int' to 'float'
    //float t = 0;
    float t = 0.0;

    vec4 curPoint;
//    QGLShader::compile: "ERROR: 0:70: '=' :  cannot convert from 'const int' to 'float'
//  Casting is a function. I should have mentioned that.
//    curPoint.x = start_p.x + (float)t*d.x;
    curPoint.x = start_p.x + float(t)*d.x;
    curPoint.y = start_p.y + float(t)*d.y;
    curPoint.z = start_p.z + float(t)*d.z;

    curPoint.w = F_Z3;

    dist = 0.0;

    //QGLShader::compile: "ERROR: 0:85: '<' :  wrong operand types no operation '<' exists that takes a left-hand operand of type 'int' and a right operand of type 'const float' (or there is no acceptable conversion)
    //I hope/assume int() casts by truncating as in C
    //for (int i = 0; i < ITR; i++) {
    for (int i = 0; i < int(ITR); i++) {

        curPoint.x += float(t)*d.x;
        curPoint.y += float(t)*d.y;
        curPoint.z += float(t)*d.z;
        curPoint.w = F_Z3;

        float curDist;
        dist += curDist;    // TODO: this doesn't make much sense -- should it be in the if statement?

        if (isInJulia(curPoint, curDist)) {
            intersection = curPoint;
            return true;
        }

        //QGLShader::compile: "ERROR: 0:102: ')' : syntax error syntax error
        //t = max(curDist, (float)EPSILON);
        t = max(curDist, float(EPSILON));

    }
    return false;
}

/// ***********
//removed third parameter because it was a CS123Light struct
/// ***********
bool JuliaShadow(vec4 p, vec4 d) {

    vec4 intersection;
    float dist;
    bool intersects = CalculateIntersection(intersection, dist, d, p);

    return intersects;
}

vec4 CalculateNormal(vec4 point, vec4 d, float dist, vec4 start_p) {
    vec4 p1, p2, p3, p4, p5, p6;
    vec4 d1, d2, d3, d4, d5, d6;

    vec4 normal;

    float ep = .001;

    bool found = true;

    d1 = d;
    d1.x = d.x + ep;
    d2 = d;
    d2.x = d.x - ep;
    d3 = d;
    d3.y = d.y + ep;
    d4 = d;
    d4.y = d.y - ep;
    d5 = d;
    d5.z = d.z + ep;
    d6 = d;
    d6.z = d.z - ep;

    float dist1, dist2, dist3, dist4, dist5, dist6;

    bool I1 = CalculateIntersection(p1, dist1, d1, start_p);
    bool I2 = CalculateIntersection(p2, dist2, d2, start_p);
    bool I3 = CalculateIntersection(p3, dist3, d3, start_p);
    bool I4 = CalculateIntersection(p4, dist4, d4, start_p);
    bool I5 = CalculateIntersection(p5, dist5, d5, start_p);
    bool I6 = CalculateIntersection(p6, dist6, d6, start_p);


    if (!I1) {
        dist1 = 0.;
    }
    if (!I2) {
        dist2 = 0.;
    }
    if (!I3) {
        dist3 = 0.;
    }
    if (!I4) {
        dist4 = 0.;
    }
    if (!I5) {
        dist5 = 0.;
    }
    if (!I6) {
        dist6 = 0.;
    }
    normal.x = dist1 - dist2;
    normal.y = dist3 - dist4;
    normal.z = dist5 - dist6;

    //QGLShader::compile: "ERROR: 0:177: '==' :  wrong operand types no operation '==' exists that takes a left-hand operand of type 'float' and a right operand of type 'const int' (or there is no acceptable conversion)
    if ((normal.x == 0.0)&&(normal.y == 0.0)&&(normal.z == 0.0)) {
        //no printing : D
        //cout << "no intersections for normal calculation" << endl;
        normal.x = 1.;
        normal.y = 1.;
        normal.z = 1.;
    }

    normal.w = 0.;

    return normalize(normal);
}

vec4 CalculateLighting(vec4 p, float dist, vec4 d, vec4 start_p) {

    vec4 n = normalize(CalculateNormal(p, d, dist, start_p));

    vec3 material_ambient = vec3(.3, .1, .1);
    vec3 material_diffuse = vec3(.1, .1, 1.);
    vec3 material_specular = vec3(1., 1., 1.);
    /*
    CS123SceneMaterial mat;
    mat.cAmbient.r = .3;
    mat.cAmbient.g = .1;
    mat.cAmbient.b = .1;

    mat.cDiffuse.r = .1;
    mat.cDiffuse.g = .1;
    mat.cDiffuse.b = 1.;

    mat.cSpecular.r = 1.;
    mat.cSpecular.g = 1.;
    mat.cSpecular.b = 1.;
    */

    vec4 light_pos = vec4(5,5,5,1);
    vec3 light_color = vec3(1.,1.,1.);
    /*
    CS123SceneLightData light;
    light.pos = vec4(5,5,5,1);
    light.color.r = 1.;
    light.color.g = 1.;
    light.color.b = 1.;
    light.type = LIGHT_POINT;
    */

    float KA = 1.;
    float KD = 1.;
    float KS = 1.;

    float Ir = KA*material_ambient.x;
    float Ig = KA*material_ambient.y;
    float Ib = KA*material_ambient.z;

    vec4 dir;
    dir = normalize((light_pos - p));

    //QGLShader::compile: "ERROR: 0:235: 'dot' : methods not supported
//    vec4 R = normalize((n*n.dot(-dir)*2.0 + dir)); //reflection vector
    vec4 R = normalize((n*dot(n,-dir)*2.0 + dir)); //reflection vector

    vec4 V = normalize(d);
    float specExp = 1.0;

    float light_r;
    float light_g;
    float light_b;

    /*
    light_r = light_color.x * (KD*(material_diffuse.x) * max(0.0, n.dot(dir)) + KS*material_specular.x * pow(max(0.0, R.dot(V)),specExp));
    light_g = light_color.y * (KD*(material_diffuse.y) * max(0.0, n.dot(dir)) + KS*material_specular.y * pow(max(0.0, R.dot(V)),specExp));
    light_b = light_color.z * (KD*(material_diffuse.z) * max(0.0, n.dot(dir)) + KS*material_specular.z * pow(max(0.0, R.dot(V)),specExp));
    */

    light_r = light_color.x * max(0.0, dot(n, dir));
    light_g = light_color.y * max(0.0, dot(n, dir));
    light_b = light_color.z * max(0.0, dot(n, dir));

    vec4 p_offset = p + n*.08;

    /// ***********
    //removed third parameter because it was a CS123Light struct
    /// ***********
    if (JuliaShadow(p_offset, dir)) {
        light_r *= .5;
        light_g *= .5;
        light_b *= .5;
    }

    Ir += light_r;
    Ig += light_g;
    Ib += light_b;

    if (Ir > 1.0) {
        Ir = 1.0;
    }
    if (Ig > 1.0) {
        Ig = 1.0;
    }
    if (Ib > 1.0) {
        Ib = 1.0;
    }

    vec4 color = vec4(0,0,0,0);
    color.x = Ir;
    color.y = Ig;
    color.z = Ib;

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

    vec4 ray = normalize(p_film_world - vec4(world_eye, 1.0));



    vec4 intersection = vec4(0,0,0,0);
    float dist = 0.0;

    //QGLShader::compile: "ERROR: 0:332: 'CalculateIntersection' : no matching overloaded function found
    //means mismatched parameters. third parameter needed to change from ray.xyz to ray, since it wanted a vec4
    if (CalculateIntersection(intersection, dist, ray, vec4(world_eye, 1))) {
        final_color = CalculateLighting(intersection, dist, ray, vec4(world_eye, 1));       // ray.xyz = d and vec4(world_eye, 1) = Peye
    } else {
        //ERROR: 0:337: 'assign' :  cannot convert from 'const 4-component vector of float' to '3-component vector of float'
        //final_color.xyz = vec4(0.,0.,0.,0.);
        final_color = vec4(0.,0.,0.,0.);
    }

    // final_color.xyz = shootOneFractalRay(ray.xyz, vec4(world_eye, 1));

    //TODO: check that this value is 0-1, not 0-255
    gl_FragColor = final_color;
}

