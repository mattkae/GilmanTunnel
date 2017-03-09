#pragma once
#if !__APPLE__
#define GLEW_STATIC
#endif

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader* Use();
	unsigned int GetProgram();

	void SetUniform1f(char* name, float v0);
	void SetUniform2f(char* name, float v0, float v1);
	void SetUniform3f(char* name, float v0, float v1, float v2);
	void SetUniform4f(char* name, float v0, float v1, float v2, float v3);
	void SetUniform1i(char* name, int v0);
	void SetUniform2i(char* name, int v0, int v1);
	void SetUniform3i(char* name, int v0, int v1, int v2);
	void SetUniform4i(char* name, int v0, int v1, int v2, int v3);
	void SetUniform1ui(char* name, unsigned int v0);
	void SetUniform2ui(char* name, unsigned int v0, unsigned int v1);
	void SetUniform3ui(char* name, unsigned int v0, unsigned int v1, unsigned int v2);
	void SetUniform4ui(char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
	void SetUniform1fv(char* name, int count, const float *v0);
	void SetUniform2fv(char* name, int count, const float *v0);
	void SetUniform3fv(char* name, int count, const float *v0);
	void SetUniform4fv(char* name, int count, const float *v0);
	void SetUniform1iv(char* name, int count, const int *v0);
	void SetUniform2iv(char* name, int count, const int *v0);
	void SetUniform3iv(char* name, int count, const int *v0);
	void SetUniform4iv(char* name, int count, const int *v0);
	void SetUniform1uiv(char* name, int count, const unsigned int *v0);
	void SetUniform2uiv(char* name, int count, const unsigned int *v0);
	void SetUniform3uiv(char* name, int count, const unsigned int *v0);
	void SetUniform4uiv(char* name, int count, const unsigned int *v0);
	void SetUniformMatrix2fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix3fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix4fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix2x3fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix3x2fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix2x4fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix4x2fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix3x4fv(char* name, int count, bool transpose, const float* v0);
	void SetUniformMatrix4x3fv(char* name, int count, bool transpose, const float* v0);

private:
	int GetUniform(const char* name);
	unsigned int Program;
};