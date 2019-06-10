//GLSL3.3を要求
#version 330

//頂点シェーダーからのテクスチャ座標入力
in vec2 fragTexCoord;
//法線（ワールド空間）
in vec3 fragNormal;
//位置（ワールド空間）
in vec3 fragWorldPos;

//出力色
out vec4 outColor;

//テクスチャサンプリング用
uniform sampler2D uTexture;

//平行光源用の構造体
struct DirectionalLight
{
	//光の方向
	vec3 mDirection;
	//拡散反射色
	vec3 mDiffuseColor;
	//鏡面反射色
	vec3 mSpecColor;
};

//ライティング用uniform
//カメラの位置（ワールド空間）
uniform vec3 uCameraPos;
//環境光の強さ
uniform vec3 uAmbientLight;
//表面の鏡面反射指数
uniform float uSpecPower;
//平行光源
uniform DirectionalLight uDirLight;

void main()
{
	//表面法線N
	vec3 N = normalize(fragNormal);
	//表面から光源へのベクトルL
	vec3 L = normalize(-uDirLight.mDirection);
	//表面からカメラへのベクトルV
	vec3 V = normalize(uCameraPos - fragWorldPos);
	//Nに関する-Lの反射R
	vec3 R = normalize(reflect(-L,N));

	//フォンの反射を計算
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if(NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	//最終的な色はテクスチャの色xフォンの色（alpha = 1）
	outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}