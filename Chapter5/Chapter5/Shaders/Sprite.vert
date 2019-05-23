#version 330

//���[���h�ϊ��ƃr���[�ˉe��uniform�ϐ�
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//���_����(0��position,1��texcoords)
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

//�o�͂Ƀe�N�X�`�����W��ǉ�
out vec2 fragTexCoord;

void main()
{
	//�ʒu���W�𓯎����W�n�ɕϊ�
	vec4 pos = vec4(inPosition, 1.0);
	//�ʒu�����[���h��ԂɁA�����ăN���b�v��Ԃɕϊ�
	gl_Position = pos * uWorldTransform * uViewProj;

	//�e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}