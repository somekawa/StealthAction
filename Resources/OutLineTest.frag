/*
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2011 Ricardo Quesada
 * Copyright (c) 2012 Zynga Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 u_OutlineColor;
uniform float u_OutlineWidth;

uniform sampler2D u_texture;

void main()
{
    float radius = 0.01f;
    vec4 ret = vec4(0.0);
    vec4 normal = vec4(0.0);
    
    vec2 center = vec2(v_texCoord.x, v_texCoord.y );
    normal = texture2D(u_texture, center);
    if(normal.a != 0.0f)
    {
    


        ret += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - radius));
        ret += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + radius));
        ret += texture2D(u_texture, vec2(v_texCoord.x + radius, v_texCoord.y));
        ret += texture2D(u_texture, vec2(v_texCoord.x - radius, v_texCoord.y));
        //ret += texture2D(u_texture, vec2(v_texCoord.x - radius, v_texCoord.y ));
        //ret += texture2D(u_texture, vec2(v_texCoord.x + radius, v_texCoord.y ));
        //ret += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + radius));
        //ret += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - radius));
        ret.rgb =  vec3(0.0f,1.0f,0.0f) * ret.a;
        ret.a = 0.5f;
    
        normal = ( ret * (1.0 - normal.a)) + (normal * normal.a);
    }
    gl_FragColor = v_fragmentColor * normal;
    
}