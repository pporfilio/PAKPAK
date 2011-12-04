varying vec3 ray;
uniform vec4 eye;
void main()
{	
        gl_Position = ftransform();
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
        vec4 eyeVec = gl_ProjectionMatrixInverse*eye;
        //vec4 eyeVec = gl_ProjectionMatrixInverse*vec4(0,0,-2.04,0);
        ray = normalize(vVertex - eyeVec.xyz); // Eye to vertex
}

