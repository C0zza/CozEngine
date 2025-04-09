#version 330 core 
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

// Assumes LEntityID is 32 bytes
uniform uint EntityID;

void main()
{ 
	float r = float((EntityID >> 24u) & 0xFFu) / 255.0;
	float g = float((EntityID >> 16u) & 0xFFu) / 255.0;
	float b = float((EntityID >> 8u) & 0xFFu) / 255.0;
	// glDisable(GL_BLEND) must be called prior to running so the alphas overwrite previous values as rgb does
	float a = float(EntityID & 0xFFu) / 255.0;

	FragColor = vec4(r, g, b, a);
}