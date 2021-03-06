@include "shaders/common/header.glsl"

///////////////////////////////////////////////////////////////////////////////
// general uniforms
///////////////////////////////////////////////////////////////////////////////
@include "shaders/common/gua_camera_uniforms.glsl"

///////////////////////////////////////////////////////////////////////////////
// video3d uniforms
///////////////////////////////////////////////////////////////////////////////
uniform int layer;
uniform int bbxclip;
uniform vec3 bbx_min;
uniform vec3 bbx_max;

///////////////////////////////////////////////////////////////////////////////
// input
///////////////////////////////////////////////////////////////////////////////
in vec2  texture_coord;
in vec3  pos_es;
in vec3  pos_cs;
in float depth;
in vec3  normal_es;

///////////////////////////////////////////////////////////////////////////////
// output
///////////////////////////////////////////////////////////////////////////////
layout (location=0) out vec4 out_color;

///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////
@include "shaders/common/pack_vec3.glsl"

// methods 

bool clip(vec3 p){
  if(p.x < bbx_min.x ||
     p.y < bbx_min.y ||
     p.z < bbx_min.z ||
     p.x > bbx_max.x ||
     p.y > bbx_max.y ||
     p.z > bbx_max.z){
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////
void main() 
{

  if(clip(pos_cs) && bbxclip > 0)
    discard;

#if 1
   // to cull away borders of the rgb camera view
   if(texture_coord.s > 0.975 || texture_coord.s < 0.025 ||
      texture_coord.t > 0.975 || texture_coord.t < 0.025) {
        discard;
   }
#endif

   float quality = 1.0 / (depth * depth);
   float dist_es = length(pos_es);
   float packed_normal = pack_vec3(normalize(normal_es));

   out_color = vec4(texture_coord, packed_normal, quality);

}
