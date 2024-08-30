#pragma once

class LFrameBuffer
{
public:
	LFrameBuffer(const int Width, const int Height);
	~LFrameBuffer();

	unsigned int GetTexture() const { return texture; }
	
	void RescaleBuffer(const int Width, const int Height);
	void Bind() const;
	void Unbind() const;

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;

	int m_Width;
	int m_Height;
};

