TEX = platex
DVIPDF = dvipdfmx

DVIFILES = report.dvi

report.pdf: $(DVIFILES)
	$(DVIPDF) -o $@ $(DVIFILES)

.tex.dvi: report.tex
	$(TEX) $< && $(TEX) $<

clean:
	rm *.dvi *.log *.aux report.pdf
