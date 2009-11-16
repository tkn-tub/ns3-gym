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
	background.texi \
	testing-framework.texi \
	how-to-write-tests.texi \
	validation.texi \
	random-variables.texi \
	propagation-loss.texi \
	references.texi \

%.eps : %.dia; $(DIA) -t eps $< -e $@
%.png : %.dia; $(DIA) -t png $< -e $@
%.pdf : %.eps; $(EPSTOPDF) $< -o=$@

all:  $(IMAGES) version testing.pdf testing.html testing/testing.html

testing.pdf: version $(IMAGES) $(CHAPTERS)
	$(TEXI2PDF) testing.texi

testing.html: version $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} testing.texi

testing/testing.html: version $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} ${SPLIT} --output testing testing.texi

figures-clean:
	rm -rf $(IMAGES)

version:
	echo -n "ns-" > VERSION-PREFIX; cat VERSION-PREFIX ../../VERSION > VERSION; rm -rf VERSION-PREFIX

clean: 	figures-clean
	rm -rf testing.aux testing.cp testing.cps testing.fn testing.ky testing.pg 
	rm -rf testing.tp testing.vr testing.toc testing.log testing.pdf testing.html testing/ VERSION missfont.log
