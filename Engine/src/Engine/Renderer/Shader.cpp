#include "epch.h"
#include "Shader.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core/Core.h"
#include <fstream>
#include <sstream>
#include <iostream>
namespace Engine
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexCode = ReadFile(vertexPath);
		std::string fragmentCode = ReadFile(fragmentPath);
	
		GLuint vertexShader = CompileShader(vertexCode, GL_VERTEX_SHADER);
		GLuint fragmentShader = CompileShader(fragmentCode, GL_FRAGMENT_SHADER);
	
		_ProgramID = glCreateProgram();
		glAttachShader(_ProgramID, vertexShader);
		glAttachShader(_ProgramID, fragmentShader);
		glLinkProgram(_ProgramID);
	
		GLint success;
		glGetProgramiv(_ProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			GLint maxLength;
			glGetProgramiv(_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_ProgramID, maxLength, &maxLength, &infoLog[0]);
			std::cerr << "Shader Link Error:\n" << infoLog.data() << std::endl;
			ENGINE_ASSERT(false, "Shader linking failed!");
		}
	
		glDetachShader(_ProgramID, vertexShader);
		glDetachShader(_ProgramID, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(_ProgramID);
	}
	
	void Shader::Bind() const
	{
		glUseProgram(_ProgramID);
	}
	
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
	
	void Shader::SetBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(_ProgramID, name.c_str()), value);
	}
	
	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(_ProgramID, name.c_str()), value);
	}
	
	void Shader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(_ProgramID, name.c_str()), value);
	}
	
	void Shader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(_ProgramID, name.c_str()), 1, glm::value_ptr(value));
	}
	
	void Shader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(_ProgramID, name.c_str()), 1, glm::value_ptr(value));
	}
	
	void Shader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(_ProgramID, name.c_str()), 1, glm::value_ptr(value));
	}
	
	void Shader::SetMat2(const std::string& name, const glm::mat2& value)
	{
		glUniformMatrix2fv(glGetUniformLocation(_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void Shader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	
	std::string Shader::ReadFile(const std::string& filePath)
	{
		std::ifstream file(filePath);
		std::stringstream stream;
		stream << file.rdbuf();
		return stream.str();
	}
	
	unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
	{
		GLuint shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
	
		CheckCompileErrors(shader, (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"));
		return shader;
	}
	
	void Shader::CheckCompileErrors(unsigned int shader, const std::string& type)
	{
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLint maxLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
			std::cerr << type << " SHADER COMPILATION ERROR:\n" << infoLog.data() << std::endl;
			ENGINE_ASSERT(false, "Shader compile failed!");
		}
	}
}
