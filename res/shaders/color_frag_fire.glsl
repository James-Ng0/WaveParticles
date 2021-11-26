#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec4 baseColor;
uniform vec4 emColor;
uniform vec4 amColor;
uniform vec4 diffColor;
uniform vec4 specColor;
uniform float shininess;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
	vec4 lightDir = vec4(0, 10, 0, 1);
	
	float ambientStrength = 0.1;
	vec4 ambient = ambientStrength * amColor;
	
	vec4 norm = normalize(vec4(f_in.normal, 0));
	lightDir = normalize(-lightDir);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * diffColor;

	//float specularStrength = 0.7;
	vec4 reflectDir = reflect(-lightDir, norm);
	vec4 viewDir = normalize(vec4(-f_in.position, 0));

	float spec = pow(max(dot(viewDir, reflectDir), 0.5), 32);
	vec4 specular = shininess * spec * specColor;
	
	vec4 result = (emColor + ambient + diffuse + specular);

	fb_color = result;
}