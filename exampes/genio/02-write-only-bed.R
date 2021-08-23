library(tidyverse)
library(genio)

## parameters
n <- 100
p <- 20

gen <- rbinom(n*(p/2), 2, 0.5)
X <- matrix(gen, n, p/2) # row = variant, column = sample
Xt <- t(X)
write_bed("ex", t(X))

## chunk 2: p/2 variants in Chr 2
gen <- rbinom(n*(p/2), 2, 0.5)
X <- matrix(gen, n, p/2) # row = variant, column = sample
Xt <- t(X)
write_bed("ex", t(X), append = TRUE) 

## read back
X <- read_bed("ex", n_ind = n, m_loci = p)
stopifnot(nrow(X) == p)
stopifnot(ncol(X) == n)
