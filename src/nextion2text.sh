#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
NEXTION_DIR="$ROOT_DIR/nextion"
OUTPUT_BASE="$NEXTION_DIR/output"
N2T_DIR="$NEXTION_DIR/Nextion2Text"

if [ ! -d "$NEXTION_DIR" ]; then
    echo "ERROR: nextion directory not found: $NEXTION_DIR"
    exit 1
fi

if [ ! -d "$N2T_DIR" ]; then
    echo "Cloning Nextion2Text..."
    git clone https://github.com/MMMZZZZ/Nextion2Text.git "$N2T_DIR"
fi

shopt -s nullglob
hmi_files=("$NEXTION_DIR"/*.HMI "$NEXTION_DIR"/*.hmi)

if [ ${#hmi_files[@]} -eq 0 ]; then
    echo "ERROR: no .HMI files found in $NEXTION_DIR"
    exit 1
fi

mkdir -p "$OUTPUT_BASE"

for hmi in "${hmi_files[@]}"; do
    file_name="$(basename "$hmi")"
    output_name="${file_name%.*}"
    output_name="${output_name// /_}"
    output_dir="$OUTPUT_BASE/$output_name"

    echo "Extracting: $file_name"
    rm -rf "$output_dir"
    mkdir -p "$output_dir"

    python3 "$SCRIPT_DIR/run_nextion2text.py" "$N2T_DIR/Nextion2Text.py" \
        -i "$hmi" \
        -o "$output_dir/text" \
        -e \
        -s \
        -j \
        -p visual

    if [ -d "$output_dir/text/json" ]; then
        mv "$output_dir/text/json" "$output_dir/json"
    elif [ -d "$output_dir/text/JSON" ]; then
        mv "$output_dir/text/JSON" "$output_dir/json"
    fi
done

echo "Done! Output: $OUTPUT_BASE"
