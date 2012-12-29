attribute vec4 qt_Vertex;
attribute vec4 qt_MultiTexCoord0;
uniform mat4 qt_ModelViewProjectionMatrix;
varying vec4 qt_TexCoord0;

varying vec3 vVertex;
void main()
{
    gl_Position = ftransform();
    vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
}
