REPORT = rapport.pdf

.PHONY: all clean

all: rapport.tex
	@echo Generating PDF file...
	@latex -halt-on-error -output-format=pdf rapport.tex
	@rm -f rapport.aux rapport.log
	@echo $@ generated.

%.pdf: %.ps
	epstopdf $<

%.ps: %.graph
	gnuplot $<

clean:
	@echo Cleaning up...
	-@rm -f $(REPORT) 2> /dev/null
	-@rm -f $(GRAPHS)
	@echo all clean
