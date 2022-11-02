#version 330 core
//layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
//layout (location = 0) in vec4 in_Position; //--- 위치 변수: attribute position 0

uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	const vec4 vertex[6] = vec4[6] (vec4(-10.0 , -0.01, 10.0, 1.0),
	vec4(-10.0, -0.01, -10.0, 1.0),
	vec4(10.0, -0.01, 10.0, 1.0),
	vec4(-10.0, -0.01, -10.0, 1.0),
	vec4(10.0, -0.01, 10.0, 1.0),
	vec4(10.0, -0.01 ,-10.0, 1.0));

	gl_Position = projection * view  * vertex[gl_VertexID];
}