// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "tnn/network/torch/tnn_impl_torch.h"

#include "tnn/core/instance.h"
#include "tnn/interpreter/abstract_model_interpreter.h"

namespace TNN_NS {

TNNImplTorch::TNNImplTorch() {}

TNNImplTorch::~TNNImplTorch() {}

Status TNNImplTorch::Init(ModelConfig& config) {
    model_config_ = config;
    TNNImpl::Init(config);
    auto interpreter = CreateModelInterpreter(config.model_type);
    if (!interpreter) {
        return Status(TNNERR_NET_ERR, "interpreter is nil from TNNImplTorch");
    }
    interpreter_ = std::shared_ptr<AbstractModelInterpreter>(interpreter);
    return interpreter_->Interpret(config.params);
}

Status TNNImplTorch::DeInit() {
    return TNN_OK;
}

Status TNNImplTorch::AddOutput(const std::string& layer_name, int output_index) {
    return Status(TNNERR_COMMON_ERROR, "TNNImplTorch can not add output");
}

Status TNNImplTorch::GetModelInputShapesMap(InputShapesMap& shapes_map) {
    return Status(TNNERR_COMMON_ERROR, "TNNImplTorch can not get model input shapes map");
}

Status TNNImplTorch::GetModelInputNames(std::vector<std::string>& input_names) {
    return Status(TNNERR_NET_ERR, "Error: CoreML do not supprt get model input names");
}

Status TNNImplTorch::GetModelOutputNames(std::vector<std::string>& output_names) {
    return Status(TNNERR_NET_ERR, "Error: CoreML do not supprt get model output names");
}

std::shared_ptr<Instance> TNNImplTorch::CreateInst(NetworkConfig& net_config, Status& status,
                                                   InputShapesMap inputs_shape) {

    auto instance = std::make_shared<Instance>(net_config, model_config_);
    status        = instance->Init(interpreter_, inputs_shape);
    return instance;
}

std::shared_ptr<Instance> TNNImplTorch::CreateInst(NetworkConfig& net_config, Status& status,
                                                   InputShapesMap min_inputs_shape, InputShapesMap max_inputs_shape) {
    auto instance = std::make_shared<Instance>(net_config, model_config_);
    status        = instance->Init(interpreter_, min_inputs_shape, max_inputs_shape);
    return instance;
}

TNNImplFactoryRegister<TNNImplFactory<TNNImplTorch>> g_tnn_impl_torch_factory_register(MODEL_TYPE_TORCHSCRIPT);

}  // namespace TNN_NS
