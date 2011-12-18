/*
Authors: Aimei Kutt, Parker Porfilio, Anne Kenyon
*/

uniform vec4 eye;
uniform int width;
uniform int height;
uniform vec3 world_eye;
uniform float halfPix;
uniform int specular_enabled;
uniform int ss_enabled;
uniform int coloring;
uniform int colorScheme;
uniform bool fog_enabled;

uniform float ITR;
uniform float EPSILON;
uniform float BREAK;
uniform int DEPTH;
float EP = .0001 * float(DEPTH);

varying vec3 vVertex;

const float M = 3.0;                 //bounding radius
const float Scale = 2.0;
const float foldingLimit = 1.0;
const float minRadius = 0.5;
const float fixedRadius = 1.;
const float minRadius2 = minRadius*minRadius;
const float fixedRadius2 = fixedRadius*fixedRadius;


float magnitude(vec3 v) {
    return sqrt(dot(v, v));
}

//blog.hvidtfeldts.net
void sphereFold(inout vec3 z, inout float dz) {

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


void boxFold(inout vec3 z, inout float dz) {
	z = clamp(z, -foldingLimit, foldingLimit) * 2.0 - z;
}

bool isInMandelbox(vec3 p, inout float dist, inout float escape) {
    vec3 Zn = vec3(0.,0.,0.);
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

vec3 CalculateNormal(vec3 point, vec3 d, float dist, vec3 start_p) {

    vec3 normal;

    vec3 gx1 = point,gx2 = point,gy1 = point,gy2 = point,gz1 = point,gz2 = point;
    gx1.x -= EP;
    gx2.x += EP;
    gy1.y -= EP;
    gy2.y += EP;
    gz1.z -= EP;
    gz2.z += EP;

    float dZn = 1.0;

    float d1,d2,d3,d4,d5,d6;

    float escape = 0.0;
    isInMandelbox(gx1, d1, escape);
    isInMandelbox(gx2, d2, escape);
    isInMandelbox(gy1, d3, escape);
    isInMandelbox(gy2, d4, escape);
    isInMandelbox(gz1, d5, escape);
    isInMandelbox(gz2, d6, escape);

    float gradX, gradY, gradZ;

    gradX = d2 - d1;
    gradY = d4 - d3;
    gradZ = d6 - d5;


    normal = vec3(gradX, gradY, gradZ);

    return normalize(normal);
}

vec4 CalculateLighting(vec3 p, float dist, vec3 d, vec3 start_p) {
    vec3 n = CalculateNormal(p, d, dist, start_p);

    if (coloring == 4) {
        return vec4(n, 1.0);
    }

    vec3 V = normalize(d);

    /*** LIGHT 1 ***/
    vec3 light_pos = vec3(20., 20., -10.);
    vec3 dir  = normalize(light_pos  - p);
    vec3 R =  normalize((n*dot(n,-dir )*2.0 + dir )); //reflection vector

    /*** LIGHT 2 ***/
    vec3 light2_pos = vec3(-20., -20., -10.);
    vec3 dir2 = normalize(light2_pos - p);
    vec3 R2 = normalize((n*dot(n,-dir2)*2.0 + dir2));

    /*** COLOR STUFF ***/
    vec3 light_color, light2_color, material_ambient, material_diffuse, material_specular;
    float KA, KD, KS, KR, specExp;

    //based on color scheme
    if (colorScheme == 1) {
        light_color = vec3(.2,0.,0.0);
        light2_color = vec3(.8, .8, 0.);
        material_ambient = vec3(0.05, 0.05, .15);
        material_diffuse = vec3(.3, 0., .15);
        material_specular = vec3(.2, 0., 0.);
        specExp = 7.0;
        KA = 1.;
        KD = 1.;
        KS = 2.;
        KR = 1.;
    }
    else if (colorScheme == 2) {
        light_color = vec3(0.,.1,.3);
        light2_color = vec3(.5, .5, .5);
        material_ambient = vec3(.01, .01, .01);
        material_diffuse = vec3(.55, .55, .48);
        material_specular = vec3(.95, .95, .83);
        specExp = 7.0;
        KA = 1.;
        KD = 1.;
        KS = 1.;
        KR = 1.;
    }
    else if (colorScheme == 3) {
        light_color = vec3(1.,0.,.3);
        light2_color = vec3(.05, 0., 1.);
        material_ambient = vec3(.05, .2, .1);
        material_diffuse = vec3(.1, .5, .2);
        material_specular = vec3(.7, .05, .3);
        specExp = 7.0;
        KA = 1.;
        KD = 1.;
        KS = .3;
        KR = 1.;
    }

    /*** AMBIENT ***/
    float Ir = KA*material_ambient.x;
    float Ig = KA*material_ambient.y;
    float Ib = KA*material_ambient.z;

    float light_r;
    float light_g;
    float light_b;

    /*** DIFFUSE ***/
    light_r = KD * material_diffuse.x * (light_color.x * max(0.0, dot(n, dir)) + light2_color.x * max(0.0, dot(n, dir2)));
    light_g = KD * material_diffuse.y * (light_color.y * max(0.0, dot(n, dir)) + light2_color.y * max(0.0, dot(n, dir2)));
    light_b = KD * material_diffuse.z * (light_color.z * max(0.0, dot(n, dir)) + light2_color.z * max(0.0, dot(n, dir2)));

    /*** SPECULAR ***/
    if (specular_enabled == 1) {
        light_r += KS * material_specular.x * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
        light_g += KS * material_specular.y * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
        light_b += KS * material_specular.z * (pow(max(0.0, dot(R, V)), specExp) + pow(max(0.0, dot(R2, V)), specExp));
    }

    Ir += light_r;
    Ig += light_g;
    Ib += light_b;

    vec4 color = vec4(Ir,Ig,Ib,1.);
    return color;
}

void main (void) {

    vec3 p_film = vec3(vVertex.x, vVertex.y, vVertex.z);
    vec3 start_p = world_eye;
    vec3 ray = normalize(p_film - start_p);

    vec4 final_color;
    if ((coloring == 2)) {
        final_color = vec4(0.0,0.0,0.0,1.0);
    } else {
        final_color = vec4(0.,0.,0.,0.);
    }

    vec3 intersection = vec3(0.0,0.0,0.0);
    float dist = 0.0;
    float escape = 0.0;

    if (CalculateIntersection(intersection, dist, ray, start_p, escape)) {

        //color based on use lighting
        if (coloring == 1) {
            final_color = CalculateLighting(intersection, dist, ray, start_p);

            if (fog_enabled) {
                float blend = dist;

                vec4 fog_color; //based on color scheme
                if (colorScheme == 1) {
                    blend = clamp(blend, 0., .9);
                    fog_color = vec4(1.,clamp(dist*.7,.5,1.),0.,1.);
                } else if (colorScheme == 2) {
                    blend = clamp(blend, 0., .9);
                    fog_color = vec4(.7, .7, .65, 1.);
                    fog_color.xyz *= clamp(dist*.7, .7, 1.);
                } else if (colorScheme == 3) {
                    blend = clamp(blend, 0., .9);
                    fog_color = vec4(clamp(dist*.7,.5,.8), 1., .1, 1.);
                }

                final_color = (1.- blend)*final_color + blend*fog_color;
                if ((final_color.x > 1.)||(final_color.y > 1.)||(final_color.z > 1.)) {
                    final_color = fog_color;
                }
            }

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
        } else if (coloring == 4) {
            final_color = CalculateLighting(intersection, dist, ray, start_p);
        }
    }

    //Please don't remove. Useful for debugging [Parker]
    //final_color = vec4(-vVertex.x, -vVertex.y, -vVertex.z, 1);

    gl_FragColor = final_color;
}
