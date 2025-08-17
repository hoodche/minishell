# Minishell 🐚

## 📖 Overview  
Minishell is a simplified implementation of a Unix shell, written in C.  
The goal of the project is to build a functional command-line interpreter that mimics some of the core behaviors of `bash`.  
---

## 🚀 Features  

- **Prompt display** waiting for user input  
- **Command history** (via `readline`)  
- **Executable search & execution** (using `$PATH` or relative/absolute paths)  
- **Signal handling**  
  - `Ctrl-C` → interrupts and shows a new prompt  
  - `Ctrl-D` → exits the shell  
  - `Ctrl-\` → does nothing (like in bash)  
- **Quoting rules**  
  - `'` prevents interpretation of meta-characters  
  - `"` prevents interpretation except for `$`  
- **Redirections**  
  - `<` : input redirection  
  - `>` : output redirection (overwrite)  
  - `>>` : output redirection (append)  
  - `<<` : heredoc with delimiter  
- **Pipes (`|`)** connecting commands in a pipeline  
- **Environment variable expansion** (`$VAR`, `$?`)  
- **Built-in commands**:  
  - `echo` (with `-n`)  
  - `cd` (relative/absolute paths)  
  - `pwd`  
  - `export`  
  - `unset`  
  - `env`  
  - `exit`  

---

## 🛠️ Compilation  

- `make` → compiles the project  
- ./minishell  → executes the minishell 

## 📺 Demo

![Demo](demo.gif)

---

## 👤 Authors

 Ignacio Castillo Franco y Grace Taza Castañeda (🐙 Gracy)

