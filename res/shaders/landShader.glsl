#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 Rcols[125];
uniform sampler2D uTexture;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

 in vec3 pot_col;

const vec3 lightDir = vec3(0.25, 0.25, -1);
const vec3 lightColor = vec3(0.5,0.5,0.5);

// framebuffer output
out vec4 fb_color;

void main() {
	
	vec3 color = uColor;

	vec3 ambient = ambientStrength * lightColor;
	vec3 norm = normalize(f_in.normal);
	vec3 lightDir = normalize(-lightDir);
 	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(-f_in.position);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * color;


	// output to the frambuffer
	fb_color = vec4(result, 1.0);
}