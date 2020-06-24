#version 400
out vec4 fragColor;
in vec2 texCoord;
in vec3 pos;
in vec3 norm;
uniform sampler2D texBowling;
uniform sampler2D texFloor;
uniform sampler2D texBall;

uniform vec3 col;

uniform int typeObject;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 normalMatrix;

vec3 lighting(vec3 objectColor, vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
{
	vec3 L =normalize(lightPos-pos);
	vec3 N = normalize(normal);
	vec3 V = normalize(viewPos-pos);
	vec3 R = reflect(-L,N);

	float diffCoef = max(dot(L,N),0.0);
	float specCoef = pow(max(dot(V,R),0.0), specPower);

	vec3 ambientColor = ambient * lightColor;
	vec3 diffuseColor = diffCoef * lightColor;
	vec3 specularColor = specCoef * specular * lightColor;
	vec3 col = ( ambientColor + diffuseColor + specularColor ) * objectColor; 

	return clamp(col, 0, 1);
}


void main() 
{
	vec3 objectColor;
	if(typeObject==3){
		objectColor = texture(texBowling,texCoord).rgb;
	}
	else if(typeObject==2){
		objectColor = texture(texFloor,texCoord).rgb;
	}
	else if(typeObject==1){
		objectColor = texture(texBall,texCoord).rgb;
	}
	else{
		objectColor = vec3(1.0,0,0);
	}
	vec3 lightColor = vec3(1.0, 0.8, 1.0);
	vec3 ambient = vec3(0.4);
	vec3 specular = vec3(0.8);
	float specPower = 64;
	vec3 normCalc =  vec3(normalMatrix * vec4(norm, 0.0));

	vec3 color = lighting(objectColor, pos, normCalc, lightPos, viewPos, ambient, lightColor, specular, specPower);
	if(typeObject!=4){
		fragColor = vec4(color,1.0);
	}
	else{
		fragColor = vec4(1.0,0.0,0.0,1.0);
	}
}
