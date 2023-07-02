#version 330 compatibility


struct light {
	vec2 position;
	vec3 color;
	float intensity;
};

in vec4 vert_pos;

layout(location = 0) out vec4 FragColor;

uniform sampler2D texture;

uniform int lightsCount;
uniform light lights[20];
uniform float ratio;

//Ambient light
uniform vec4 ambient_light;

void main()
{
	vec2 lightToFrag;
	vec3 color_intensity;
	float vecLength;

	for (int i = 0; i < lightsCount; i++) {
		//Convert light to view coords & Calculate the vector from light to pixel (Make circular)
		lightToFrag = (gl_ModelViewProjectionMatrix * vec4(lights[i].position, 0, 1)).xy - vert_pos.xy;
		lightToFrag.y = lightToFrag.y / ratio;

		//Length of the vector (distance)
		vecLength = clamp(length(lightToFrag) / lights[i].intensity + 0.07, 0, 1);
		color_intensity.r += clamp(lights[i].color.r - sqrt(vecLength), 0, 1);
		color_intensity.g += clamp(lights[i].color.g - sqrt(vecLength), 0, 1);
		color_intensity.b += clamp(lights[i].color.b - sqrt(vecLength), 0, 1);
	}

    // lookup the pixel in the texture

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color and lighting

	FragColor = gl_Color * pixel * (clamp(ambient_light + vec4(color_intensity.r, color_intensity.g, color_intensity.b, 1), 0, 1));

}