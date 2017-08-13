#ifndef OPENGLEXT_HPP
#define OPENGLEXT_HPP

#include "OE/Platform/OpenGL/OpenGL.hpp"

#if OE_OPENGL

#if OE_WINDOWS
#define GLDECL WINAPI
#else
#define GLDECL 
#endif

#define GL_STATIC_DRAW                    0x88E4
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_COMPILE_STATUS                 0x8B81
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE0                       0x84C0
#define GL_STREAM_DRAW                    0x88E0
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_R16                            0x822A
#define GL_R32F                           0x822E
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGB16F                         0x881B
#define GL_RG8                            0x822B
#define GL_RG32F                          0x8230
#define GL_MIRRORED_REPEAT                0x8370
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_TEXTURE_3D                     0x806F
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    0x8A2F
#define GL_FRAMEBUFFER                    0x8D40
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9

typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#if OE_WINDOWS
	#define OE_OPENGL_EXTENSIONS_OS \
 	GLE(void, ActiveTexture, GLenum texture)
#else
	#define OE_OPENGL_EXTENSIONS_OS 
#endif

#define OE_OPENGL_EXTENSIONS \
	GLE(void, BlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) \
	GLE(void, StencilOpSeparate, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) \
	GLE(void, DrawBuffers, GLsizei n, const GLenum *bufs) \
	GLE(void, DeleteFramebuffers, GLsizei n, GLuint *framebuffers) \
	GLE(GLenum, CheckFramebufferStatus, GLenum target) \
	GLE(void, BindFramebuffer, GLenum target, GLuint framebuffer) \
	GLE(void, FramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level) \
	GLE(void, FramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
	GLE(void, GenFramebuffers, GLsizei n, GLuint *framebuffers) \
	GLE(void, TexStorage3D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) \
	GLE(void, GenerateMipmap, GLenum target) \
	GLE(void, GetActiveUniformBlockiv, GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) \
	GLE(void, GetActiveUniformBlockName, GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) \
	GLE(GLuint, GetUniformBlockIndex, GLuint program, const GLchar *uniformBlockName) \
	GLE(void, UniformBlockBinding, GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) \
	GLE(void, BindBufferBase, GLenum target, GLuint index, GLuint buffer) \
	GLE(void*, MapBuffer, GLenum target, GLenum access) \
	GLE(void, UnmapBuffer, GLenum target) \
	GLE(void, BufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) \
	GLE(void*, MapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) \
	GLE(void, LinkProgram, GLuint program) \
	GLE(void, ValidateProgram, GLuint program) \
	GLE(void, GetProgramiv, GLuint program, GLenum pname, GLint *params) \
	GLE(GLuint, CreateShader, GLenum type) \
	GLE(void, ShaderSource, GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) \
	GLE(void, CompileShader, GLuint shader) \
	GLE(void, GetShaderiv, GLuint shader, GLenum pname, GLint *params) \
	GLE(void, GetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
	GLE(void, DeleteShader, GLuint shader) \
	GLE(GLuint, CreateProgram, void) \
	GLE(void, AttachShader, GLuint program, GLuint shader) \
	GLE(void, DetachShader, GLuint program, GLuint shader) \
	GLE(void, UseProgram, GLuint program) \
	GLE(void, DeleteProgram, GLuint program) \
	GLE(void, GenVertexArrays, GLsizei n, GLuint *arrays) \
	GLE(void, BindVertexArray, GLuint array) \
	GLE(void, BufferData, GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) \
	GLE(void, GenBuffers, GLsizei n, GLuint *buffers) \
	GLE(void, BindBuffer, GLenum target, GLuint buffer) \
	GLE(void, DeleteBuffers, GLsizei n, const GLuint *buffers) \
	GLE(void, BindAttribLocation, GLuint program, GLuint index, const GLchar *name) \
	GLE(GLint, GetUniformLocation, GLuint program, const GLchar *name) \
	GLE(void, Uniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
	GLE(void, Uniform4fv, GLint location, GLsizei count, const GLfloat *value) \
	GLE(void, DeleteVertexArrays, GLsizei n, const GLuint *arrays) \
	GLE(void, EnableVertexAttribArray, GLuint index) \
	GLE(void, DisableVertexAttribArray, GLuint index) \
	GLE(void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) \
	GLE(void, Uniform1i, GLint location, GLint v0) \
	GLE(void, Uniform1f, GLint location, GLfloat v0) \
	GLE(void, Uniform1iv, GLint location, GLsizei count, const GLint *value) \
	GLE(void, Uniform2f, GLint location, GLfloat v0, GLfloat v1) \
	GLE(void, Uniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) \
	GLE(void, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \

#define GLE(ret, name, ...) typedef ret (GLDECL name##proc) (__VA_ARGS__); extern name##proc* gl##name;
	OE_OPENGL_EXTENSIONS
	OE_OPENGL_EXTENSIONS_OS
#undef GLE

	bool OpenGLEXTENSIONS_Load();

#endif

#endif