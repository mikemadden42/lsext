#!/bin/sh
# Usage: run_tests.sh /absolute/path/to/lsext

lsext=$1
failures=0

work=$(mktemp -d) || exit 1
trap 'chmod -R u+rwx "$work"; rm -rf "$work"' EXIT

fail() {
    echo "FAIL: $1"
    failures=$((failures + 1))
}

# Compare the output of lsext in the current directory to stdin.
expect_output() {
    name=$1
    cat >"$work/expected"
    "$lsext" >"$work/actual" 2>&1
    status=$?
    if [ "$status" -ne 0 ]; then
        fail "$name: exited $status"
    elif ! diff -u "$work/expected" "$work/actual"; then
        fail "$name: unexpected output"
    fi
}

# Expect lsext in the current directory to exit 1 and report the message.
expect_failure() {
    name=$1
    message=$2
    "$lsext" 2>"$work/stderr" >&-
    status=$?
    if [ "$status" -ne 1 ]; then
        fail "$name: exited $status"
    elif ! grep -q "$message" "$work/stderr"; then
        fail "$name: stderr was: $(cat "$work/stderr")"
    fi
}

mkdir "$work/grouping" && cd "$work/grouping" || exit 1
touch zebra.txt apple.txt mango.txt notes.md archive.tar.gz
mkdir subdir
expect_output "groups and sorts files, skipping directories" <<'END'
gz:
- archive.tar.gz

md:
- notes.md

txt:
- apple.txt
- mango.txt
- zebra.txt

END

mkdir "$work/no-extension" && cd "$work/no-extension" || exit 1
touch Makefile README .bashrc .txt trailing.
expect_output "groups names without an extension" <<'END'
(no extension):
- .bashrc
- .txt
- Makefile
- README
- trailing.

END

mkdir "$work/symlinks" && cd "$work/symlinks" || exit 1
touch real.txt
mkdir subdir
ln -s real.txt link.txt
ln -s missing.txt dangling.txt
ln -s subdir link.dir
expect_output "lists links to files only" <<'END'
txt:
- link.txt
- real.txt

END

mkdir "$work/empty" && cd "$work/empty" || exit 1
expect_output "prints nothing for an empty directory" </dev/null

mkdir "$work/closed-stdout" && cd "$work/closed-stdout" || exit 1
touch a.txt
expect_failure "fails when stdout is closed" "error writing output"

# Root ignores directory permissions.
if [ "$(id -u)" -ne 0 ]; then
    mkdir "$work/unreadable" && cd "$work/unreadable" || exit 1
    chmod 311 .
    expect_failure "fails on an unreadable directory" "lsext: "
    chmod 755 .
fi

if [ "$failures" -ne 0 ]; then
    echo "$failures test(s) failed"
    exit 1
fi
echo "All tests passed"
