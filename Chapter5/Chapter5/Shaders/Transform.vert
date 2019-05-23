#version 330

//���[���h�ϊ��ƃr���[�ˉe��uniform�ϐ�
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//���_����
in vec3 inPosition;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}