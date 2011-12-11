uniform vec4 eye;
uniform int width;
uniform int height;
uniform vec3 world_eye;
uniform float F_Z3;
uniform vec4 F_C;
uniform samplerCube CubeMap;

varying vec3 vVertex;

vec3 material_specular = vec3(.5, .5, 1.);
vec3 material_reflect = vec3(1., 1., 1.);
const float EPSILON = .001;          // closeness to fractal
const float ITR = 300.0;             // number of iterations along ray
const int DEPTH = 15;                // number of fractal iterations
const float BREAK = 10.;             // fractal escape bound
const float ep = .001;               // for normal
const float M = 3.0;                 // bounding radius

const float Scale = 2.0;
//const float radius = 0.5;
const float foldingLimit = 1.0;
const float minRadius2 = 0.5;
const float fixedRadius2 = 0.5;

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


//blog.hvidtfeldts.net
void sphereFold(inout vec4 z, inout float dz) {
	float r2 = dot(z,z);
	if (r2<minRadius2) {
		// linear inner scaling
		float temp = (fixedRadius2/minRadius2);
		z *= temp;
		dz*= temp;
	} else if (r2<fixedRadius2) {
		// this is the actual sphere inversion
		float temp =(fixedRadius2/r2);
		z *= temp;
		dz*= temp;
	}
}


void boxFold(inout vec4 z, inout float dz) {
	z = clamp(z, -foldingLimit, foldingLimit) * 2.0 - z;
}

/*
//blog.hvidtfeldts.net
float DE(vec3 z)
{
        vec3 offset = z;
        float dr = 1.0;
        for (int n = 0; n < Iterations; n++) {
                boxFold(z,dr);       // Reflect
                sphereFold(z,dr);    // Sphere Inversion

                z=Scale*z + offset;  // Scale & Translate
                dr = dr*Scale+1.0;
	}
	float r = length(z);
	return r/abs(dr);
}
*/


//You don't pass pointers in GLSL. Instead, you can specify
//that the value is copied into the function and then back out
//when the function is finished. See section 6.1 of the GLSL spec
//I didn't know this until just now. Glad to have found out.

