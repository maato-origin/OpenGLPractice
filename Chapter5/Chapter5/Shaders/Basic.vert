//GLSL3.3を要求
#version330

//頂点属性をここに入れる
//今は位置情報だけ
in vec3 inPosition;

void main()
{
	//inPositionをgl_Positionにそのまま渡す
	gl_Position = vec4(inPosition,1.0);
}