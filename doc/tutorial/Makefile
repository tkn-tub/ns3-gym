TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
DIA = dia
CONVERT = convert
CSS = --css-include=tutorial.css
SPLIT = --split section

DIA_SOURCES = pp.dia dumbbell.dia star.dia 

DIA_EPS = ${DIA_SOURCES:.dia=.eps}
DIA_PNG = ${DIA_SOURCES:.dia=.png}
DIA_PDF = ${DIA_SOURCES:.dia=.pdf}

all:  html split-html pdf

# Note:  tgif requires a valid x display to convert from .obj to .png.  
#        If running this makefile on a remote console, the X virtual frame
#        buffer may be needed (xorg-x11-server-Xvfb) to provide a "fake"
#        display
images:
	cd figures/; $(DIA) -t png $(DIA_SOURCES) 
	cd figures/; $(DIA) -t eps $(DIA_SOURCES) 
	cd figures/; $(foreach FILE,$(DIA_EPS),$(EPSTOPDF) $(FILE);)

html: 
	$(TEXI2HTML) ${CSS} tutorial.texi

split-html: 
	$(TEXI2HTML) ${CSS} ${SPLIT} tutorial.texi

pdf:
	$(TEXI2PDF) tutorial.texi

figures-clean:
	cd figures/; rm -rf $(DIA_EPS); rm -rf $(DIA_PNG); rm -rf $(DIA_PDF)

clean: # figures-clean
	rm -rf tutorial.aux tutorial.cp tutorial.cps tutorial.fn tutorial.ky tutorial.pg tutorial.tp tutorial.vr tutorial.toc tutorial.log tutorial.pdf tutorial.html tutorial/ 
