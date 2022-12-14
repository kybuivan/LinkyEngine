#ifndef SHADER_H
#define SHADER_H
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

// class Shader
// {
// public:
// 	Shader() { mProgram = glCreateProgram(); }
// 	~Shader() { glDeleteProgram(mProgram); }

// 	Shader & activate();
// 	Shader & attach(std::string const & filename);
// 	GLuint   create(std::string const & filename);
// 	GLuint   get() { return mProgram; }
// 	Shader & link();

// 	void bind(unsigned int location, float value);
// 	void bind(unsigned int location, glm::mat4 const & matrix);
// 	template<typename T> Shader & bind(std::string const & name, T&& value)
// 	{
// 		int location = glGetUniformLocation(mProgram, name.c_str());
// 		if (location == -1) fprintf(stderr, "Missing Uniform: %s\n", name.c_str());
// 		else bind(location, std::forward<T>(value));
// 		return *this;
// 	}

// private:
// 	Shader(Shader const &) = delete;
// 	Shader & operator=(Shader const &) = delete;

// private:
// 	GLuint mProgram;
// 	GLint  mStatus;
// 	GLint  mLength;
// };
#endif