//GLSL3.3��v��
#version 330

//���_�V�F�[�_�[����̃e�N�X�`�����W����
in vec2 fragTexCoord;

//�o�͐F
out vec4 outColor;

//�e�N�X�`���T���v�����O�p
uniform sampler2D uTexture;

void main()
{
	//�e�N�X�`������F���T���v�����O����
	outColor = texture(uTexture, fragTexCoord);
}