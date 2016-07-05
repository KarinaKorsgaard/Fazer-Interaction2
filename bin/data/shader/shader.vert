
uniform float evenColor;
uniform float addthis;
void main() {

    
    gl_Position   = gl_ModelViewProjectionMatrix *vec4(gl_Vertex.x,gl_Vertex.y,0.,1. );
    gl_PointSize   = gl_Vertex.z  * addthis ;
   // gl_Vertex.z=1.;
    
    //gl_Position +=0.;
    //float alpha   = gl_Normal.y;
    
   // gl_PointSize  = gl_Vertex.z*2.;
    vec4 col;

    if(evenColor>0.5){
        col =gl_Color*vec4(vec3(1.),0.5);
    }else{
        vec3 c = gl_Normal / 255.;
        col =gl_Color*vec4(vec3(c),1.);
    }
    
  

    
    gl_FrontColor =col;
    

}