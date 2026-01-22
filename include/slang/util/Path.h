//------------------------------------------------------------------------------
//! @file Path.h
//! @brief Wrapper types around `std::filesystem::path`
//
// SPDX-FileCopyrightText: Michael Popoloski
// SPDX-License-Identifier: MIT
//------------------------------------------------------------------------------
#pragma once

#include <compare>
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

    auto operator<=>(const CanonicalPath& rhs) const { return **this <=> rhs.inner; }
    bool operator==(const CanonicalPath&) const = default;

    static const CanonicalPath Empty;

#if defined(_WIN32)
    /// Gets a string representation of the given path, in UTF-8 encoding.
    inline std::string asU8Str() const {
        return this->inner.string();
    }
#else
    /// Gets a string representation of the given path, in UTF-8 encoding.
    inline const std::string& asU8Str() const {
        return this->inner.native();
    }
#endif

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

    auto operator<=>(const RawPath& rhs) const { return **this <=> rhs.inner; }
    bool operator==(const RawPath&) const = default;

    template<class Source>
    void operator/=(const Source& source) {
        this->inner /= source;
    }

    void replace_extension(std::string_view ext);

    inline CanonicalPath asCanonical() const { return CanonicalPath(**this); }
    inline CanonicalPath asCanonical(std::error_code& ec) const {
        return CanonicalPath(**this, ec);
    }

    static const RawPath Empty;

#if defined(_WIN32)
    /// Gets a string representation of the given path, in UTF-8 encoding.
    inline std::string asU8Str() const {
        return this->inner.string();
    }
#else
    /// Gets a string representation of the given path, in UTF-8 encoding.
    inline const std::string& asU8Str() const {
        return this->inner.native();
    } // !!! use for reporting and globs
#endif

private:
    std::filesystem::path inner;
};

struct RawPathPattern {
    RawPathPattern(std::string_view&& pattern) : inner(pattern) {}
    RawPathPattern(const std::string_view& pattern) : inner(pattern) {}

    const std::string_view& operator->() const { return inner; }
    const std::string_view& operator*() const { return inner; }

private:
    std::string_view inner;
};

} // namespace slang

namespace std {

template<>
struct hash<const slang::CanonicalPath> {
    size_t operator()(const slang::CanonicalPath& p) const noexcept{ return hash<filesystem::path>{}(*p); }
};

template<>
struct hash<slang::CanonicalPath> {
    size_t operator()(const slang::CanonicalPath& p) const noexcept{ return hash<filesystem::path>{}(*p); }
};

// NOTE: should not use `RawPath`s as keys in maps (they are not necessarily
// unique) so no `std::hash<>` specialization
//
// nevermind?

template<>
struct hash<const slang::RawPath> {
    size_t operator()(const slang::RawPath& p) const noexcept { return hash<filesystem::path>{}(*p); }
};

template<>
struct hash<slang::RawPath> {
    size_t operator()(const slang::RawPath& p) const noexcept { return hash<filesystem::path>{}(*p); }
};

} // namespace std

// TODO: split off impls from header
