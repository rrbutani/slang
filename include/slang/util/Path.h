//------------------------------------------------------------------------------
//! @file Path.h
//! @brief Wrapper types around `std::filesystem::path`
//
// SPDX-FileCopyrightText: Michael Popoloski
// SPDX-License-Identifier: MIT
//------------------------------------------------------------------------------
#pragma once

#include <filesystem>
#include <string_view>
#include <system_error>

namespace slang {

/// Path that has been canonicalized (with `weakly_canonical`).
///
/// Normalized, absolute, weakly canonical.
struct CanonicalPath {
    CanonicalPath(const std::filesystem::path& raw) :
        inner(std::filesystem::weakly_canonical(raw)) {}
    CanonicalPath(const std::filesystem::path& raw, std::error_code& ec) :
        inner(std::filesystem::weakly_canonical(raw, ec)) {}

    const std::filesystem::path& operator->() const { return inner; }
    const std::filesystem::path& operator*() const { return inner; }

    auto operator<=>(const CanonicalPath& rhs) { return **this <=> rhs.inner; }

private:
    std::filesystem::path inner;
};

/// Raw path, as specified by a user.
///
/// Not normalized or canonicalized.
struct RawPath {
    RawPath(std::filesystem::path&& raw) : inner(std::move(raw)) {}
    RawPath(const std::filesystem::path& raw) : inner(raw) {}

    const std::filesystem::path& operator->() const { return inner; }
    const std::filesystem::path& operator*() const { return inner; }

    auto operator<=>(const RawPath& rhs) { return **this <=> rhs.inner; }

    template<class Source>
    void operator/=(const Source& source) {
        this->inner /= source;
    }

    inline CanonicalPath asCanonical() const { return CanonicalPath(**this); }

    static const RawPath Empty;

private:
    std::filesystem::path inner;
};

} // namespace slang

namespace std {

template<>
struct hash<slang::CanonicalPath> {
    size_t operator()(const slang::CanonicalPath& p) const { return hash<filesystem::path>{}(*p); }
};

// NOTE: should not use `RawPath`s as keys in maps (they are not necessarily
// unique) so no `std::hash<>` specialization

// template<>
// struct hash<slang::RawPath> {
//     size_t operator()(const slang::RawPath& p) const { return hash<filesystem::path>{}(*p); }
// };

} // namespace std

// TODO: split off impls from header
