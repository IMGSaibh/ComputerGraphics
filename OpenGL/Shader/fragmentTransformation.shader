#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;
void main()
{
	//only one Texture
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
	
	//multiple Textures
	FragColor = mix(texture(ourTexture, TexCoord), texture(texture2, TexCoord), 0.2);
}