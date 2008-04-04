TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
TGIF = tgif 
CSS = --css-include=tutorial.css

TGIF_SOURCES = helpers.obj
TGIF_EPS = ${TGIF_SOURCES:.obj=.eps}

all:  images tutorial.html tutorial.pdf

images:
	cd figures/; $(TGIF) -print -png $(TGIF_SOURCES) 
	cd figures/; $(TGIF) -print -eps $(TGIF_SOURCES) 
	cd figures/; $(EPSTOPDF) $(TGIF_EPS)

tutorial.html:
	$(TEXI2HTML) ${CSS} tutorial.texi

tutorial.pdf:
	$(TEXI2PDF) tutorial.texi
