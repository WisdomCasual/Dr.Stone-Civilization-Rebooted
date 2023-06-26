#version 410 compatibility

in vec4 vert_pos;

uniform sampler2D texture;
uniform vec2 lightPos;
uniform float ratio;

//Ambient light
uniform vec4 light_level;

void main()
{

	//Convert light to view coords & Calculate the vector from light to pixel (Make circular)
	vec2 lightToFrag = (gl_ModelViewProjectionMatrix * vec4(lightPos, 0, 1)).xy - vert_pos.xy;
	lightToFrag.y = lightToFrag.y / ratio;

	//Length of the vector (distance)
	float vecLength = clamp(length(lightToFrag) * 2, 0, 1);

    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);


    // multiply it by the color and lighting

	float color_intensity = 1 - sqrt(vecLength);

	gl_FragColor = gl_Color * pixel * (clamp(light_level + vec4(color_intensity, color_intensity, color_intensity, 1), 0, 1));

}