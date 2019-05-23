#version 330

//ワールド変換とビュー射影のuniform変数
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//頂点属性
in vec3 inPosition;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}