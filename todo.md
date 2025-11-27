
tests:
  - paths in:
    * [ ] diagnostics from source file
    * [ ] diagnostics from included file (try abs and relative, verbatim style should match how it was specified)
    * [ ] diagnostics about command file
    * [ ] diagnostics from file specified in command file:
      + [ ] abs path -> abs path
      + [ ] relative path w/`-f` -> relative path
      + [ ] relative path `-F` -> stacked relative path (no normalization so... `foo/bar/list.F` having `../../hey.sv` should yield `foo/bar/../../hey.sv` when using verbatim!)
    * [ ] generated line directives
    * [ ] generated dep files

  - iter directories recursive tests:
    * [ ] case where dir contains a symlink to `..`: want to visit initial dir only once (not a correctness issue, just perf)

apply changes to tools:
  - [ ] tidy
