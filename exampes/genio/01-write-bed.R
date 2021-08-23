library(tidyverse)
library(genio)

## parameters
n <- 100
p <- 20

# fam: The tibble or data.frame containing individual information.
#      It must contain ‘n’ rows and these columns: ‘fam’, ‘id’,
#      ‘pat’, ‘mat’, ‘sex’, ‘pheno’. If ‘NULL’ (default), it will be
#      quietly auto-generated.
fam <- tibble(id = paste0("ID", seq(n)), pat = 0, mat = 0, sex = 0, pheno = 0) %>%
  mutate(fam = id)

## chunk 1: p/2 variants in Chr 1
# bim: The tibble or data.frame containing locus information. It
#      must contain ‘m’ rows and these columns: ‘chr’, ‘id’, ‘posg’,
#      ‘pos’, ‘ref’, ‘alt’. If ‘NULL’ (default), it will be quietly
#      auto-generated.
bim <- tibble(chr = 1, id = paste0("snp", seq(p/2)), posg = 0, pos = 0,
  ref = "0", alt = "1")
gen <- rbinom(n*(p/2), 2, 0.5)
X <- matrix(gen, n, p/2) # row = variant, column = sample
Xt <- t(X)
write_plink("ex", t(X), bim = bim, fam = fam) 

## chunk 2: p/2 variants in Chr 2
bim <- tibble(chr = 2, id = paste0("snp", seq(p/2)), posg = 0, pos = 0,
  ref = "0", alt = "1")
gen <- rbinom(n*(p/2), 2, 0.5)
X <- matrix(gen, n, p/2) # row = variant, column = sample
Xt <- t(X)
write_plink("ex", t(X), bim = bim, fam = fam, append = TRUE) 


## read back
out <- read_plink("ex")
stopifnot(nrow(out$X) == p)
stopifnot(ncol(out$X) == n)
