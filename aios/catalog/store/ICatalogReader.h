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

#include <string>

#include "catalog/entity/Type.h"
#include "catalog/util/StatusBuilder.h"

namespace catalog {

class ICatalogReader {
public:
    ICatalogReader() = default;
    virtual ~ICatalogReader() {}
    ICatalogReader(const ICatalogReader &) = delete;
    ICatalogReader &operator=(const ICatalogReader &) = delete;

public:
    virtual Status getLatestVersion(CatalogVersion &version) = 0;
    virtual Status read(CatalogVersion version, proto::Catalog *proto) = 0;
    // for build
    virtual Status read(std::vector<proto::Build> *builds) = 0;
};

} // namespace catalog
