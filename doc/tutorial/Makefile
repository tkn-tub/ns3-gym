TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
TGIF = tgif 
CONVERT = convert
CSS = --css-include=tutorial.css
SPLIT = --split section

TGIF_SOURCES = helpers.obj 
TGIF_EPS = ${TGIF_SOURCES:.obj=.eps}

all:  images html split-html pdf

images:
	cd figures/; $(TGIF) -print -eps $(TGIF_SOURCES) 
	cd figures/; $(foreach FILE,$(TGIF_EPS),$(CONVERT) $(FILE) ${FILE:.eps=.png};)
	cd figures/; $(foreach FILE,$(TGIF_EPS),$(EPSTOPDF) $(FILE);)

html: images
	$(TEXI2HTML) ${CSS} tutorial.texi

split-html: images
	$(TEXI2HTML) ${CSS} ${SPLIT} tutorial.texi

pdf: images
	$(TEXI2PDF) tutorial.texi

clean:
	rm -rf tutorial.aux tutorial.cp tutorial.cps tutorial.fn tutorial.ky tutorial.pg tutorial.tp tutorial.vr tutorial.toc tutorial.log tutorial.pdf tutorial.html tutorial/ 
