#version 330 core
//--- out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- FragColor: ����� ������ ������ ������ ���۷� ���� ��.

uniform vec4 vColor = vec4 (0.1, 2, 0.8, 1.0);
out vec4 FragColor; //--- ���� ���

void main(void)
{
//FragColor = vec4 (vColor, 1.0);
FragColor = vColor;
}