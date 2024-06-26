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
#pragma once

#include "autil/Log.h"
#include "autil/NoCopyable.h"
#include "indexlib/file_system/LifecycleConfig.h"
#include "indexlib/framework/lifecycle/LifecycleStrategy.h"

namespace indexlib::framework {
class LifecycleStrategyFactory : public autil::NoCopyable
{
public:
    LifecycleStrategyFactory() {}
    ~LifecycleStrategyFactory() {}

public:
    static const std::string DYNAMIC_STRATEGY;
    static const std::string STATIC_STRATEGY;

public:
    static std::unique_ptr<LifecycleStrategy>
    CreateStrategy(const indexlib::file_system::LifecycleConfig& lifecycleConfig,
                   const std::map<std::string, std::string>& parameters);

private:
    AUTIL_LOG_DECLARE();
};

} // namespace indexlib::framework
