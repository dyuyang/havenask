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
#include "indexlib/base/Status.h"
#include "indexlib/base/Types.h"

namespace indexlibv2::index {
class IIndexer;
class DeletionMapModifier;

class DeletionMapPatcher
{
public:
    DeletionMapPatcher();
    ~DeletionMapPatcher();

public:
    static Status LoadPatch(const std::vector<std::pair<std::shared_ptr<IIndexer>, segmentid_t>>& sourceIndexers,
                            const std::vector<segmentid_t>& targetSegmentIds, bool applyInBranch,
                            DeletionMapModifier* modifier);

private:
    AUTIL_LOG_DECLARE();
};

} // namespace indexlibv2::index
