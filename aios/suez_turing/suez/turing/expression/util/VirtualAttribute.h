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

#include <map>
#include <memory>
#include <stddef.h>
#include <string>
#include <vector>

#include "matchdoc/ValueType.h"
#include "suez/turing/expression/common.h"
#include "suez/turing/expression/syntax/SyntaxExpr.h"

namespace autil {
class DataBuffer;
} // namespace autil

namespace suez {
namespace turing {

class VirtualAttribute {
public:
    VirtualAttribute() : _syntaxExpr(NULL){};
    VirtualAttribute(const std::string &name, SyntaxExpr *syntaxexpr);
    ~VirtualAttribute();

private:
    VirtualAttribute(const VirtualAttribute &);
    VirtualAttribute &operator=(const VirtualAttribute &);

public:
    void serialize(autil::DataBuffer &dataBuffer) const;
    void deserialize(autil::DataBuffer &dataBuffer);

    const std::string &getVirtualAttributeName() const { return _name; }
    SyntaxExpr *getVirtualAttributeSyntaxExpr() const { return _syntaxExpr; }
    matchdoc::BuiltinType getResultType() const {
        if (_syntaxExpr) {
            return _syntaxExpr->getExprResultType();
        }
        return vt_unknown;
    }
    bool isMultiValue() const {
        if (_syntaxExpr) {
            return _syntaxExpr->isMultiValue();
        }
        return false;
    }
    std::string getExprString() const {
        std::string exprString = "";
        if (_syntaxExpr) {
            exprString = _syntaxExpr->getExprString();
        }
        return exprString;
    }

private:
    std::string _name;
    SyntaxExpr *_syntaxExpr;
};

typedef std::shared_ptr<VirtualAttribute> VirtualAttributePtr;
typedef std::vector<VirtualAttribute *> VirtualAttributes;
typedef std::map<std::string, SyntaxExpr *> VirtualAttrMap;

} // namespace turing
} // namespace suez
