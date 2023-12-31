#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

// X macros for defining and loading each opengl function
#define FOR_EACH_GL_FUNCTION(DO) \
        DO(PFNGLDRAWRANGEELEMENTSPROC, DrawRangeElements) \
        DO(PFNGLTEXIMAGE3DPROC, TexImage3D) \
        DO(PFNGLTEXSUBIMAGE3DPROC, TexSubImage3D) \
        DO(PFNGLCOPYTEXSUBIMAGE3DPROC, CopyTexSubImage3D) \
        DO(PFNGLACTIVETEXTUREPROC, ActiveTexture) \
        DO(PFNGLSAMPLECOVERAGEPROC, SampleCoverage) \
        DO(PFNGLCOMPRESSEDTEXIMAGE3DPROC, CompressedTexImage3D) \
        DO(PFNGLCOMPRESSEDTEXIMAGE2DPROC, CompressedTexImage2D) \
        DO(PFNGLCOMPRESSEDTEXIMAGE1DPROC, CompressedTexImage1D) \
        DO(PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC, CompressedTexSubImage3D) \
        DO(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, CompressedTexSubImage2D) \
        DO(PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC, CompressedTexSubImage1D) \
        DO(PFNGLGETCOMPRESSEDTEXIMAGEPROC, GetCompressedTexImage) \
        DO(PFNGLBLENDFUNCSEPARATEPROC, BlendFuncSeparate) \
        DO(PFNGLMULTIDRAWARRAYSPROC, MultiDrawArrays) \
        DO(PFNGLMULTIDRAWELEMENTSPROC, MultiDrawElements) \
        DO(PFNGLPOINTPARAMETERFPROC, PointParameterf) \
        DO(PFNGLPOINTPARAMETERFVPROC, PointParameterfv) \
        DO(PFNGLPOINTPARAMETERIPROC, PointParameteri) \
        DO(PFNGLPOINTPARAMETERIVPROC, PointParameteriv) \
        DO(PFNGLBLENDCOLORPROC, BlendColor) \
        DO(PFNGLBLENDEQUATIONPROC, BlendEquation) \
        DO(PFNGLGENQUERIESPROC, GenQueries) \
        DO(PFNGLDELETEQUERIESPROC, DeleteQueries) \
        DO(PFNGLBEGINQUERYPROC, BeginQuery) \
        DO(PFNGLENDQUERYPROC, EndQuery) \
        DO(PFNGLGETQUERYIVPROC, GetQueryiv) \
        DO(PFNGLGETQUERYOBJECTIVPROC, GetQueryObjectiv) \
        DO(PFNGLGETQUERYOBJECTUIVPROC, GetQueryObjectuiv) \
        DO(PFNGLBINDBUFFERPROC, BindBuffer) \
        DO(PFNGLDELETEBUFFERSPROC, DeleteBuffers) \
        DO(PFNGLGENBUFFERSPROC, GenBuffers) \
        DO(PFNGLBUFFERDATAPROC, BufferData) \
        DO(PFNGLBUFFERSUBDATAPROC, BufferSubData) \
        DO(PFNGLGETBUFFERSUBDATAPROC, GetBufferSubData) \
        DO(PFNGLGETBUFFERPARAMETERIVPROC, GetBufferParameteriv) \
        DO(PFNGLGETBUFFERPOINTERVPROC, GetBufferPointerv) \
        DO(PFNGLISQUERYPROC, IsQuery) \
        DO(PFNGLISBUFFERPROC, IsBuffer) \
        DO(PFNGLMAPBUFFERPROC, MapBuffer) \
        DO(PFNGLUNMAPBUFFERPROC, UnmapBuffer) \
        DO(PFNGLBLENDEQUATIONSEPARATEPROC, BlendEquationSeparate) \
        DO(PFNGLDRAWBUFFERSPROC, DrawBuffers) \
        DO(PFNGLSTENCILOPSEPARATEPROC, StencilOpSeparate) \
        DO(PFNGLSTENCILFUNCSEPARATEPROC, StencilFuncSeparate) \
        DO(PFNGLSTENCILMASKSEPARATEPROC, StencilMaskSeparate) \
        DO(PFNGLATTACHSHADERPROC, AttachShader) \
        DO(PFNGLBINDATTRIBLOCATIONPROC, BindAttribLocation) \
        DO(PFNGLCOMPILESHADERPROC, CompileShader) \
        DO(PFNGLDELETEPROGRAMPROC, DeleteProgram) \
        DO(PFNGLDELETESHADERPROC, DeleteShader) \
        DO(PFNGLDETACHSHADERPROC, DetachShader) \
        DO(PFNGLDISABLEVERTEXATTRIBARRAYPROC, DisableVertexAttribArray) \
        DO(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray) \
        DO(PFNGLGETACTIVEATTRIBPROC, GetActiveAttrib) \
        DO(PFNGLGETACTIVEUNIFORMPROC, GetActiveUniform) \
        DO(PFNGLGETATTACHEDSHADERSPROC, GetAttachedShaders) \
        DO(PFNGLGETPROGRAMIVPROC, GetProgramiv) \
        DO(PFNGLGETPROGRAMINFOLOGPROC, GetProgramInfoLog) \
        DO(PFNGLGETSHADERIVPROC, GetShaderiv) \
        DO(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog) \
        DO(PFNGLGETSHADERSOURCEPROC, GetShaderSource) \
        DO(PFNGLGETUNIFORMFVPROC, GetUniformfv) \
        DO(PFNGLGETUNIFORMIVPROC, GetUniformiv) \
        DO(PFNGLGETVERTEXATTRIBDVPROC, GetVertexAttribdv) \
        DO(PFNGLGETVERTEXATTRIBFVPROC, GetVertexAttribfv) \
        DO(PFNGLGETVERTEXATTRIBIVPROC, GetVertexAttribiv) \
        DO(PFNGLGETVERTEXATTRIBPOINTERVPROC, GetVertexAttribPointerv) \
        DO(PFNGLLINKPROGRAMPROC, LinkProgram) \
        DO(PFNGLSHADERSOURCEPROC, ShaderSource) \
        DO(PFNGLUSEPROGRAMPROC, UseProgram) \
        DO(PFNGLUNIFORM1FPROC, Uniform1f) \
        DO(PFNGLUNIFORM2FPROC, Uniform2f) \
        DO(PFNGLUNIFORM3FPROC, Uniform3f) \
        DO(PFNGLUNIFORM4FPROC, Uniform4f) \
        DO(PFNGLUNIFORM1IPROC, Uniform1i) \
        DO(PFNGLUNIFORM2IPROC, Uniform2i) \
        DO(PFNGLUNIFORM3IPROC, Uniform3i) \
        DO(PFNGLUNIFORM4IPROC, Uniform4i) \
        DO(PFNGLUNIFORM1FVPROC, Uniform1fv) \
        DO(PFNGLUNIFORM2FVPROC, Uniform2fv) \
        DO(PFNGLUNIFORM3FVPROC, Uniform3fv) \
        DO(PFNGLUNIFORM4FVPROC, Uniform4fv) \
        DO(PFNGLUNIFORM1IVPROC, Uniform1iv) \
        DO(PFNGLUNIFORM2IVPROC, Uniform2iv) \
        DO(PFNGLUNIFORM3IVPROC, Uniform3iv) \
        DO(PFNGLUNIFORM4IVPROC, Uniform4iv) \
        DO(PFNGLUNIFORMMATRIX2FVPROC, UniformMatrix2fv) \
        DO(PFNGLUNIFORMMATRIX3FVPROC, UniformMatrix3fv) \
        DO(PFNGLUNIFORMMATRIX4FVPROC, UniformMatrix4fv) \
        DO(PFNGLVALIDATEPROGRAMPROC, ValidateProgram) \
        DO(PFNGLVERTEXATTRIB1DPROC, VertexAttrib1d) \
        DO(PFNGLVERTEXATTRIB1DVPROC, VertexAttrib1dv) \
        DO(PFNGLVERTEXATTRIB1FPROC, VertexAttrib1f) \
        DO(PFNGLVERTEXATTRIB1FVPROC, VertexAttrib1fv) \
        DO(PFNGLVERTEXATTRIB1SPROC, VertexAttrib1s) \
        DO(PFNGLVERTEXATTRIB1SVPROC, VertexAttrib1sv) \
        DO(PFNGLVERTEXATTRIB2DPROC, VertexAttrib2d) \
        DO(PFNGLVERTEXATTRIB2DVPROC, VertexAttrib2dv) \
        DO(PFNGLVERTEXATTRIB2FPROC, VertexAttrib2f) \
        DO(PFNGLVERTEXATTRIB2FVPROC, VertexAttrib2fv) \
        DO(PFNGLVERTEXATTRIB2SPROC, VertexAttrib2s) \
        DO(PFNGLVERTEXATTRIB2SVPROC, VertexAttrib2sv) \
        DO(PFNGLVERTEXATTRIB3DPROC, VertexAttrib3d) \
        DO(PFNGLVERTEXATTRIB3DVPROC, VertexAttrib3dv) \
        DO(PFNGLVERTEXATTRIB3FPROC, VertexAttrib3f) \
        DO(PFNGLVERTEXATTRIB3FVPROC, VertexAttrib3fv) \
        DO(PFNGLVERTEXATTRIB3SPROC, VertexAttrib3s) \
        DO(PFNGLVERTEXATTRIB3SVPROC, VertexAttrib3sv) \
        DO(PFNGLVERTEXATTRIB4NBVPROC, VertexAttrib4Nbv) \
        DO(PFNGLVERTEXATTRIB4NIVPROC, VertexAttrib4Niv) \
        DO(PFNGLVERTEXATTRIB4NSVPROC, VertexAttrib4Nsv) \
        DO(PFNGLVERTEXATTRIB4NUBPROC, VertexAttrib4Nub) \
        DO(PFNGLVERTEXATTRIB4NUBVPROC, VertexAttrib4Nubv) \
        DO(PFNGLVERTEXATTRIB4NUIVPROC, VertexAttrib4Nuiv) \
        DO(PFNGLVERTEXATTRIB4NUSVPROC, VertexAttrib4Nusv) \
        DO(PFNGLVERTEXATTRIB4BVPROC, VertexAttrib4bv) \
        DO(PFNGLVERTEXATTRIB4DPROC, VertexAttrib4d) \
        DO(PFNGLVERTEXATTRIB4DVPROC, VertexAttrib4dv) \
        DO(PFNGLVERTEXATTRIB4FPROC, VertexAttrib4f) \
        DO(PFNGLVERTEXATTRIB4FVPROC, VertexAttrib4fv) \
        DO(PFNGLVERTEXATTRIB4IVPROC, VertexAttrib4iv) \
        DO(PFNGLVERTEXATTRIB4SPROC, VertexAttrib4s) \
        DO(PFNGLVERTEXATTRIB4SVPROC, VertexAttrib4sv) \
        DO(PFNGLVERTEXATTRIB4UBVPROC, VertexAttrib4ubv) \
        DO(PFNGLVERTEXATTRIB4UIVPROC, VertexAttrib4uiv) \
        DO(PFNGLVERTEXATTRIB4USVPROC, VertexAttrib4usv) \
        DO(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer) \
        DO(PFNGLCREATEPROGRAMPROC, CreateProgram) \
        DO(PFNGLCREATESHADERPROC, CreateShader) \
        DO(PFNGLGETATTRIBLOCATIONPROC, GetAttribLocation) \
        DO(PFNGLGETUNIFORMLOCATIONPROC, GetUniformLocation) \
        DO(PFNGLISPROGRAMPROC, IsProgram) \
        DO(PFNGLISSHADERPROC, IsShader) \
        DO(PFNGLUNIFORMMATRIX2X3FVPROC, UniformMatrix2x3fv) \
        DO(PFNGLUNIFORMMATRIX3X2FVPROC, UniformMatrix3x2fv) \
        DO(PFNGLUNIFORMMATRIX2X4FVPROC, UniformMatrix2x4fv) \
        DO(PFNGLUNIFORMMATRIX4X2FVPROC, UniformMatrix4x2fv) \
        DO(PFNGLUNIFORMMATRIX3X4FVPROC, UniformMatrix3x4fv) \
        DO(PFNGLUNIFORMMATRIX4X3FVPROC, UniformMatrix4x3fv) \
        DO(PFNGLCOLORMASKIPROC, ColorMaski) \
        DO(PFNGLGETBOOLEANI_VPROC, GetBooleani_v) \
        DO(PFNGLGETINTEGERI_VPROC, GetIntegeri_v) \
        DO(PFNGLENABLEIPROC, Enablei) \
        DO(PFNGLDISABLEIPROC, Disablei) \
        DO(PFNGLBEGINTRANSFORMFEEDBACKPROC, BeginTransformFeedback) \
        DO(PFNGLENDTRANSFORMFEEDBACKPROC, EndTransformFeedback) \
        DO(PFNGLBINDBUFFERRANGEPROC, BindBufferRange) \
        DO(PFNGLBINDBUFFERBASEPROC, BindBufferBase) \
        DO(PFNGLTRANSFORMFEEDBACKVARYINGSPROC, TransformFeedbackVaryings) \
        DO(PFNGLGETTRANSFORMFEEDBACKVARYINGPROC, GetTransformFeedbackVarying) \
        DO(PFNGLCLAMPCOLORPROC, ClampColor) \
        DO(PFNGLBEGINCONDITIONALRENDERPROC, BeginConditionalRender) \
        DO(PFNGLENDCONDITIONALRENDERPROC, EndConditionalRender) \
        DO(PFNGLVERTEXATTRIBIPOINTERPROC, VertexAttribIPointer) \
        DO(PFNGLGETVERTEXATTRIBIIVPROC, GetVertexAttribIiv) \
        DO(PFNGLGETVERTEXATTRIBIUIVPROC, GetVertexAttribIuiv) \
        DO(PFNGLVERTEXATTRIBI1IPROC, VertexAttribI1i) \
        DO(PFNGLVERTEXATTRIBI2IPROC, VertexAttribI2i) \
        DO(PFNGLVERTEXATTRIBI3IPROC, VertexAttribI3i) \
        DO(PFNGLVERTEXATTRIBI4IPROC, VertexAttribI4i) \
        DO(PFNGLVERTEXATTRIBI1UIPROC, VertexAttribI1ui) \
        DO(PFNGLVERTEXATTRIBI2UIPROC, VertexAttribI2ui) \
        DO(PFNGLVERTEXATTRIBI3UIPROC, VertexAttribI3ui) \
        DO(PFNGLVERTEXATTRIBI4UIPROC, VertexAttribI4ui) \
        DO(PFNGLVERTEXATTRIBI1IVPROC, VertexAttribI1iv) \
        DO(PFNGLVERTEXATTRIBI2IVPROC, VertexAttribI2iv) \
        DO(PFNGLVERTEXATTRIBI3IVPROC, VertexAttribI3iv) \
        DO(PFNGLVERTEXATTRIBI4IVPROC, VertexAttribI4iv) \
        DO(PFNGLVERTEXATTRIBI1UIVPROC, VertexAttribI1uiv) \
        DO(PFNGLVERTEXATTRIBI2UIVPROC, VertexAttribI2uiv) \
        DO(PFNGLVERTEXATTRIBI3UIVPROC, VertexAttribI3uiv) \
        DO(PFNGLVERTEXATTRIBI4UIVPROC, VertexAttribI4uiv) \
        DO(PFNGLVERTEXATTRIBI4BVPROC, VertexAttribI4bv) \
        DO(PFNGLVERTEXATTRIBI4SVPROC, VertexAttribI4sv) \
        DO(PFNGLVERTEXATTRIBI4UBVPROC, VertexAttribI4ubv) \
        DO(PFNGLVERTEXATTRIBI4USVPROC, VertexAttribI4usv) \
        DO(PFNGLGETUNIFORMUIVPROC, GetUniformuiv) \
        DO(PFNGLBINDFRAGDATALOCATIONPROC, BindFragDataLocation) \
        DO(PFNGLUNIFORM1UIPROC, Uniform1ui) \
        DO(PFNGLUNIFORM2UIPROC, Uniform2ui) \
        DO(PFNGLUNIFORM3UIPROC, Uniform3ui) \
        DO(PFNGLUNIFORM4UIPROC, Uniform4ui) \
        DO(PFNGLUNIFORM1UIVPROC, Uniform1uiv) \
        DO(PFNGLUNIFORM2UIVPROC, Uniform2uiv) \
        DO(PFNGLUNIFORM3UIVPROC, Uniform3uiv) \
        DO(PFNGLUNIFORM4UIVPROC, Uniform4uiv) \
        DO(PFNGLTEXPARAMETERIIVPROC, TexParameterIiv) \
        DO(PFNGLTEXPARAMETERIUIVPROC, TexParameterIuiv) \
        DO(PFNGLGETTEXPARAMETERIIVPROC, GetTexParameterIiv) \
        DO(PFNGLGETTEXPARAMETERIUIVPROC, GetTexParameterIuiv) \
        DO(PFNGLCLEARBUFFERIVPROC, ClearBufferiv) \
        DO(PFNGLCLEARBUFFERUIVPROC, ClearBufferuiv) \
        DO(PFNGLCLEARBUFFERFVPROC, ClearBufferfv) \
        DO(PFNGLCLEARBUFFERFIPROC, ClearBufferfi) \
        DO(PFNGLBINDRENDERBUFFERPROC, BindRenderbuffer) \
        DO(PFNGLDELETERENDERBUFFERSPROC, DeleteRenderbuffers) \
        DO(PFNGLGENRENDERBUFFERSPROC, GenRenderbuffers) \
        DO(PFNGLRENDERBUFFERSTORAGEPROC, RenderbufferStorage) \
        DO(PFNGLGETRENDERBUFFERPARAMETERIVPROC, GetRenderbufferParameteriv) \
        DO(PFNGLBINDFRAMEBUFFERPROC, BindFramebuffer) \
        DO(PFNGLDELETEFRAMEBUFFERSPROC, DeleteFramebuffers) \
        DO(PFNGLGENFRAMEBUFFERSPROC, GenFramebuffers) \
        DO(PFNGLFRAMEBUFFERTEXTURE1DPROC, FramebufferTexture1D) \
        DO(PFNGLFRAMEBUFFERTEXTURE2DPROC, FramebufferTexture2D) \
        DO(PFNGLFRAMEBUFFERTEXTURE3DPROC, FramebufferTexture3D) \
        DO(PFNGLFRAMEBUFFERRENDERBUFFERPROC, FramebufferRenderbuffer) \
        DO(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, GetFramebufferAttachmentParameteriv) \
        DO(PFNGLGENERATEMIPMAPPROC, GenerateMipmap) \
        DO(PFNGLBLITFRAMEBUFFERPROC, BlitFramebuffer) \
        DO(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC, RenderbufferStorageMultisample) \
        DO(PFNGLFRAMEBUFFERTEXTURELAYERPROC, FramebufferTextureLayer) \
        DO(PFNGLFLUSHMAPPEDBUFFERRANGEPROC, FlushMappedBufferRange) \
        DO(PFNGLBINDVERTEXARRAYPROC, BindVertexArray) \
        DO(PFNGLDELETEVERTEXARRAYSPROC, DeleteVertexArrays) \
        DO(PFNGLGENVERTEXARRAYSPROC, GenVertexArrays) \
        DO(PFNGLISENABLEDIPROC, IsEnabledi) \
        DO(PFNGLGETFRAGDATALOCATIONPROC, GetFragDataLocation) \
        DO(PFNGLGETSTRINGIPROC, GetStringi) \
        DO(PFNGLISRENDERBUFFERPROC, IsRenderbuffer) \
        DO(PFNGLISFRAMEBUFFERPROC, IsFramebuffer) \
        DO(PFNGLCHECKFRAMEBUFFERSTATUSPROC, CheckFramebufferStatus) \
        DO(PFNGLMAPBUFFERRANGEPROC, MapBufferRange) \
        DO(PFNGLISVERTEXARRAYPROC, IsVertexArray) \
        DO(PFNGLDRAWARRAYSINSTANCEDPROC, DrawArraysInstanced) \
        DO(PFNGLDRAWELEMENTSINSTANCEDPROC, DrawElementsInstanced) \
        DO(PFNGLTEXBUFFERPROC, TexBuffer) \
        DO(PFNGLPRIMITIVERESTARTINDEXPROC, PrimitiveRestartIndex) \
        DO(PFNGLCOPYBUFFERSUBDATAPROC, CopyBufferSubData) \
        DO(PFNGLGETUNIFORMINDICESPROC, GetUniformIndices) \
        DO(PFNGLGETACTIVEUNIFORMSIVPROC, GetActiveUniformsiv) \
        DO(PFNGLGETACTIVEUNIFORMNAMEPROC, GetActiveUniformName) \
        DO(PFNGLGETACTIVEUNIFORMBLOCKIVPROC, GetActiveUniformBlockiv) \
        DO(PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC, GetActiveUniformBlockName) \
        DO(PFNGLUNIFORMBLOCKBINDINGPROC, UniformBlockBinding) \
        DO(PFNGLGETUNIFORMBLOCKINDEXPROC, GetUniformBlockIndex) \
        DO(PFNGLDRAWELEMENTSBASEVERTEXPROC, DrawElementsBaseVertex) \
        DO(PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC, DrawRangeElementsBaseVertex) \
        DO(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC, DrawElementsInstancedBaseVertex) \
        DO(PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC, MultiDrawElementsBaseVertex) \
        DO(PFNGLPROVOKINGVERTEXPROC, ProvokingVertex) \
        DO(PFNGLDELETESYNCPROC, DeleteSync) \
        DO(PFNGLWAITSYNCPROC, WaitSync) \
        DO(PFNGLGETINTEGER64VPROC, GetInteger64v) \
        DO(PFNGLGETSYNCIVPROC, GetSynciv) \
        DO(PFNGLGETINTEGER64I_VPROC, GetInteger64i_v) \
        DO(PFNGLGETBUFFERPARAMETERI64VPROC, GetBufferParameteri64v) \
        DO(PFNGLFRAMEBUFFERTEXTUREPROC, FramebufferTexture) \
        DO(PFNGLTEXIMAGE2DMULTISAMPLEPROC, TexImage2DMultisample) \
        DO(PFNGLTEXIMAGE3DMULTISAMPLEPROC, TexImage3DMultisample) \
        DO(PFNGLGETMULTISAMPLEFVPROC, GetMultisamplefv) \
        DO(PFNGLSAMPLEMASKIPROC, SampleMaski) \
        DO(PFNGLFENCESYNCPROC, FenceSync) \
        DO(PFNGLISSYNCPROC, IsSync) \
        DO(PFNGLCLIENTWAITSYNCPROC, ClientWaitSync) \
        DO(PFNGLBINDFRAGDATALOCATIONINDEXEDPROC, BindFragDataLocationIndexed) \
        DO(PFNGLGENSAMPLERSPROC, GenSamplers) \
        DO(PFNGLDELETESAMPLERSPROC, DeleteSamplers) \
        DO(PFNGLBINDSAMPLERPROC, BindSampler) \
        DO(PFNGLSAMPLERPARAMETERIPROC, SamplerParameteri) \
        DO(PFNGLSAMPLERPARAMETERIVPROC, SamplerParameteriv) \
        DO(PFNGLSAMPLERPARAMETERFPROC, SamplerParameterf) \
        DO(PFNGLSAMPLERPARAMETERFVPROC, SamplerParameterfv) \
        DO(PFNGLSAMPLERPARAMETERIIVPROC, SamplerParameterIiv) \
        DO(PFNGLSAMPLERPARAMETERIUIVPROC, SamplerParameterIuiv) \
        DO(PFNGLGETSAMPLERPARAMETERIVPROC, GetSamplerParameteriv) \
        DO(PFNGLGETSAMPLERPARAMETERIIVPROC, GetSamplerParameterIiv) \
        DO(PFNGLGETSAMPLERPARAMETERFVPROC, GetSamplerParameterfv) \
        DO(PFNGLGETSAMPLERPARAMETERIUIVPROC, GetSamplerParameterIuiv) \
        DO(PFNGLQUERYCOUNTERPROC, QueryCounter) \
        DO(PFNGLGETQUERYOBJECTI64VPROC, GetQueryObjecti64v) \
        DO(PFNGLGETQUERYOBJECTUI64VPROC, GetQueryObjectui64v) \
        DO(PFNGLVERTEXATTRIBDIVISORPROC, VertexAttribDivisor) \
        DO(PFNGLVERTEXATTRIBP1UIPROC, VertexAttribP1ui) \
        DO(PFNGLVERTEXATTRIBP1UIVPROC, VertexAttribP1uiv) \
        DO(PFNGLVERTEXATTRIBP2UIPROC, VertexAttribP2ui) \
        DO(PFNGLVERTEXATTRIBP2UIVPROC, VertexAttribP2uiv) \
        DO(PFNGLVERTEXATTRIBP3UIPROC, VertexAttribP3ui) \
        DO(PFNGLVERTEXATTRIBP3UIVPROC, VertexAttribP3uiv) \
        DO(PFNGLVERTEXATTRIBP4UIPROC, VertexAttribP4ui) \
        DO(PFNGLVERTEXATTRIBP4UIVPROC, VertexAttribP4uiv) \
        DO(PFNGLGETFRAGDATAINDEXPROC, GetFragDataIndex) \
        DO(PFNGLISSAMPLERPROC, IsSampler)
#define DEFINE_GL_FUNCTION(type, name) type name;
#define LOAD_GL_FUNCTION_WIN32(type, name) result.name = (type) wglGetProcAddress("gl" #name);

typedef struct OpenGLInfo OpenGLInfo;
typedef struct OpenGLFunctions OpenGLFunctions;

struct OpenGLInfo {
	char *vendor;
	char *renderer;
	char *version;
	char *shader_language_version;
	char *extensions;
};

struct OpenGLFunctions {
        FOR_EACH_GL_FUNCTION(DEFINE_GL_FUNCTION)
};
#undef DEFINE_GL_FUNCTION
