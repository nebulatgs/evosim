
// precision highp vec2
// uniform vec2 res;
uniform sampler2D screenTexture;
uniform highp vec2 resolution;

uniform bool horizontal;
// highp float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main(){
    highp vec2 TexCoords = gl_FragCoord.xy / resolution.xy;
    highp vec2 tex_offset = 1.0 / resolution.xy; // gets size of single texel
    highp vec3 result = texture2D(screenTexture, TexCoords).rgb * 0.227027; // current fragment's contribution
    if(horizontal)
    {
        // for(int i = 1; i < 5; ++i)
        // {
            result += texture2D(screenTexture, TexCoords + vec2(tex_offset.x * 1., 0.0)).rgb * 0.1945946;
            result += texture2D(screenTexture, TexCoords - vec2(tex_offset.x * 1., 0.0)).rgb * 0.1945946;

            result += texture2D(screenTexture, TexCoords + vec2(tex_offset.x * 2., 0.0)).rgb * 0.1216216;
            result += texture2D(screenTexture, TexCoords - vec2(tex_offset.x * 2., 0.0)).rgb * 0.1216216;

            result += texture2D(screenTexture, TexCoords + vec2(tex_offset.x * 3., 0.0)).rgb * 0.054054;
            result += texture2D(screenTexture, TexCoords - vec2(tex_offset.x * 3., 0.0)).rgb * 0.054054;

            result += texture2D(screenTexture, TexCoords + vec2(tex_offset.x * 4., 0.0)).rgb * 0.016216;
            result += texture2D(screenTexture, TexCoords - vec2(tex_offset.x * 4., 0.0)).rgb * 0.016216;
        // }
    }
    else
    {
    //     for(int i = 1; i < 5; ++i)
    //     {
    //         result += texture2D(screenTexture, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    //         result += texture2D(screenTexture, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    //     }
            result += texture2D(screenTexture, TexCoords + vec2(0.0, tex_offset.y * 1.)).rgb * 0.1945946;
            result += texture2D(screenTexture, TexCoords - vec2(0.0, tex_offset.y * 1.)).rgb * 0.1945946;

            result += texture2D(screenTexture, TexCoords + vec2(0.0, tex_offset.y * 2.)).rgb * 0.1216216;
            result += texture2D(screenTexture, TexCoords - vec2(0.0, tex_offset.y * 2.)).rgb * 0.1216216;

            result += texture2D(screenTexture, TexCoords + vec2(0.0, tex_offset.y * 3.)).rgb * 0.054054;
            result += texture2D(screenTexture, TexCoords - vec2(0.0, tex_offset.y * 3.)).rgb * 0.054054;

            result += texture2D(screenTexture, TexCoords + vec2(0.0, tex_offset.y * 4.)).rgb * 0.016216;
            result += texture2D(screenTexture, TexCoords - vec2(0.0, tex_offset.y * 4.)).rgb * 0.016216;
    }
    // gl_FragColor = vec4(result, 1.);
    gl_FragColor = vec4(1.);
}