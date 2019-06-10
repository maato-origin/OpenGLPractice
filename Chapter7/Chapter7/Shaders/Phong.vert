#version 330

//���[���h�ϊ��ƃr���[�ˉe��uniform�ϐ�
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//���_����(0�͈ʒu���W,1�͖@���x�N�g��,2�̓e�N�X�`�����W)
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

//�o�͂Ƀe�N�X�`�����W��ǉ�
out vec2 fragTexCoord;
//�@���i���[���h��ԁj
out vec3 fragNormal;
//�ʒu�i���[���h��ԁj
out vec3 fragWorldPos;

void main()
{
	//�ʒu���W�𓯎����W�n�ɕϊ�
	vec4 pos = vec4(inPosition, 1.0);
	//�ʒu�����[���h��Ԃɕϊ�����
	pos = pos * uWorldTransform;
	//���[���h��Ԃ̈ʒu��ۑ�
	fragWorldPos = pos.xyz;
	//�N���b�v��Ԃɕϊ�
	gl_Position = pos * uViewProj;

	//�@�������[���h��Ԃɕϊ��iw=0�j
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	//�e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}