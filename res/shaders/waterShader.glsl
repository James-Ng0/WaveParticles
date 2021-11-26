#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec4 uColor;
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


const vec4 lightDir = vec4(0,-1,0,0);

const vec4 lightColor = vec4(0.5,0.5,0.5,1);

// framebuffer output
out vec4 fb_color;

void main() {
	//vec3 inpos = normalize(-f_in.position)
	vec4 color = uColor;

	vec4 ambient = ambientStrength * lightColor;
	vec4 norm = vec4(normalize(f_in.normal),0);
	vec4 lightDir = vec4(normalize(-lightDir));
 	
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * lightColor;
	
	vec4 reflectDir = reflect(-lightDir, norm);
	vec4 viewDir = vec4(normalize(-f_in.position),0);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specular = specularStrength * spec * lightColor;

	vec4 result = (ambient + diffuse + specular) * color;


	// output to the frambuffer
	fb_color = result;
	}