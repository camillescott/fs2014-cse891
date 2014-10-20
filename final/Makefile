all: abstract.pdf

abstract.pdf: abstract.md
	pandoc -r markdown+yaml_metadata_block -s -S --latex-engine=pdflatex --template=latex.tpl abstract.md -o abstract.pdf
