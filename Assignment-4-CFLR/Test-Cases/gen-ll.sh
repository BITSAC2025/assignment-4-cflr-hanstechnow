set -euo pipefail

CLANG=${CLANG:-clang}
OPT=${OPT:--O0}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
shopt -s nullglob

for src in "$SCRIPT_DIR"/*.c; do
  out="${src%.c}.ll"
  "$CLANG" -S -emit-llvm $OPT "$src" -o "$out"
  echo "generated $(basename "$out")"
done