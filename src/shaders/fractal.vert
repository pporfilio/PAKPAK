varying vec3 vVertex;
void main()
{	
    gl_Position = ftransform();
    vVertex = gl_Position.xyz;
}

