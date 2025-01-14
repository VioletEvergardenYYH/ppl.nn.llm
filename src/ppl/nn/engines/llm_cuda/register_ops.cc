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

#include "opt_kernel_creator_manager.h"

#include "ppl/nn/common/logger.h"

#include "ppl/nn/engines/llm_cuda/ops/onnx/cast_op.h"
#include "ppl/nn/engines/llm_cuda/ops/onnx/gather_op.h"
#include "ppl/nn/engines/llm_cuda/ops/onnx/reshape_op.h"
#include "ppl/nn/engines/llm_cuda/ops/onnx/slice_op.h"
#include "ppl/nn/engines/llm_cuda/ops/onnx/split_op.h"
#include "ppl/nn/engines/llm_cuda/ops/onnx/sub_op.h"

#include "ppl/nn/engines/llm_cuda/ops/pmx/column_parallel_linear_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/key_value_cache_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/multi_head_attention_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/parallel_embedding_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/rms_norm_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/rotary_position_embedding_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/row_parallel_linear_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/silu_op.h"

#include "ppl/nn/engines/llm_cuda/ops/pmx/dynamic_batching_key_value_cache_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/dynamic_batching_multi_head_attention_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/dynamic_batching_multi_head_cache_attention_op.h"
#include "ppl/nn/engines/llm_cuda/ops/pmx/dynamic_batching_rotary_position_embedding_op.h"

using namespace std;
using namespace ppl::common;

namespace ppl { namespace nn { namespace llm { namespace cuda {

template <typename T>
static LlmCudaOptKernel* GenericCreateOptKernel(const ir::Node* node) {
    return new T(node);
}

template <typename T>
static void RegisterOptKernelCreator(const string& domain, const string& type, uint64_t first_version,
                                     uint64_t last_version) {
    if (last_version < first_version) {
        LOG(ERROR) << "register op[" << domain << ":" << type << "] failed: last_version[" << last_version
                   << "] < first_version[" << first_version << "]";
        exit(-1);
    }
    OptKernelCreatorManager::GetInstance()->Register(domain, type, utils::VersionRange(first_version, last_version),
                                                     GenericCreateOptKernel<T>);
}

void RegisterBuiltinOpImpls() {
    static bool ops_are_registered = false;
    if (ops_are_registered) {
        return;
    }
    ops_are_registered = true;

    /*                                                                        */
    /*                                 ONNX                                   */
    /*                                                                        */
    // A
    // B
    // C
    RegisterOptKernelCreator<onnx::CastOp>("", "Cast", 9, 16);
    // D
    // E
    // F
    // G
    RegisterOptKernelCreator<onnx::GatherOp>("", "Gather", 1, 16);
    // H
    // I
    // J
    // K
    // L
    // M
    // N
    // O
    // P
    // Q
    // R
    RegisterOptKernelCreator<onnx::ReshapeOp>("", "Reshape", 5, 16);
    // S
    RegisterOptKernelCreator<onnx::SliceOp>("", "Slice", 1, 16);
    RegisterOptKernelCreator<onnx::SplitOp>("", "Split", 2, 16);
    RegisterOptKernelCreator<onnx::SubOp>("", "Sub", 7, 16);
    // T
    // U
    // V
    // W
    // X
    // Y
    // Z

    /*                                                                        */
    /*                                 PMX                                    */
    /*                                                                        */
    // A
    // B
    // C
    RegisterOptKernelCreator<pmx::ColumnParallelLinearOp>("pmx", "ColumnParallelLinear", 1, 1);
    // D
    // E
    // F
    // G
    // H
    // I
    // J
    // K
    RegisterOptKernelCreator<pmx::KeyValueCacheOp>("pmx", "KeyValueCache", 1, 1);
    // L
    // M
    // N
    RegisterOptKernelCreator<pmx::MultiHeadAttentionOp>("pmx", "MultiHeadAttention", 1, 1);
    // O
    // P
    RegisterOptKernelCreator<pmx::ParallelEmbeddingOp>("pmx", "ParallelEmbedding", 1, 1);
    // Q
    // R
    RegisterOptKernelCreator<pmx::RMSNormOp>("pmx", "RMSNorm", 1, 1);
    RegisterOptKernelCreator<pmx::RotaryPositionEmbeddingOp>("pmx", "RotaryPositionEmbedding", 1, 1);
    RegisterOptKernelCreator<pmx::RowParallelLinearOp>("pmx", "RowParallelLinear", 1, 1);
    // S
    RegisterOptKernelCreator<pmx::SiLUOp>("pmx", "SiLU", 1, 1);
    // T
    // U
    // V
    // W
    // X
    // Y
    // Z

    /*                                                                        */
    /*                        PMX.DYNAMIC_BATCHING                            */
    /*                                                                        */
    // A
    // B
    // C
    // D
    // E
    // F
    // G
    // H
    // I
    // J
    // K
    RegisterOptKernelCreator<pmx::DynamicBatchingKeyValueCacheOp>("pmx.dynamic_batching", "KeyValueCache", 1, 1);
    // L
    // M
    // N
    RegisterOptKernelCreator<pmx::DynamicBatchingMultiHeadAttentionOp>("pmx.dynamic_batching", "MultiHeadAttention", 1, 1);
    RegisterOptKernelCreator<pmx::DynamicBatchingMultiHeadCacheAttentionOp>("pmx.dynamic_batching", "MultiHeadCacheAttention", 1, 1);
    // O
    // P
    // Q
    // R
    RegisterOptKernelCreator<pmx::DynamicBatchingRotaryPositionEmbeddingOp>("pmx.dynamic_batching", "RotaryPositionEmbedding", 1, 1);
    // S
    // T
    // U
    // V
    // W
    // X
    // Y
    // Z
}

}}}} // namespace ppl::nn::llm::cuda
