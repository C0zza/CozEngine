#pragma once

class LFrameBuffer
{
public:
	LFrameBuffer(const float Width, const float Height);
	~LFrameBuffer();

	unsigned int GetTexture() { return texture; }
	
	void RescaleBuffer(const float Width, const float Height);
	void Bind() const;
	void Unbind() const;

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;

	int m_Width;
	int m_Height;
};

