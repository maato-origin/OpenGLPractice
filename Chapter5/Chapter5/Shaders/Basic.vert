//GLSL3.3��v��
#version330

//���_�����������ɓ����
//���͈ʒu��񂾂�
in vec3 inPosition;

void main()
{
	//inPosition��gl_Position�ɂ��̂܂ܓn��
	gl_Position = vec4(inPosition,1.0);
}