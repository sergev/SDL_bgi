# README.md

Documentation in this directory:

- Compatibility (`compatibility.md`)

- About internal fonts (`fonts.md`)

- Functions (`functions.md`)

- How to compile SDL_bgi programs with Code-Blocks (`howto_CodeBlocks.md`)

- How to compile SDL_bgi programs with Dev-C++ (`howto_Dev-Cpp.md`)

- How to turn an `SDL_bgi` program to an AppImage (`making_AppImages.md`)

- How to use `SDL_bgi` in Python programs (`howto_Python.md`)

- SDL bgi Quick Reference (`sdl_bgi-quickref.tex`)

- Turtle Graphics Quick Reference (`turtlegraphics.tex`)

- Using SDL_bgi (`using.md`)

- graphics.3.gz (GNU/Linux manpage)

All `.md` (Markdown) files have been converted to PDF using `pandoc`
and the default PDF engine (pdflatex). If you want to modify these
files and convert them to PDF without a LaTeX installation, a good
alternative is `pdfroff`, provided by the `groff` package:

```
pandoc --pdf-engine=pdfroff file.md -o file.pdf
```

Documents written in LaTeX are typeset with `pdflatex`.

