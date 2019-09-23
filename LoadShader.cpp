#define GLEW_STATIC
#include <glew.h>
#include <GLFW/glfw3.h>
#include "loadShader.h"
#include <string>
#include <fstream>
#include <sstream>//stringstream
#include <iostream>
using namespace std;


int loadVertexShader(const char* vertexshadersource, int type);
int loadFragmentShader(const char* fragmentshadersource, int type);
int loadProgram(int vertexshader, int fragmentshader);
bool getCompileStatus(int id, bool isprogram);
void printProblam(int id, int encoding);

Shader::Shader(const GLchar* vertexshaderpath, const GLchar* fragmentshaderpath)       //filecode ----------->  stringcode --------->  stringstreamcode(�м���) ----------> strcode----------> cstrcode
{
	string vertexshadercode;
	string fragmentshadercode;
	ifstream vshaderfile;
	ifstream fshaderfile;
	vshaderfile.exceptions(ifstream::failbit | ifstream::badbit);
	fshaderfile.exceptions(ifstream::failbit | ifstream::badbit);
	stringstream vshaderstream;
	stringstream fshaderstream;
	GLint vertexshader, fragmentshader;
	try
	{
		vshaderfile.open(vertexshaderpath);
		fshaderfile.open(fragmentshaderpath);
		vshaderstream << vshaderfile.rdbuf();
		fshaderstream << fshaderfile.rdbuf();
		vshaderfile.close();
		fshaderfile.close();
		vertexshadercode = vshaderstream.str();
		fragmentshadercode = fshaderstream.str();
	}
	catch (ifstream::failure)
	{
		cout << "�ļ���ȡʧ��" << endl;
	}
	const char* vshadercode = vertexshadercode.c_str();
	const char* fshadercode = fragmentshadercode.c_str();
	vertexshader = loadVertexShader(vshadercode, GL_VERTEX_SHADER);
	fragmentshader = loadFragmentShader(fshadercode, GL_FRAGMENT_SHADER);
	programid = loadProgram(vertexshader, fragmentshader);
}

void Shader::useProgram()
{
	glUseProgram(programid);
}
int Shader::returnProgramid()
{
	GLint shaderProgram;
	shaderProgram = programid;
	return shaderProgram;
}

Shader::~Shader()
{
	cout << "������й���,�ͷ��ڴ�" << endl;
}

int loadVertexShader(const char* vertexshadersource, int type)
{
	int vertexshader;
	vertexshader = glCreateShader(type);
	glShaderSource(vertexshader, 1, &vertexshadersource, NULL);
	glCompileShader(vertexshader);
	if (!getCompileStatus(vertexshader, false))
	{
		cout << "������ɫ������ʧ��" << endl;
		printProblam(vertexshader, 1);
		return -1;
	}
	return vertexshader;
}


int loadFragmentShader(const char* fragmentshadersource, int type)
{
	int fragmentshader;
	fragmentshader = glCreateShader(type);
	glShaderSource(fragmentshader, 1, &fragmentshadersource, NULL);
	glCompileShader(fragmentshader);
	if (!getCompileStatus(fragmentshader, false))
	{
		cout << "ƬԪ��ɫ������ʧ��" << endl;
		printProblam(fragmentshader, 2);
		return -1;
	}
	return fragmentshader;
}

int loadProgram(int vertexshader, int fragmentshader)
{
	int shaderprogram;
	if (vertexshader && fragmentshader == 0)
	{
		cout << "����ɫ����һ����������" << endl;
		return -1;
	}
	shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);
	glLinkProgram(shaderprogram);
	if (!getCompileStatus(shaderprogram, true))
	{
		cout << "��ɫ���������ʧ��" << endl;
		printProblam(shaderprogram, 3);
		shaderprogram = 0;
		return -1;
	}
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	return shaderprogram;
}

bool getCompileStatus(int id, bool isprogram)
{
	int status;
	if (isprogram)
	{
		glGetProgramiv(id, GL_LINK_STATUS, &status);
	}
	else
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	}
	return status == GL_TRUE;
}

void printProblam(int id, int encoding)
{
	char* value;
	int size;
	if (encoding == 1)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);
		value = new char[size + 1];
		cout << "������ɫ�����ⱨ��:";
		glGetShaderInfoLog(id, size + 1, nullptr, value);
		cout << value << endl;
	}
	else if (encoding == 2)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);
		value = new char[size + 1];
		cout << "ƬԪ��ɫ�����ⱨ��:";
		glGetShaderInfoLog(id, size + 1, nullptr, value);
		cout << value << endl;
	}
	else if (encoding == 3)
	{
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);
		value = new char[size + 1];
		cout << "��ɫ���������ⱨ��:";
		glGetProgramInfoLog(id, size + 1, nullptr, value);
		cout << value << endl;
	}
}