---
header-includes: |
  \usepackage{libertine}
  \renewcommand{\ttdefault}{cmtt}
...

<!---

  Convert this file with:

  pandoc -V urlcolor=blue fonts.md -o fonts.pdf

--->

# About internal fonts

Internal vector fonts have been decoded from original Borland `CHR`
fonts and turned to arrays of coordinates; that is, glyphs.

I'm not aware of the legal status of the `CHR` fonts that accompanied
Borland Turbo C and other compilers. Anyhow, even assuming that they
are still copyrighted under U.S. law, their glyphs are not; please see
[this Wikipedia article](https://en.wikipedia.org/wiki/Intellectual_property_protection_of_typefaces)
for details. It is therefore legal to incorporate the glyphs of `CHR`
fonts in `SDL_bgi`.

Fonts were taken from Borland Turbo C++ 3, available
[here](https://archive.org/details/turboc3), and converted using the
ancillary program `tmp/chr_decoder.c`.
