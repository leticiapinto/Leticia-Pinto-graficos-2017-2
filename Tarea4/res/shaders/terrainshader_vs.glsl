#version 130
//#version 330 core

//layout ( location = 0 ) in vec3 position;
//layout (location = 1) in vec3 aTexCoord;
//attribute vec3 normal;
//attribute vec3 color;

attribute vec3 position;
attribute vec2 aTexCoord;


uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;
uniform vec3 viewPos;

varying vec2 TexCoord; //descomentar para el texture
varying float height;

//varying vec3 vNormal;
//varying vec3 vFragPos;
//varying vec3 vColor;

vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}
vec2 fade(vec2 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}

float cnoise(vec2 P){
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod(Pi, 289.0); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;
  vec4 i = permute(permute(ix) + iy);
  vec4 gx = 2.0 * fract(i * 0.0243902439) - 1.0; // 1/41 = 0.024...
  vec4 gy = abs(gx) - 0.5;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;
  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);
  vec4 norm = 1.79284291400159 - 0.85373472095314 * 
    vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11));
  g00 *= norm.x;
  g01 *= norm.y;
  g10 *= norm.z;
  g11 *= norm.w;
  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));
  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 1.2 * n_xy;
  //return 6.3 * n_xy;
}


void main()
{
    

    vec3 point = vec3(position.x + viewPos.x, position.y + viewPos.y , position.z + viewPos.z);
    vec2 P = vec2(point.x, point.z )/2  ;
    height = cnoise(P)*3;
    gl_Position = u_tProj * u_tView * u_tModel * vec4( point.x, height, point.z, 1.0f );
    //vFragPos = vec3( u_tModel * vec4( position, 1.0f ) );
    //vNormal = mat3(u_tModel)*normal;
    //vColor = color;

    TexCoord = vec2(aTexCoord.x, aTexCoord.y); //descomentar para el texture
}



//	Classic Perlin 2D Noise 
//	by Stefan Gustavson
//
