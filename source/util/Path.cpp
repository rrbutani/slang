//------------------------------------------------------------------------------
// Path.cpp
// Wrapper types around `std::filesystem::path`
//
// SPDX-FileCopyrightText: Michael Popoloski
// SPDX-License-Identifier: MIT
//------------------------------------------------------------------------------
#include "slang/util/Path.h"

namespace slang {

const CanonicalPath CanonicalPath::Empty = CanonicalPath({});

const RawPath RawPath::Empty = RawPath({});

void RawPath::replace_extension(std::string_view ext) {
    this->inner.replace_extension(ext);
}

} // namespace slang
