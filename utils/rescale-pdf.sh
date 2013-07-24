#!/usr/bin/env bash

TMPDIR=${TMPDIR:-/tmp}

TMPFILE=`mktemp -t $(basename ${2}).XXXXXX`

echo "Rescaling ${2} to ${1}"

echo "
\documentclass{book}
  \usepackage{pdfpages}
  \begin{document}
    \includepdf[width=${1},fitpaper]{${2}}
  \end{document}
" \
>${TMPFILE}.tex

pdflatex -output-directory ${TMPDIR} ${TMPFILE}.tex >/dev/null 2>/dev/null
cp ${TMPFILE}.pdf ${2}
rm -f ${TMPFILE}{,.{tex,aux,log,pdf}}
