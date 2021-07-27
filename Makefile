# render:
# 	Rscript --slave -e "pkgdown::build_site(devel = TRUE, install = TRUE, preview = FALSE)"

readme:
	Rscript --slave -e "rmarkdown::render('README.Rmd')"

install:
	R CMD INSTALL .

# copy:
# 	cp -r docs/* /srv/shiny-server/rpkg/bedpca/
# 	echo " - check http://172.24.80.251:443/rpkg/bedpca/"

.PHONY: install readme
