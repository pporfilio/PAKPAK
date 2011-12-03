//uniform samplerCube CubeMap;
varying vec3 ray;


void main (void)
{
    vec4 final_color = vec4(0,0,0,0);
    final_color.xyz = ray;
    //TODO: based on the ray, and maybe eye if necessary, check if point
    //      is in the Julia set
    //      If it is, color that pixel a fixed color.
    gl_FragColor = final_color;
}
