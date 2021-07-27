# library(RcppStatGen)
devtools::load_all()

n <- 1e4
p <- 1e3
X <- matrix(rnorm(n*p), n, p)
t1 <- system.time(out1 <- scale(X))
t2 <- system.time(out2 <- eigen_scale(X))

X <- iris[, -5] %>% head %>% as.matrix
eigen_scale(X) %>% str


