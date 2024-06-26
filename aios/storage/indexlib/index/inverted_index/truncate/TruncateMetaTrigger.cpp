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
#include "indexlib/index/inverted_index/truncate/TruncateMetaTrigger.h"

namespace indexlib::index {
AUTIL_LOG_SETUP(index, TruncateMetaTrigger);

bool TruncateMetaTrigger::NeedTruncate(const TruncateTriggerInfo& info) const
{
    if (!_metaReader) {
        AUTIL_LOG(WARN, "truncate meta reader not exist in index");
        return false;
    }
    return _metaReader->IsExist(info.GetDictKey());
}

void TruncateMetaTrigger::SetTruncateMetaReader(const std::shared_ptr<TruncateMetaReader>& reader)
{
    _metaReader = reader;
}
} // namespace indexlib::index
