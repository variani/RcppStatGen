library(bench)
library(ggplot2)

fun_bench <- function(n = 1e4, p = 1e3, times = 3)
{
  X <- matrix(rnorm(n*p), n, p)
  mark(scale(X), eigen_scale_naive(X), eigen_scale(X), check = FALSE, relative = TRUE)
}

b <- fun_bench()
p <- autoplot(b)
ggsave("tmp.png")




