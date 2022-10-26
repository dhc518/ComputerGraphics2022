#version 330 core

layout (location = 0) in vec3 vPos; //--- 응용 프로그램에서 받아온 도형 좌표값
uniform mat4 modelTransform; //--- 모델링 변환 행렬: uniform 변수로 선언


void main()
{
	gl_Position = modelTransform * vec4(vPos, 1.0); //--- 좌표값에 modelTransform 변환을 적용한다.
}