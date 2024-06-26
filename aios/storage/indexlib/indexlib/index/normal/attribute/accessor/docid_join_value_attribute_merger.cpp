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
#include "indexlib/index/normal/attribute/accessor/docid_join_value_attribute_merger.h"

#include "indexlib/config/attribute_config.h"
#include "indexlib/file_system/Directory.h"
#include "indexlib/file_system/file/BufferedFileWriter.h"
#include "indexlib/index/util/reclaim_map.h"

using namespace std;
using namespace indexlib::config;
using namespace indexlib::file_system;
using namespace indexlib::index_base;

namespace indexlib { namespace index {
IE_LOG_SETUP(index, DocidJoinValueAttributeMerger);

DocidJoinValueAttributeMerger::DocidJoinValueAttributeMerger() : AttributeMerger(false) {}

DocidJoinValueAttributeMerger::~DocidJoinValueAttributeMerger() {}

void DocidJoinValueAttributeMerger::Init(const AttributeConfigPtr& attrConfig)
{
    FieldType fieldType = attrConfig->GetFieldType();
    const std::string fieldName = attrConfig->GetAttrName();

    if (attrConfig->IsMultiValue()) {
        INDEXLIB_FATAL_ERROR(UnSupported, "attribute[%s] should not be multiValue", fieldName.c_str());
    }

    if (fieldName != MAIN_DOCID_TO_SUB_DOCID_ATTR_NAME && fieldName != SUB_DOCID_TO_MAIN_DOCID_ATTR_NAME) {
        INDEXLIB_FATAL_ERROR(UnSupported, "attribute[%s] not match name:%s or %s", fieldName.c_str(),
                             MAIN_DOCID_TO_SUB_DOCID_ATTR_NAME, SUB_DOCID_TO_MAIN_DOCID_ATTR_NAME);
    }

    if (fieldType != ft_int32) {
        INDEXLIB_FATAL_ERROR(UnSupported, "attribute[%s] is not ft_int32 type", fieldName.c_str());
    }
    AttributeMerger::Init(attrConfig);
}

void DocidJoinValueAttributeMerger::Merge(const MergerResource& resource, const SegmentMergeInfos& segMergeInfos,
                                          const OutputSegmentMergeInfos& outputSegMergeInfos)
{
    SortByWeightMerge(resource, segMergeInfos, outputSegMergeInfos);
}

void DocidJoinValueAttributeMerger::SortByWeightMerge(const MergerResource& resource,
                                                      const SegmentMergeInfos& segMergeInfos,
                                                      const OutputSegmentMergeInfos& outputSegMergeInfos)
{
    IE_LOG(INFO, "Start dumping join attribute data for attribute : %s", mAttributeConfig->GetAttrName().c_str());
    std::vector<file_system::FileWriterPtr> dataFiles;
    dataFiles.reserve(outputSegMergeInfos.size());
    for (const auto& outputSegInfo : outputSegMergeInfos) {
        dataFiles.push_back(CreateDataFileWriter(outputSegInfo.directory));
    }
    const auto& reclaimMap = resource.reclaimMap;

    const auto& fieldName = mAttributeConfig->GetAttrName();

    if (fieldName == MAIN_DOCID_TO_SUB_DOCID_ATTR_NAME) {
        MainToSubMerge(resource, dataFiles);
    } else {
        SubToMainMerge(resource, dataFiles);
    }

    for (const auto& dataFile : dataFiles) {
        dataFile->Close().GetOrThrow();
    }
    IE_LOG(INFO, "Finish dumping join attribute data for attribute : %s", mAttributeConfig->GetAttrName().c_str());
    reclaimMap->ReleaseJoinValueArray();
}

void DocidJoinValueAttributeMerger::MainToSubMerge(const MergerResource& resource,
                                                   const std::vector<file_system::FileWriterPtr>& dataFiles)
{
    const auto& reclaimMap = resource.reclaimMap;
    std::vector<int64_t> targetDocOffset(dataFiles.size(), 0);
    docid_t lastDocJoinValue = 0;
    const auto& joinValueArray = reclaimMap->GetJoinValueArray();

    for (size_t docId = 0; docId < joinValueArray.size(); ++docId) {
        auto targetSegIdx = reclaimMap->GetTargetSegmentIndex(docId);
        auto curDocJoinValue = joinValueArray[docId];
        int64_t diff = curDocJoinValue - lastDocJoinValue;
        targetDocOffset[targetSegIdx] += diff;
        const auto& dataFile = dataFiles[targetSegIdx];
        docid_t targetJoinValue = targetDocOffset[targetSegIdx];
        dataFile->Write(&targetJoinValue, sizeof(targetJoinValue)).GetOrThrow();
        lastDocJoinValue = curDocJoinValue;
    }
}

void DocidJoinValueAttributeMerger::SubToMainMerge(const MergerResource& resource,
                                                   const std::vector<file_system::FileWriterPtr>& dataFiles)
{
    const auto& reclaimMap = resource.reclaimMap;
    std::vector<int64_t> targetDocOffset(dataFiles.size(), 0);
    docid_t lastDocJoinValue = 0;
    int64_t lastSegIdx = -1;
    const auto& joinValueArray = reclaimMap->GetJoinValueArray();
    for (size_t docId = 0; docId < joinValueArray.size(); ++docId) {
        auto targetSegIdx = reclaimMap->GetTargetSegmentIndex(docId);
        auto curDocJoinValue = joinValueArray[docId];
        int64_t diff = 0;
        if (lastSegIdx != static_cast<int64_t>(targetSegIdx)) {
            diff = curDocJoinValue - GetBaseDocId(resource.mainBaseDocIds, targetSegIdx);
            lastSegIdx = targetSegIdx;
        } else {
            diff = curDocJoinValue - lastDocJoinValue;
        }
        targetDocOffset[targetSegIdx] += diff;
        const auto& dataFile = dataFiles[targetSegIdx];
        docid_t targetJoinValue = targetDocOffset[targetSegIdx];
        dataFile->Write(&targetJoinValue, sizeof(targetJoinValue)).GetOrThrow();
        lastDocJoinValue = curDocJoinValue;
    }
}

// docid_t DocidJoinValueAttributeMerger::MainToSubJoinValue(
//     docid_t lastDocJoinValue, docid_t curDocJoinValue, bool newSegment, size_t& segDocCount)
// {
//     size_t joinDocCount = curDocJoinValue - lastDocJoinValue;
//     segDocCount += joinDocCount;
//     return static_cast<docid_t>(segDocCount);
// }

// docid_t DocidJoinValueAttributeMerger::SubToMainJoinValue(
//     docid_t lastDocJoinValue, docid_t curDocJoinValue, bool newSegment, size_t& segDocCount)
// {
//     if (newSegment)
//     {
//         return segDocCount;
//     }
//     size_t mainDocOffset = curDocJoinValue - lastDocJoinValue;
//     segDocCount += mainDocOffset;
// }

file_system::FileWriterPtr DocidJoinValueAttributeMerger::CreateDataFileWriter(const DirectoryPtr& attributeDir)
{
    RemoveOption removeOption = RemoveOption::MayNonExist();
    attributeDir->RemoveDirectory(mAttributeConfig->GetAttrName(), removeOption);
    DirectoryPtr directory = attributeDir->MakeDirectory(mAttributeConfig->GetAttrName());
    return directory->CreateFileWriter(ATTRIBUTE_DATA_FILE_NAME);
}

int64_t DocidJoinValueAttributeMerger::EstimateMemoryUse(const SegmentDirectoryBasePtr& segDir,
                                                         const MergerResource& resource,
                                                         const SegmentMergeInfos& segMergeInfos,
                                                         const OutputSegmentMergeInfos& outputSegMergeInfos,
                                                         bool isSortedMerge) const
{
    return sizeof(*this);
}
}} // namespace indexlib::index
