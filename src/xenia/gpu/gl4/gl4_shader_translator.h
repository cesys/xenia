/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_GPU_GL4_GL4_SHADER_TRANSLATOR_H_
#define XENIA_GPU_GL4_GL4_SHADER_TRANSLATOR_H_

#include <string>

#include <alloy/string_buffer.h>
#include <xenia/common.h>
#include <xenia/gpu/gl4/gl_context.h>
#include <xenia/gpu/gl4/gl4_shader.h>
#include <xenia/gpu/ucode.h>
#include <xenia/gpu/xenos.h>

namespace xe {
namespace gpu {
namespace gl4 {

class GL4ShaderTranslator {
 public:
  static const uint32_t kMaxInterpolators = 16;

  GL4ShaderTranslator();
  ~GL4ShaderTranslator();

  std::string TranslateVertexShader(
      GL4Shader* vertex_shader,
      const xenos::xe_gpu_program_cntl_t& program_cntl);
  std::string TranslatePixelShader(
      GL4Shader* pixel_shader,
      const xenos::xe_gpu_program_cntl_t& program_cntl);

 protected:
  ShaderType shader_type_;
  uint32_t tex_fetch_index_;
  const uint32_t* dwords_;

  static const int kOutputCapacity = 64 * 1024;
  alloy::StringBuffer output_;

  bool is_vertex_shader() const { return shader_type_ == ShaderType::kVertex; }
  bool is_pixel_shader() const { return shader_type_ == ShaderType::kPixel; }

  void Reset(GL4Shader* shader);
  void Append(const char* format, ...) {
    va_list args;
    va_start(args, format);
    output_.AppendVarargs(format, args);
    va_end(args);
  }

  void AppendTextureHeader(const GL4Shader::SamplerInputs& sampler_inputs);

  void AppendSrcReg(uint32_t num, uint32_t type, uint32_t swiz, uint32_t negate,
                    uint32_t abs);
  void AppendDestRegName(uint32_t num, uint32_t dst_exp);
  void AppendDestReg(uint32_t num, uint32_t mask, uint32_t dst_exp);
  void AppendDestRegPost(uint32_t num, uint32_t mask, uint32_t dst_exp);
  void PrintSrcReg(uint32_t num, uint32_t type, uint32_t swiz, uint32_t negate,
                   uint32_t abs);
  void PrintDstReg(uint32_t num, uint32_t mask, uint32_t dst_exp);
  void PrintExportComment(uint32_t num);

  bool TranslateALU(const ucode::instr_alu_t* alu, int sync);
  bool TranslateALU_ADDv(const ucode::instr_alu_t& alu);
  bool TranslateALU_MULv(const ucode::instr_alu_t& alu);
  bool TranslateALU_MAXv(const ucode::instr_alu_t& alu);
  bool TranslateALU_MINv(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETXXv(const ucode::instr_alu_t& alu, const char* op);
  bool TranslateALU_SETEv(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETGTv(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETGTEv(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETNEv(const ucode::instr_alu_t& alu);
  bool TranslateALU_FRACv(const ucode::instr_alu_t& alu);
  bool TranslateALU_TRUNCv(const ucode::instr_alu_t& alu);
  bool TranslateALU_FLOORv(const ucode::instr_alu_t& alu);
  bool TranslateALU_MULADDv(const ucode::instr_alu_t& alu);
  bool TranslateALU_CNDXXv(const ucode::instr_alu_t& alu, const char* op);
  bool TranslateALU_CNDEv(const ucode::instr_alu_t& alu);
  bool TranslateALU_CNDGTEv(const ucode::instr_alu_t& alu);
  bool TranslateALU_CNDGTv(const ucode::instr_alu_t& alu);
  bool TranslateALU_DOT4v(const ucode::instr_alu_t& alu);
  bool TranslateALU_DOT3v(const ucode::instr_alu_t& alu);
  bool TranslateALU_DOT2ADDv(const ucode::instr_alu_t& alu);
  // CUBEv
  bool TranslateALU_MAX4v(const ucode::instr_alu_t& alu);
  // ...
  bool TranslateALU_MAXs(const ucode::instr_alu_t& alu);
  bool TranslateALU_MINs(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETXXs(const ucode::instr_alu_t& alu, const char* op);
  bool TranslateALU_SETEs(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETGTs(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETGTEs(const ucode::instr_alu_t& alu);
  bool TranslateALU_SETNEs(const ucode::instr_alu_t& alu);
  bool TranslateALU_RECIP_IEEE(const ucode::instr_alu_t& alu);
  bool TranslateALU_MUL_CONST_0(const ucode::instr_alu_t& alu);
  bool TranslateALU_MUL_CONST_1(const ucode::instr_alu_t& alu);
  bool TranslateALU_ADD_CONST_0(const ucode::instr_alu_t& alu);
  bool TranslateALU_ADD_CONST_1(const ucode::instr_alu_t& alu);
  bool TranslateALU_SUB_CONST_0(const ucode::instr_alu_t& alu);
  bool TranslateALU_SUB_CONST_1(const ucode::instr_alu_t& alu);
  bool TranslateALU_RETAIN_PREV(const ucode::instr_alu_t& alu);

  void PrintDestFecth(uint32_t dst_reg, uint32_t dst_swiz);
  void AppendFetchDest(uint32_t dst_reg, uint32_t dst_swiz);

  bool TranslateExec(const ucode::instr_cf_exec_t& cf);
  bool TranslateVertexFetch(const ucode::instr_fetch_vtx_t* vtx, int sync);
  bool TranslateTextureFetch(const ucode::instr_fetch_tex_t* tex, int sync);
};

}  // namespace gl4
}  // namespace gpu
}  // namespace xe

#endif  // XENIA_GPU_GL4_GL4_SHADER_TRANSLATOR_H_
