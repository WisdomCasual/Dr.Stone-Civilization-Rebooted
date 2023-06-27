#version 330 compatibility

layout(location = 0) in vec4 position;
out vec4 vert_pos;

void main()
{
    // transform the vertex position
	vert_pos = gl_ModelViewProjectionMatrix * position;
    gl_Position = gl_ModelViewProjectionMatrix * position;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}