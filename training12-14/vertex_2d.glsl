#version 330
//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
//layout (location = 1) in vec3 in_Color; //--- 컬러 변수: attribute position 1
uniform mat4 xyzTransform

//out vec3 out_Color; //--- 프래그먼트 세이더에게 전달
void main(void) 
{
xyz_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
//gl_Color = vec4(in_Color.x, in_Color.y, 0, 1.0);
//gl_Position = modelTransform * vec4(vPos, 1.0); //--- 좌표값에 modelTransform 변환을 적용한다.

//out_Color = in_Color;
}