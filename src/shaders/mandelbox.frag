uniform vec4 eye;
uniform int width;
uniform int height;
uniform vec3 world_eye;
uniform float halfPix;
uniform float F_Z3;
uniform vec3 F_C;
uniform samplerCube CubeMap;
uniform int reflections_enabled;
uniform int specular_enabled;
uniform int ss_enabled;
uniform vec3 material_specular;
uniform vec3 material_reflect;

uniform int coloring;
uniform bool fog_enabled;

uniform float ITR;
uniform float EPSILON;
uniform float BREAK;
uniform int DEPTH;

varying vec3 vVertex;

//vec3 material_specular = vec3(.5, .5, 1.);
//vec3 material_reflect = vec3(1., 1., 1.);
//const float EPSILON = .003;          //closeness to fractal
//const float ITR = 300.0;             //number of iterations along ray
//const int DEPTH = 20;                //number of fractal iterations       NEEDS TO BE CHANGED BASED ON CLOSENESS
//const float BREAK = 200.0;             //fractal escape bound
//const float ep = .1;                 //for normal
const float M = 3.0;                 //bounding radius

const float Scale = 2.0;
const float foldingLimit = 1.0;
const float minRadius = 0.5;
const float fixedRadius = 1.;
const float minRadius2 = minRadius*minRadius;
const float fixedRadius2 = fixedRadius*fixedRadius;

//int DEPTH = int(60.0 / dot(world_eye, world_eye)) + 15; //was + 10
//int DEPTH = 10;
float ep = .01 * float(DEPTH);
//int DEPTH = int(4.);

float magnitude(vec3 v) {
    return sqrt(dot(v, v));
}

