all: full

clean:
	latexmk -C

%.pdf: %.tex
	latexmk --quiet --shell-escape -pdf $<
	latexmk -c

full: titlepage.pdf main.pdf
	pdfjam titlepage.pdf 1 main.pdf 2-12 -o paper.pdf
