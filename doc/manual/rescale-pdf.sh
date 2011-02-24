#!/usr/bin/env bash

TMPFILE=`mktemp`

echo "\documentclass{book}
\usepackage{pdfpages}
\begin{document}
\includepdf[width=${1},fitpaper]{${2}}
\end{document}" >${TMPFILE}.tex
pdflatex -output-directory /tmp ${TMPFILE}.tex >/dev/null 2>/dev/null
cp ${TMPFILE}.pdf ${3}