//blog.hvidtfeldts.net
void sphereFold(inout vec3 z, inout float dz) {

        float r2 = dot(z,z);
        if (r2<minRadius2) {    /*
        Matrix4x4 film_to_world = m_camera->getFilmToWorld(width, height);

        float plane_depth = 2.0;

        float aspect = (float)width/(float)height;

        float half_height = plane_depth * tan(toRadians(m_camera->fovy/2));
        float half_width = half_height * aspect;

        Vector4 plane_ul = Vector4(-half_width, half_height, plane_depth, 1);
        Vector4 plane_ll = Vector4(-half_width, -half_height, plane_depth, 1);
        Vector4 plane_lr = Vector4(half_width, -half_height, plane_depth, 1);
        Vector4 plane_ur = Vector4(half_width, half_height, plane_depth, 1);



        t_ul = film_to_world*plane_ul;
        t_ll = film_to_world*plane_ll;
        t_lr = film_to_world*plane_lr;
        t_ur = film_to_world*plane_ur;
        */

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


void boxFold(inout vec3 z, inout float dz) {
	z = clamp(z, -foldingLimit, foldingLimit) * 2.0 - z;
}


////blog.hvidtfeldts.net
//float DE(vec3 z)
//{
//        vec3 offset = z;
//        z = vec3(0.,0.,0.);
//        float dr = 1.0;
//        for (int n = 0; n < DEPTH; n++) {
//                boxFold(z,dr);       // Reflect
//                sphereFold(z,dr);    // Sphere Inversion

//                z=Scale*z + offset;  // Scale & Translate
//                dr = dr*Scale+1.0;
//	}
//	float r = length(z);
//	return r/abs(dr);
//}



//You don't pass pointers in GLSL. Instead, you can specify
//that the value is copied into the function and then back out
//when the function is finished. See section 6.1 of the GLSL spec
//I didn't know this until just now. Glad to have found out.

bool isInMandelbox(vec3 p, inout float dist, inout float escape) {
    vec3 Zn = vec3(0.,0.,0.); //p;
    float dZn = 1.0;
    vec3 offset = p;

    bool foundFractal = true;

    for (int i = 0; i < DEPTH; i++) {

	boxFold(Zn, dZn);
	sphereFold(Zn, dZn);
	
        Zn = Scale*Zn + offset;
        dZn = Scale*dZn + 1.0;

        if (dot(Zn,Zn) > float(BREAK)) {
            foundFractal = false;
            break;
        }
    }

    //dist = magnitude(Zn)/abs(dZn);
    dist = magnitude(Zn)*log((magnitude(Zn)))/abs(dZn);
    escape = dot(Zn, Zn);
    return foundFractal;
}

bool CalculateIntersection(inout vec3 intersection, inout float dist, vec3 d, vec3 start_p, inout float escape) {
    float t = 0.0;

    vec3 curPoint;

    curPoint.x = start_p.x;
    curPoint.y = start_p.y;
    curPoint.z = start_p.z;

    dist = 0.0;
    float curDist = 0.0;

    for (int i = 0; i < int(ITR); i++) {

        dist += t;

        if (isInMandelbox(curPoint, curDist, escape)) {
            intersection = curPoint;
            return true;
        }

        t = max(curDist, float(EPSILON));
        curPoint.x += float(t)*d.x;
        curPoint.y += float(t)*d.y;
        curPoint.z += float(t)*d.z;

    }
    return false;
}

/// ***********
//removed third parameter because it was a CS123Light struct
/// ***********
//bool JuliaShadow(vec3 p, vec3 d) {

//    vec3 intersection;
//    float dist1;
//    bool intersects = CalculateIntersection(intersection, dist1, d, p);

//    return intersects;
//}

vec3 CalculateNormal(vec3 point, vec3 d, float dist, vec3 start_p) {

    vec3 normal;

    vec3 gx1,gx2,gy1,gy2,gz1,gz2;
    gx1 = point - vec3(ep,0.0,0.0);
    gx2 = point + vec3(ep,0.0,0.0);
    gy1 = point - vec3(0.0,ep,0.0);
    gy2 = point + vec3(0.0,ep,0.0);
    gz1 = point - vec3(0.0,0.0,ep);
    gz2 = point + vec3(0.0,0.0,ep);

    float dZn = 1.0;

    float d1,d2,d3,d4,d5,d6;


    float escape = 0.0;
    isInMandelbox(gx1, d1, escape);
    isInMandelbox(gx2, d2, escape);
    isInMandelbox(gy1, d3, escape);
    isInMandelbox(gy2, d4, escape);
    isInMandelbox(gz1, d5, escape);
    isInMandelbox(gz2, d6, escape);

/*
    d1 = DE(gx1);
    d2 = DE(gx2);
    d3 = DE(gy1);
    d4 = DE(gy2);
    d5 = DE(gz1);
    d6 = DE(gz2);
*/

    float gradX, gradY, gradZ;

    /*
    gradX = magnitude(d2) - magnitude(d1);
    gradY = magnitude(d4) - magnitude(d3);
    gradZ = magnitude(d6) - magnitude(d5);
    */

    gradX = d2 - d1;
    gradY = d4 - d3;
    gradZ = d6 - d5;


    normal = vec3(gradX, gradY, gradZ);

    return normalize(normal);
}

vec4 CalculateLighting(vec3 p, float dist, vec3 d, vec3 start_p) {
    vec3 n = normalize(CalculateNormal(p, d, dist, start_p));
    //return vec4(n,1.0);

    vec3 material_ambient = vec3(0., 0., .2);
    vec3 material_diffuse = vec3(.7, 0., .5);


    vec3 light_pos = vec3(5., 5., -2.);
    vec3 light_color = vec3(.2,0.,0.0);

    vec3 light2_pos = vec3(-5., -5., -2.);
    vec3 light2_color = vec3(.8, .8, 0.);

    float KA = 1.;
    float KD = 1.;
    float KS = 2.;
    float KR = 1.;

    float Ir = KA*material_ambient.x;
    float Ig = KA*material_ambient.y;
    float Ib = KA*material_ambient.z;

    vec3 dir  = normalize(light_pos  - p);
    vec3 dir2 = normalize(light2_pos - p);

    vec3 R =  normalize((n*dot(n,-dir )*2.0 + dir )); //reflection vector
    vec3 R2 = normalize((n*dot(n,-dir2)*2.0 + dir2));

    vec3 V = normalize(d);
    float specExp = 7.0;

    float light_r;
    float light_g;
    float light_b;


    light_r = KD * material_diffuse.x * (light_color.x * max(0.0, dot(n, dir)) + light2_color.x * max(0.0, dot(n, dir2)));
    light_g = KD * material_diffuse.y * (light_color.y * max(0.0, dot(n, dir)) + light2_color.y * max(0.0, dot(n, dir2)));
    light_b = KD * material_diffuse.z * (light_color.z * max(0.0, dot(n, dir)) + light2_color.z * max(0.0, dot(n, dir2)));

    //Specular
    if (specular_enabled == 1) {
        light_r += KS * material_specular.x * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
        light_g += KS * material_specular.y * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
        light_b += KS * material_specular.z * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
    }



    /// ***********
    //removed third parameter because it was a CS123Light struct
    /// ***********

    /*
    vec3 p_offset = p + n*.08;
    if (JuliaShadow(p_offset, dir)) {
        light_r *= .5;
        light_g *= .5;
        light_b *= .5;
    }*/

    Ir += light_r;
    Ig += light_g;
    Ib += light_b;


    vec4 color = vec4(Ir,Ig,Ib,1.);


    //Reflection
//    if (reflections_enabled == 1) {
//        vec3 r = reflect(d,n);
//        color += KR*textureCube( CubeMap, vec4(r, 1.0));

//        float lambertTerm = dot(n,dir);
//        if(lambertTerm > 0.0)
//        {
//                // Specular
//                color += textureCube( CubeMap, vec4(r, 1.0));
//        }
//    }

    //color = vec4(dist, dist, dist, 1.0);
    //color.xyz = color.xyz / (ITR * EPSILON);


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

    vec3 p_film = vec3(vVertex.x, vVertex.y, vVertex.z);


    vec3 start_p = world_eye;
    vec3 ray = normalize(p_film - start_p);

    vec4 final_color = vec4(0.,0.,0.,0.);
    vec4 fog_color = vec4(.3,.3,.5, 1.);

    vec3 intersection = vec3(0.0,0.0,0.0);
    float dist = 0.0;
    float escape = 0.0;

    if (CalculateIntersection(intersection, dist, ray, start_p, escape)) {

        //color baseed on use lighting
        if (coloring == 1) {
            final_color = CalculateLighting(intersection, dist, ray, start_p);
        //fake camera illumination
        } else if (coloring == 2) {
            if (dist > 5.0) {
                final_color = vec4(0.0, 0.0, 0.0, 1.0);
            } else {
                float tmp = ((5.0 - dist) * .2);
                final_color = vec4(tmp, tmp, tmp, 1.0);
            }
        //color based on how close a point is to being out of the set
        //color is darker if point is closer to escaping
        } else if (coloring == 3) {
            final_color = vec4(0.0, 1.0 - (escape / BREAK), 0.0, 1.0);
        //color based on normal
        }

        if (fog_enabled) {
            float blend = dist;
            blend = min(.6, blend);
            final_color = (1.- blend)*final_color + blend*fog_color;
            if ((final_color.x > 1.)||(final_color.y > 1.)||(final_color.z > 1.)) {
                final_color = fog_color;
            }
        }
    }

    //Please don't remove. Useful for debugging [Parker]
    //final_color = vec4(-vVertex.x, -vVertex.y, -vVertex.z, 1);

    gl_FragColor = final_color;
}
