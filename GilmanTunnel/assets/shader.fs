#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;
uniform float alpha;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	color = mix(texture2D(ourTexture1, TexCoord), vec4(0.f,0.f,0.f,0.f), alpha);
}
