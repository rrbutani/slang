//------------------------------------------------------------------------------
//! @file DiagnosticClient.h
//! @brief Client interface for diagnostic rendering
//
// SPDX-FileCopyrightText: Michael Popoloski
// SPDX-License-Identifier: MIT
//------------------------------------------------------------------------------
#pragma once

#include "slang/diagnostics/DiagnosticEngine.h"
#include "slang/util/Util.h"

namespace slang {

#define COLUMN_UNIT(x) x(Byte) x(Display)
SLANG_ENUM(ColumnUnit, COLUMN_UNIT)
#undef COLUMN_UNIT

#define PATH_STYLE(x) x(Verbatim) x(Canonical) x(Proximate)
SLANG_ENUM(PathStyle, PATH_STYLE)
#undef PATH_STYLE

/// A base class for diagnostic clients, which receive issued diagnostics
/// and present them to the user in some form.
class SLANG_EXPORT DiagnosticClient {
public:
    virtual ~DiagnosticClient() = default;

    /// Called when a diagnostic is issued by the engine.
    virtual void report(const ReportedDiagnostic& diagnostic) = 0;

    /// Sets the engine that this client is associated with.
    /// This is called by the engine when the client is added to it.
    void setEngine(const DiagnosticEngine& engine);

    /// Sets the style to use for displayed filenames for diagnostics.
    ///
    /// @a style values:
    ///   * @ref PathStyle::Verbatim
    ///     + report paths as they were passed in: no normalization, no symlink
    ///       resolution, no conversion to absolute paths
    ///   * @ref PathStyle::Canonical
    ///     + report canonicalized (normalized, symlinks resolved, absolute)
    ///       paths
    ///   * @ref PathStyle::Proximate
    ///     + convert reported paths to relative to the current working
    ///       directory
    ///     + @note paths are canonicalized *first* and then made relative
    void setPathStyle(PathStyle style) { pathStyle = style; }

    /// Sets the unit used for displaying column numbers.
    void setColumnUnit(ColumnUnit unit) { columnUnit = unit; }

protected:
    const DiagnosticEngine* engine = nullptr;
    const SourceManager* sourceManager = nullptr;
    ColumnUnit columnUnit = ColumnUnit::Byte;
    PathStyle pathStyle = PathStyle::Proximate;

    std::string getFileName(SourceLocation location) const;
    void getIncludeStack(BufferID buffer, SmallVectorBase<SourceLocation>& stack) const;
    std::string_view getSourceLine(SourceLocation location, size_t col) const;
    size_t getColumnNumber(SourceLocation location) const;
    static std::string_view getSeverityString(DiagnosticSeverity severity);
};

} // namespace slang


// todo: for -F paths at least tack on the `-F` path
