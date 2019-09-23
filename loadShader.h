#ifndef _SHADER_H
#define _SHADER_H
class Shader
{
public:
	Shader(const GLchar* vertexshaderpath, const GLchar* fragmentshaderpath);
	void useProgram();
	int returnProgramid();
	~Shader();

private:
	GLint programid;
};


#endif