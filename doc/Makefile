# Makefile for documentation

# PDF engine for Pandoc: tested with 
# pdflatex, pdfroff, wkhtmltopdf, and weasyprint
PDFENGINE = pdflatex
OPTS = -V urlcolor=blue -V toccolor=red --pdf-engine=$(PDFENGINE)

PDFDOCS = compatibility.pdf fonts.pdf functions.pdf howto_AppImage.pdf \
          howto_CodeBlocks.pdf howto_Dev-Cpp.pdf howto_Emscripten.pdf \
	  howto_Python.pdf using.pdf

HTMLDOCS  = $(PDFDOCS:.pdf=.html)

all: $(PDFDOCS) $(HTMLDOCS)
	echo $(HTMLDOCS)

compatibility.pdf: compatibility.md
	pandoc $(OPTS) compatibility.md \
	-o compatibility.pdf

compatibility.html: compatibility.md
	pandoc compatibility.md \
	-o compatibility.html

fonts.pdf: fonts.md
	pandoc $(OPTS) fonts.md -o fonts.pdf

fonts.html: fonts.md
	pandoc fonts.md -o fonts.html

functions.pdf: functions.md
	pandoc $(OPTS) functions.md -o functions.pdf

functions.html: functions.md
	pandoc functions.md -o functions.html

howto_AppImage.pdf: howto_AppImage.md
	pandoc $(OPTS) howto_AppImage.md \
	-o howto_AppImage.pdf

howto_AppImage.html: howto_AppImage.md
	pandoc howto_AppImage.md \
	-o howto_AppImage.html

howto_CodeBlocks.pdf: howto_CodeBlocks.md
	pandoc $(OPTS) howto_CodeBlocks.md \
	-o howto_CodeBlocks.pdf

howto_CodeBlocks.html: howto_CodeBlocks.md
	pandoc howto_CodeBlocks.md \
	-o howto_CodeBlocks.html

howto_Dev-Cpp.pdf: howto_Dev-Cpp.md
	pandoc $(OPTS) howto_Dev-Cpp.md \
	-o howto_Dev-Cpp.pdf

howto_Dev-Cpp.html: howto_Dev-Cpp.md
	pandoc howto_Dev-Cpp.md \
	-o howto_Dev-Cpp.html

howto_Emscripten.pdf: howto_Emscripten.md
	pandoc $(OPTS) howto_Emscripten.md \
	-o howto_Emscripten.pdf

howto_Emscripten.html: howto_Emscripten.md
	pandoc howto_Emscripten.md \
	-o howto_Emscripten.html

howto_Python.pdf: howto_Python.md
	pandoc $(OPTS) howto_Python.md \
	-o howto_Python.pdf

howto_Python.html: howto_Python.md
	pandoc howto_Python.md \
	-o howto_Python.html

using.pdf: using.md
	pandoc $(OPTS) --toc \
	using.md -o using.pdf

using.html: using.md
	pandoc --toc \
	using.md -o using.html

# --- end of Makefile
