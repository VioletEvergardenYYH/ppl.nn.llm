// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "cast_kernel.h"

#include "cudakernel/unary/cast.h"

namespace ppl { namespace nn { namespace llm { namespace cuda { namespace onnx {

ppl::common::RetCode CastKernel::DoExecute(KernelExecContext* ctx) {
    PPLNN_LLM_CUDA_DEBUG_TRACE("Entry LlmCudaKernel: [%s]\n", GetName().c_str());

    PPLNN_LLM_CUDA_REQUIRED_INPUT(input, 0);
    PPLNN_LLM_CUDA_REQUIRED_OUTPUT(output, 0);

    PPLNN_LLM_CUDA_DEBUG_TRACE("Input [input]:\n");
    PPLNN_LLM_CUDA_TENSOR_PRINT_DEBUG_MSG(input);

    PPLNN_LLM_CUDA_DEBUG_TRACE("to: %s\n", ppl::common::GetDataTypeStr(param_->to));

    bool can_trans = ctx->IsLastConsumerOfInput(0)
        && input->GetType() == TENSORTYPE_NORMAL
        && input->GetShape()->GetDataType() == output->GetShape()->GetDataType();

    if (can_trans) {
        output->TransferBufferFrom(input);
    } else {
        PPLNN_LLM_CUDA_REALLOC_TENSOR_BUFFER(output);
    }
    PPLNN_LLM_CUDA_DEBUG_TRACE("Output [output]:\n");
    PPLNN_LLM_CUDA_TENSOR_PRINT_DEBUG_MSG(output);

    if (!can_trans) {
        return PPLCUDACastForwardImp(
            GetStream(),
            input->GetShape(),
            input->GetBufferPtr(),
            output->GetShape(),
            output->GetBufferPtr(),
            param_->to);
    }

    return ppl::common::RC_SUCCESS;
}

}}}}} // namespace ppl::nn::llm::cuda::pmx
