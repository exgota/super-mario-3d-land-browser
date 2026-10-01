# Source this file before running any project tool: . ./development_environment.sh
# It activates the Python environment and points the differ at Homebrew's ARM binutils.
repository_root="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"
. "$repository_root/.venv/bin/activate"
export DEVKITARM=/opt/homebrew
