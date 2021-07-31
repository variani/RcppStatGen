library(bench)
library(ggplot2)

fun_bench <- function(n = 1e4, p = 1e3, times = 3)
{
  X <- matrix(rnorm(n*p), n, p)
  mark(scale(X), eigen_scale_naive(X), eigen_scale(X), check = FALSE, relative = TRUE)
}

fun_bench_inplace <- function(n = 1e4, p = 1e3, times = 3)
{
  X <- matrix(rnorm(n*p), n, p)
  mark(eigen_scale(X), eigen_scale_inplace(X), check = FALSE)
}

# native R function scale requires 10x more memory than eigen implementations
# eigen implementations are also several times faster
b <- fun_bench()

# in-place scaling function requires less memory by a factor of ~#rows = n = 1e4
b_inplace <- fun_bench_inplace()

# p <- autoplot(b)
# ggsave("tmp.png")




