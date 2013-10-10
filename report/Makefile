TEX = platex
DVIPDF = dvipdfmx

DVIFILES = report.dvi

report.pdf: $(DVIFILES)
	$(DVIPDF) -o $@ $(DVIFILES)

.tex.dvi: report.tex purpose.tex theory.tex setup.tex analyze.tex study.tex
	$(TEX) $< && $(TEX) $<

clean:
	rm *.dvi *.log *.aux
