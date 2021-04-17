
// precision highp vec2
// uniform vec2 res;
uniform sampler2D screenTexture;
uniform highp vec2 resolution;

highp vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  highp vec4 color = vec4(0.0);
  highp vec2 off1 = vec2(1.411764705882353) * direction;
  highp vec2 off2 = vec2(3.2941176470588234) * direction;
  highp vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}

void main()
{ 
    // Normalized pixel coordinates (from 0 to 1)
    highp vec2 uv = gl_FragCoord.xy/resolution.xy;
    // if(texture2D(screenTexture, uv) == vec4(0,0,0,1.))
    //     discard;   
    uv /= 4.;
    highp vec4 Color = blur13(screenTexture, uv, resolution, vec2(0,0.5));

    // Color = (Color / vec4(2.,2.,2.,1.)) + (texture2D(screenTexture, uv) / vec4(2.,2.,2.,1.));

    gl_FragColor = Color;
}