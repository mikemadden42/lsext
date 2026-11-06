# TODO

Findings from a project review on 2026-11-06. Items were confirmed by running
the tool unless marked as coming from reading the code.

## Decisions needed

- [x] Should symlinks count as files? Yes for links to files. Dangling links
      and links to directories are skipped.
- [ ] Should extensions be case-insensitive?
- [x] Should dotfiles count as having no extension? Yes.

## Bugs

- [x] **Files can go missing without warning (`lsext.cc:17`).** The
      `d_type == DT_REG` check drops symlinks to files (`link.txt` never
      appeared in testing). This may be intended, but it isn't documented.
      From reading the code: some filesystems return `DT_UNKNOWN`, which would
      drop every file, print nothing and exit 0. Not reproduced.
- [x] **Write failures are ignored.** With stdout closed, it printed nothing
      and still exited 0. Check `cout` before returning.
- [x] **Extension parsing gives odd groups (`lsext.cc:19`).**
      `std::filesystem::path::extension()` handles the dotfile and no-dot
      cases, and the project is already on C++17. Files without a dot only work
      because `npos + 1` wraps to 0.

      | File                     | Result                                       | Likely expected            |
      |--------------------------|----------------------------------------------|----------------------------|
      | `Makefile`, `README`     | each gets its own group, named after the file | shared "no extension" group |
      | `.bashrc`, `.gitignore`  | grouped as `bashrc`, `gitignore`             | no extension               |
      | `trailing.`              | blank `:` heading                            | no extension               |
      | `a.txt`, `B.TXT`, `d.Txt` | three separate groups                       | possibly one               |

- [x] **Arguments are ignored.** `lsext --help` and `lsext sub` both list the
      current directory, so `lsext sub` shows the wrong directory with no error.
- [x] **The error message has no context (`lsext.cc:25`).** `perror("")` prints
      only `Permission denied`; `perror("lsext")` would name the source.

## Build and CI

- [x] **CI runs no tests.** `tests/run_tests.sh` now runs through CTest.
- [x] **CMake overrides user settings.**
  - `CMakeLists.txt:7` replaces `CMAKE_CXX_FLAGS`, so `CXXFLAGS` and
    `-DCMAKE_CXX_FLAGS` are dropped, which blocks things like sanitizer builds.
    Use `target_compile_options` instead.
  - `CMakeLists.txt:14` forces both architectures, so
    `-DCMAKE_OSX_ARCHITECTURES=arm64` still produces a universal binary.
- [x] **CI only runs on Ubuntu.** The macOS universal build is never exercised
      there.
- [x] Update `actions/checkout@v3`, which is out of date.

## Minor cleanup

- [x] Include `<string>`, `<cstdio>` and `<cstdlib>` directly. They are used
      but only pulled in indirectly.
- [x] Avoid copying each key and vector just to sort them in the output loop.
      Sort in place with `auto &`, or use `map<string, set<string>>`.
- [x] Remove the `if (NOT MSVC)` branch. Windows isn't a supported platform.
- [x] Replace `endl` with `'\n'`. This is a style issue: 50,000 files still took
      only 0.23s.
- [x] Add an `install()` rule.
- [x] Add build instructions to the README.

