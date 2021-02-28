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

uniform vec3 u_OutlineColor;

uniform sampler2D u_texture;

void main()
{
    float radius = 0.01;
    vec4 org = vec4(0.0);
    org = v_fragmentColor * texture2D(u_texture, v_texCoord);
    vec3 color = vec3(0.0, 0.8, 1.0);
    vec3 gray = vec3(0.299, 0.587, 0.114);
    float grayCol;// = dot(org, gray);
    grayCol = org.x * gray.x + org.y * gray.y + org.z * org.z;
    vec3 retCol; 
    
    // êFí≤êÆ
    retCol.r = clamp(grayCol * color.r, 0.0, 1.0);
    retCol.g = clamp(grayCol * color.g, 0.0, 1.0);
    retCol.b = clamp(grayCol * color.b, 0.0, 1.0);
    // à√ÇﬂÇ…
    retCol.r = max(retCol.r * 1.2, 0.0);
    retCol.g = max(retCol.g * 1.2, 0.0);
    retCol.b = max(retCol.b * 1.2, 0.0);
    gl_FragColor = vec4(retCol, org.a);
}