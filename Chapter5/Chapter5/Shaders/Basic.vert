//GLSL3.3‚ğ—v‹
#version330

//’¸“_‘®«‚ğ‚±‚±‚É“ü‚ê‚é
//¡‚ÍˆÊ’uî•ñ‚¾‚¯
in vec3 inPosition;

void main()
{
	//inPosition‚ğgl_Position‚É‚»‚Ì‚Ü‚Ü“n‚·
	gl_Position = vec4(inPosition,1.0);
}