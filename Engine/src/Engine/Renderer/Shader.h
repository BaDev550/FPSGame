#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();
	
		void Bind() const;
		void Unbind() const;
	
		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetVec2(const std::string& name, const glm::vec2& value);
		void SetVec3(const std::string& name, const glm::vec3& value);
		void SetVec4(const std::string& name, const glm::vec4& value);
		void SetMat2(const std::string& name, const glm::mat2& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
	private:
		std::string ReadFile(const std::string& filePath);
		unsigned int CompileShader(const std::string& source, unsigned int type);
		void CheckCompileErrors(unsigned int shader, const std::string& type);
	
		uint32_t _ProgramID;
	};
}
