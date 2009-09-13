TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
DIA = dia
CONVERT = convert
CSS = --css-include=testing.css
SPLIT = --split section

FIGURES = figures
VPATH = $(FIGURES)

IMAGES_EPS = \

IMAGES_PNG = ${IMAGES_EPS:.eps=.png}
IMAGES_PDF = ${IMAGES_EPS:.eps=.pdf}

IMAGES = $(IMAGES_EPS) $(IMAGES_PNG) $(IMAGES_PDF)

CHAPTERS = \
	testing.texi \
	overview.texi \
	propagation-loss.texi \

%.eps : %.dia; $(DIA) -t eps $< -e $@
%.png : %.dia; $(DIA) -t png $< -e $@
%.pdf : %.eps; $(EPSTOPDF) $< -o=$@

all:  $(IMAGES) testing.pdf testing.html testing/testing.html

testing.pdf: $(IMAGES) $(CHAPTERS)
	$(TEXI2PDF) testing.texi

testing.html: $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} testing.texi

testing/testing.html: $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} ${SPLIT} testing.texi

figures-clean:
	rm -rf $(IMAGES)

clean: 	figures-clean
	rm -rf testing.aux testing.cp testing.cps testing.fn testing.ky testing.pg 
	rm -rf testing.tp testing.vr testing.toc testing.log testing.pdf testing.html testing/ 
