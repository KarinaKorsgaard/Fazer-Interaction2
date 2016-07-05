void main() {

    gl_Position   = gl_ModelViewProjectionMatrix *vec4(gl_Vertex.x,gl_Vertex.y,0.,1. );
    gl_PointSize   = gl_Vertex.z  * 2. ;

    vec3 c = gl_Normal / 255.;
    gl_FrontColor =gl_Color*vec4(vec3(c),1.);

}