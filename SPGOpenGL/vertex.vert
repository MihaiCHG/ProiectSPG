
#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec3 vTexCoord;
uniform mat4 modelViewProjectionMatrix;
out vec4 coord;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
uniform int typeObject;
void main() 
{
	gl_Position = modelViewProjectionMatrix * vec4(vp, 1.0);
	coord = vec4(vp, 1.0);
	pos = vec3(gl_Position);
	if(typeObject==1)
	{
		norm = normalize(vp);	
	}
	else if(typeObject==2)
	{
		norm = vNorm;
	}
	if(typeObject==1){
		float s = atan(vp.x, vp.z) / 3.141592 + 0.5;
		float t = vp.y * 0.5 + 0.5;
		texCoord = vec2(s, t);
	}
	else
	{
		texCoord = vTexCoord.xy;
	}
}
