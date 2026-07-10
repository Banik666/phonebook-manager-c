# Phonebook Manager (C)

A password-protected, console-based contact management system written in C for Windows.

This started as a first-year university project and has since been completed and
extended with proper file handling, input validation, and a few extra features.

## Features

- 🔒 **Password-protected access** — set your own password on first run (masked input, 3 login attempts)
- ➕ **Add** contacts (name, address, email, phone)
- 🔍 **Search** contacts by name (partial, case-insensitive match)
- ✏️ **Modify** existing contacts (keep old value by pressing Enter)
- 🗑️ **Delete** contacts (with confirmation prompt)
- 📋 **List** all contacts in a formatted table
- 🔤 **Sort** contacts alphabetically by name
- 📤 **Export** all contacts to a CSV file
- Colored console output for a nicer terminal experience

## Tech notes

- Records are stored as fixed-size binary structs in `records.dat`, which is
  faster and more robust than parsing a delimited text file.
- Uses `<conio.h>` and `<windows.h>` for masked password entry and colored
  console text, so this project targets **Windows**.
- Input is read with `fgets()` instead of the unsafe `gets()`, and `scanf`
  buffers are cleared safely instead of relying on `fflush(stdin)` (which is
  undefined behavior for input streams).

## Building

Using MinGW / GCC:

```bash
gcc main.c -o phonebook.exe
```

Or open `main.c` in Code::Blocks / Dev-C++ and build/run directly.

## Running

```bash
phonebook.exe
```

On the very first run you'll be asked to create a password (stored in
`password.dat`). After that, you'll be prompted to log in each time you
start the program.

## Files generated at runtime

| File                  | Purpose                              |
|------------------------|---------------------------------------|
| `password.dat`         | Stores your login password            |
| `records.dat`          | Binary storage for all contacts       |
| `records_export.csv`   | Created when you use the export option|

These are generated locally and are excluded from version control (see `.gitignore`).

## Possible future improvements

- Hash the stored password instead of saving it in plain text
- Cross-platform support (replace `conio.h`/`windows.h` calls)
- Duplicate-entry detection on add
- Search by phone number or email in addition to name

## Author's note

Originally written during my first year of university as an early C
project. Revisited and completed as a portfolio piece.
