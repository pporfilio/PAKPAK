varying vec3 vVertex;
void main()
{
    gl_Position = ftransform();
    vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
}