bool isInJulia(vec4 p, inout float dist) {
    vec4 Zn = p;
    float dZn = 1.0;

    bool foundFractal = true;
    
    for (int i = 0; i < DEPTH; i++) {

	//Zn = spherefold(boxfold(Zn, dZn)) + F_C;
	
	boxFold(Zn, dZn);
	sphereFold(Zn, dZn);
	
	Zn = Scale*Zn + F_C;
	dZn = Scale*dZn + 1.0;
	
	
	
//        Zn = QMultiply(Zn,Zn) + F_C;

//        dZn = QMultiply(Zn, dZn) * 2.;
//        dZn.x += 1.0;

        if (dot(Zn,Zn) > float(BREAK)) {
            foundFractal = false;
            break;
        }
    }

    dist = magnitude(Zn)/abs(dZn); //* log(magnitude(Zn)) / (2.0*magnitude(dZn));

    return foundFractal;
}
/*
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
*/

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

    float dZn = 1.0;

    for (int i = 0; i < DEPTH; i++) {
        /*
        gx1 = QMultiply(gx1,gx1) + F_C;
        gx2 = QMultiply(gx2,gx2) + F_C;
        gy1 = QMultiply(gy1,gy1) + F_C;
        gy2 = QMultiply(gy2,gy2) + F_C;
        gz1 = QMultiply(gz1,gz1) + F_C;
        gz2 = QMultiply(gz2,gz2) + F_C;
        */

        boxFold(gx1, dZn);
        sphereFold(gx1, dZn);
        gx1 = Scale*gx1 + F_C;

        boxFold(gx2, dZn);
        sphereFold(gx2, dZn);
        gx2 = Scale*gx2 + F_C;

        boxFold(gy1, dZn);
        sphereFold(gy1, dZn);
        gy1 = Scale*gy1 + F_C;

        boxFold(gy2, dZn);
        sphereFold(gy2, dZn);
        gy2 = Scale*gy2 + F_C;

        boxFold(gz1, dZn);
        sphereFold(gz1, dZn);
        gz1 = Scale*gz1 + F_C;

        boxFold(gz2, dZn);
        sphereFold(gz2, dZn);
        gz2 = Scale*gz2 + F_C;

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
    //n.w = 1.0;
    //return n;

    vec3 material_ambient = vec3(0., 0., .2);
    vec3 material_diffuse = vec3(.3, 0., .5);


    vec4 light_pos = vec4(5., 5., -2., 1.0);
    vec3 light_color = vec3(.2,0.,0.0);

    vec4 light2_pos = vec4(-5., -5., -2., 1.0);
    vec3 light2_color = vec3(.8, .8, 0.);

    float KA = .2;
    float KD = .5;
    float KS = 2.;
    float KR = 1.;

    float Ir = KA*material_ambient.x;
    float Ig = KA*material_ambient.y;
    float Ib = KA*material_ambient.z;

    vec4 dir  = normalize(light_pos  - p);
    vec4 dir2 = normalize(light2_pos - p);

    vec4 R =  normalize((n*dot(n,-dir )*2.0 + dir )); //reflection vector
    vec4 R2 = normalize((n*dot(n,-dir2)*2.0 + dir2));

    vec4 V = normalize(d);
    float specExp = 50.0;

    float light_r;
    float light_g;
    float light_b;


    light_r = KD * material_diffuse.x * (light_color.x * max(0.0, dot(n, dir)) + light2_color.x * max(0.0, dot(n, dir2)));
    light_g = KD * material_diffuse.y * (light_color.y * max(0.0, dot(n, dir)) + light2_color.y * max(0.0, dot(n, dir2)));
    light_b = KD * material_diffuse.z * (light_color.z * max(0.0, dot(n, dir)) + light2_color.z * max(0.0, dot(n, dir2)));

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


    vec4 color = vec4(Ir,Ig,Ib,1.);

    /*
    vec3 r = reflect(d,n);
    color += KR*textureCube( CubeMap, r);

    float lambertTerm = dot(n,dir);
    if(lambertTerm > 0.0)
    {
            // Specular
            color += textureCube( CubeMap,r);
    }
    */

    return color;

    /*
    vec3 r = reflect(-1,-n);
    vec4 final_color = textureCube( CubeMap, r);

    float lambertTerm = dot(n,dir);
    if(lambertTerm > 0.0)
    {
            // Specular
            final_color += textureCube( CubeMap,r);
    }


    return final_color; */
}

void main (void) {

    vec4 final_color = vec4(0,0,0,0);

    /*
        Slightly easier way to get a world-space position on the film plane:
        - Declare a varying in the shaders (i.e. 'varying vec3 film_plane')
        - In the vertex shader, set film_plane to the vertex's world space position
        - Use film_plane from the fragment shader

        OpenGL will automatically interpolate between the vertex positions you
        explicitly set in the vertex shader, so the value you look up in the
        fragment shader should be the real world-space position on the film plane
    */

    vec4 p_film = vec4(vVertex.x, vVertex.y, vVertex.z, 1.0);


    vec4 start_p = vec4(world_eye, 1.0);
    vec4 ray = normalize(p_film - start_p);


    float t = 0.0; // IntersectFSphere(start_p, ray, M);

    if (dot(start_p,start_p) < M + 7.0) { //weird thing happens where screen is black for a while;
        t = 0.0;
    }

    final_color = vec4(0.,0.,0.,0.);

    if (t != -1.0) {
        start_p = start_p + ray * t;

        vec4 intersection = vec4(0.0,0.0,0.0,0.0);
        float dist = 0.0;

        if (CalculateIntersection(intersection, dist, ray, start_p, false)) {
            final_color = CalculateLighting(intersection, dist, ray, start_p);
            gl_FragColor = final_color;
        }
    }

    //useful for debugging. Please don't remove [Parker]
    //final_color = vec4(-vVertex.x, -vVertex.y, -vVertex.z, 1);

    gl_FragColor = final_color;
}
