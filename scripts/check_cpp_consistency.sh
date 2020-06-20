#!/usr/bin/env bash

exit-with-error() {
  if [[ $# -ne 1 ]]; then
    echo "exit-with-error expects a single argument."
    return 1
  fi
  echo "$1"
  exit 1
}

# Ensure all headers start with #pragma once.
diff <(find . -wholename "./src/*.hpp" | wc -l) <(git grep "#pragma once" -- src/*.hpp | wc -l) || exit-with-error "Not all sources use #pragma once."
diff <(find . -wholename "./test/*.hpp" | wc -l) <(git grep "#pragma once" -- test/*.hpp | wc -l) || exit-with-error "Not all tests use #pragma once."

# Ensure our namespace is always used.
diff <(find . -wholename "./src/*.hpp" | wc -l) <(git grep "namespace WayoutPlayer {" -- src/*.hpp | wc -l) || exit-with-error "Not all sources use the right namespace."
diff <(find . -wholename "./test/*.hpp" | wc -l) <(git grep "namespace WayoutPlayer::Tests {" -- test/*.hpp | wc -l) || exit-with-error "Not all tests use the right namespace."
