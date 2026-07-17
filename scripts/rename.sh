#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"
TEMPLATE_DIR="$REPO_ROOT/template"

replace_string_in_directory() {
    local directory="$1"
    local search_string="$2"
    local replace_string="$3"
    local file

    # Process substitution (not a pipe) so a zero-match grep doesn't trip
    # "set -o pipefail" and silently abort the script under "set -e".
    while IFS= read -r file; do
        echo "Processing file: $file"
        sed -i.bak "s#$search_string#$replace_string#g" "$file"
        rm -f "$file.bak"
    done < <(grep -rlF --binary-files=without-match -- "$search_string" "$directory" || true)
}

rename_paths_with_string() {
    local directory="$1"
    local search_string="$2"
    local replace_string="$3"

    # -depth: rename children before their parent directory, so a renamed
    # parent doesn't invalidate the path of a not-yet-processed child.
    find "$directory" -depth -name "*$search_string*" | while IFS= read -r path; do
        new_path="${path//$search_string/$replace_string}"
        echo "Renaming: $path -> $new_path"
        mv "$path" "$new_path"
    done
}

NEW_PROJECT_NAME="${1:-}"
if [ -z "$NEW_PROJECT_NAME" ]; then
    read -r -p "Enter the new project name: " NEW_PROJECT_NAME
fi

if [ -z "$NEW_PROJECT_NAME" ]; then
    echo "Project name cannot be empty."
    exit 1
fi

DEST_DIR="${2:-$(dirname "$REPO_ROOT")/$NEW_PROJECT_NAME}"

NEW_PROJECT_NAME_LOWERCASE=$(echo "$NEW_PROJECT_NAME" | tr '[:upper:]' '[:lower:]')
NEW_PROJECT_NAME_UPPERCASE=$(echo "$NEW_PROJECT_NAME" | tr '[:lower:]' '[:upper:]')

echo "New project name: $NEW_PROJECT_NAME"
echo "  lowercase:       $NEW_PROJECT_NAME_LOWERCASE"
echo "  uppercase:       $NEW_PROJECT_NAME_UPPERCASE"
echo "Destination:       $DEST_DIR"

read -r -p "Are you sure you want to continue? (y/n): " choice
case "$choice" in
    y|Y ) ;;
    * ) echo "Operation aborted."; exit 1 ;;
esac

if [ -e "$DEST_DIR" ]; then
    echo "Destination '$DEST_DIR' already exists. Aborting."
    exit 1
fi

mkdir -p "$DEST_DIR"
cp -R "$TEMPLATE_DIR/." "$DEST_DIR/"

replace_string_in_directory "$DEST_DIR" "ExampleDSP" "$NEW_PROJECT_NAME"
replace_string_in_directory "$DEST_DIR" "EXAMPLEDSP" "$NEW_PROJECT_NAME_UPPERCASE"
replace_string_in_directory "$DEST_DIR" "exampledsp" "$NEW_PROJECT_NAME_LOWERCASE"

rename_paths_with_string "$DEST_DIR" "ExampleDSP" "$NEW_PROJECT_NAME"
rename_paths_with_string "$DEST_DIR" "EXAMPLEDSP" "$NEW_PROJECT_NAME_UPPERCASE"
rename_paths_with_string "$DEST_DIR" "exampledsp" "$NEW_PROJECT_NAME_LOWERCASE"

git init -q "$DEST_DIR"
git -C "$DEST_DIR" add -A
if ! git -C "$DEST_DIR" commit -q -m "Initial commit"; then
    echo "Warning: could not create the initial commit (configure 'git config user.name/user.email'). Commit manually when ready."
fi

echo "Done. New project created at: $DEST_DIR"
