/*
 * Copyright 2014-present Alibaba Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "navi/ops/NullSourceKernel.h"

namespace navi {

const std::string NullSourceKernel::KERNEL_ID = "navi.null_source_k";
const std::string NullSourceKernel::OUTPUT_PORT = "navi.null_source_k.out";

NullSourceKernel::NullSourceKernel() {
}

NullSourceKernel::~NullSourceKernel() {
}

void NullSourceKernel::def(navi::KernelDefBuilder &builder) const {
    builder.name(KERNEL_ID)
        .output(OUTPUT_PORT, "");
}

navi::ErrorCode NullSourceKernel::init(navi::KernelInitContext &ctx) {
    return navi::EC_NONE;
}

navi::ErrorCode NullSourceKernel::compute(navi::KernelComputeContext &ctx) {
    ctx.setOutput(0, nullptr, true);
    return navi::EC_NONE;
}

REGISTER_KERNEL(NullSourceKernel);

}
