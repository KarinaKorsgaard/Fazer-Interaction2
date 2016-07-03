
uniform float evenColor;
uniform float addthis;
void main() {

    gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_PointSize   = gl_Vertex.z * addthis;
	
    
    //float alpha   = gl_Normal.y;
    
   // gl_PointSize  = gl_Vertex.z*2.;
    vec4 col;

    if(evenColor>0.5){
        col =gl_Color*vec4(vec3(1.),0.5);
    }else{
        vec3 c = gl_Normal / 255.;
        col =gl_Color*vec4(vec3(c),10.);
    }
    
  

    
    gl_FrontColor =col;
    

}