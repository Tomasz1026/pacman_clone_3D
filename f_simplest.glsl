#version 330
//#extension GL_EXT_debug_printf : enable
//out vec4 pixelColor; //Output variable. Almost final pixel color.
out vec4 FragColor;

uniform sampler2D textureMap0;
uniform vec4 cameraPos;
//uniform vec4 lightPosition;
uniform int numberOfLights;
uniform vec4 lights[210];

//Varying variables
in vec2 iTexCoord0;
in vec4 aNormal;
in vec4 fragPos;

vec4 calculateLight() {
	
	vec4 lightColor = vec4(1, 1, 1, 1);

	//Normalized, interpolated vectors
	vec4 norm = normalize(aNormal);
	vec4 viewDir = normalize(cameraPos - fragPos);//vector towards the viewer in world space

	vec4 ambient = 0.2 * lightColor;


	vec4 result = ambient;
	float attenuation = 0.0f;

	for(int i = 0; i < numberOfLights; i++) {
		float distance = length(lights[i] - fragPos);

		if(distance > 4.0f) {
			continue;
		}

		
		vec4 lightDir = normalize(lights[i] - fragPos); //vector towards the light in world space
		

		//Reflected vector
		vec4 reflectDir = reflect(-lightDir, norm);		
		
		
		attenuation = 1.0f / (1.0f + 0.7f * distance + 1.8f * (distance * distance));
		

		float diff = max(dot(norm, lightDir), 0.0);
		vec4 diffuse = diff * lightColor;
		diffuse *= attenuation;

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec4 specular = spec * lightColor * 0.5;
		specular *= attenuation;
	

		result += (diffuse+specular);
	}
	return result;
}

void main(void) {

	vec4 kd = texture(textureMap0, iTexCoord0);

	FragColor = calculateLight() * kd;	
}
