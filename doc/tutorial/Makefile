TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
TGIF = tgif 
CSS = --css-include=tutorial.css
SPLIT = --split section

TGIF_SOURCES = helpers.obj
TGIF_EPS = ${TGIF_SOURCES:.obj=.eps}

all:  images html split-html pdf

images:
	cd figures/; $(TGIF) -print -png $(TGIF_SOURCES) 
	cd figures/; $(TGIF) -print -eps $(TGIF_SOURCES) 
	cd figures/; $(EPSTOPDF) $(TGIF_EPS)

html: images
	$(TEXI2HTML) ${CSS} tutorial.texi

split-html: images
	$(TEXI2HTML) ${CSS} ${SPLIT} tutorial.texi

pdf: images
	$(TEXI2PDF) tutorial.texi
