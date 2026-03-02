#!/usr/bin/env bash

#set -euo pipefail

ORIGIN_DIR="${1:-.}"
OUTPUT_FILE="${2:-compiled_output.txt}"

ORIGIN_DIR="$(realpath "$ORIGIN_DIR")"
> "$OUTPUT_FILE"

echo "Compilando archivos desde: $ORIGIN_DIR"
echo "Salida en: $OUTPUT_FILE"
echo

# ===============================
# Extensiones permitidas
# ===============================
EXTENSIONS=(
    "*.txt"

    "*.md"
    "*.dox"
    "makefile"
    "Makefile"
    "doxyfile"
    "Doxyfile"

    "*.c"
    "*.h"
    "*.sh"
    "*.py"
    "*.js"
    "*.html"
    "*.css"
    ".njk"
)

# ===============================
# Directorios a excluir
# ===============================
EXCLUDE_DIRECTORIES=(
    ".git"
    ".venv"
    "__pycache__"

    "migrations"

    "result"

    "img"
    "docs"
)

# ===============================
# Archivos a excluir
# ===============================
EXCLUDE_FILES=(
    "__init__.py"

    "manage.py"
    "asgi.py"
    "settings.py"
    "urls.py"
    "wsgi.py"
    "requirements.txt"

    "*LICENSE*"
    "*LICESE*"
    "README*"
    "CONTRIBUTING*"
    "ROADMAP*"

    "nt*"
)

# ===============================
# Construcción dinámica
# ===============================

# ... (Tus arreglos se mantienen igual) ...

# 1. Construir Expresión de Directorios (PRUNE)
PRUNE_EXPR=()
for dir in "${EXCLUDE_DIRECTORIES[@]}"; do
    PRUNE_EXPR+=( -name "$dir" -o )
done
unset 'PRUNE_EXPR[${#PRUNE_EXPR[@]}-1]'

# 2. Construir Expresión de Extensiones (INCLUSIÓN)
FIND_EXT_EXPR=()
for ext in "${EXTENSIONS[@]}"; do
    FIND_EXT_EXPR+=( -name "$ext" -o )
done
unset 'FIND_EXT_EXPR[${#FIND_EXT_EXPR[@]}-1]'

# 3. Construir Expresión de Archivos (EXCLUSIÓN)
# NOTA: Importante mantenerlos como una lista de condiciones AND
EXFILES_EXPR=()
for pattern in "${EXCLUDE_FILES[@]}"; do
    EXFILES_EXPR+=( ! -name "$pattern" )
done

# ===============================
# GENERAR LISTA FINAL FILTRADA
# ===============================
FILTERED_FILES=()
while IFS= read -r -d '' file; do
    if file "$file" | grep -qi text; then
        FILTERED_FILES+=("$file")
    fi
done < <(
    find "$ORIGIN_DIR" \
        \( -type d \( "${PRUNE_EXPR[@]}" \) -prune \) -o \
        \( -type f \( "${FIND_EXT_EXPR[@]}" \) "${EXFILES_EXPR[@]}" -print0 \) \
    | sort -z
)
TOTAL_FILES=${#FILTERED_FILES[@]}
TOTAL_BYTES=0
for file in "${FILTERED_FILES[@]}"; do
    size=$(stat -c%s "$file")
    TOTAL_BYTES=$((TOTAL_BYTES + size))
done

# ===============================
# EJECUCIÓN DEL FIND (Lógica Corregida)
# ===============================
{
    echo "===== PROJECT SNAPSHOT ====="
    echo "ROOT: $(basename "$ORIGIN_DIR")"
    echo "TOTAL_FILES: $TOTAL_FILES"
    echo "TOTAL_BYTES: $TOTAL_BYTES"
    echo "GENERATED_AT: $(date -u +"%Y-%m-%dT%H:%M:%SZ")"
    echo
    echo "===== DIRECTORY INDEX ====="

    for file in "${FILTERED_FILES[@]}"; do
        realpath --relative-to="$ORIGIN_DIR" "$file"
    done

    echo
} > "$OUTPUT_FILE"
for file in "${FILTERED_FILES[@]}"; do
    REL_PATH="$(realpath --relative-to="$ORIGIN_DIR" "$file")"
    FILE_SIZE=$(stat -c%s "$file")

    {
        echo "===== FILE BEGIN ====="
        echo "PATH: $REL_PATH"
        echo "SIZE: $FILE_SIZE bytes"
        echo "-----"
        cat "$file"
        echo
        echo "===== FILE END ====="
        echo
    } >> "$OUTPUT_FILE"
done
echo "✔ Compilación finalizada."
