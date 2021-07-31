
<!-- README.md is generated from README.Rmd. Please edit that file -->

# RcppStatGen

<!-- badges: start -->

<!-- badges: end -->

RcppStatGen is an R/RcppEigen package with code snippets for Statistical
Genetics tasks.

## Installation

You can install the development version from
[GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("variani/RcppStatGen")
```

# Examples

``` r
library(RcppStatGen)
library(magrittr) # for %>%

eigen_version()
```

Basic example of scaling a numeric matrix.

``` r
iris[, -5]  %>% head %>% as.matrix %>% eigen_scale
#>            [,1]       [,2]       [,3]       [,4]
#> [1,]  0.5206576  0.3401105 -0.3627381 -0.4082483
#> [2,] -0.1735525 -1.1175060 -0.3627381 -0.4082483
#> [3,] -0.8677627 -0.5344594 -1.0882144 -0.4082483
#> [4,] -1.2148677 -0.8259827  0.3627381 -0.4082483
#> [5,]  0.1735525  0.6316338 -0.3627381 -0.4082483
#> [6,]  1.5619728  1.5062037  1.8136906  2.0412415
```

Example of scaling a matrix with missing data.

``` r
matrix(1:9, 3, 3) %>% eigen_scale_naive
#>      [,1] [,2] [,3]
#> [1,]   -1   -1   -1
#> [2,]    0    0    0
#> [3,]    1    1    1
matrix(c(NA, 2:9), 3, 3) %>% eigen_scale_naive
#>      [,1] [,2] [,3]
#> [1,]   NA   -1   -1
#> [2,]   NA    0    0
#> [3,]   NA    1    1
matrix(c(NA, 2:9), 3, 3) %>% eigen_scale
#>            [,1] [,2] [,3]
#> [1,]         NA   -1   -1
#> [2,] -0.7071068    0    0
#> [3,]  0.7071068    1    1
```

# Benchmarks

## Scale matrix

The standard R function `scale` requires ~10x more RAM than its Eigen
implementations. Eigen implementations are also several times faster.

``` r
library(bench)

fun_bench <- function(n = 1e4, p = 1e3)
{
  X <- matrix(rnorm(n*p), n, p)
  bench::mark(scale(X), eigen_scale_naive(X), eigen_scale(X), check = FALSE, relative = TRUE)
}
fun_bench()
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 3 x 6
#>   expression             min median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>           <dbl>  <dbl>     <dbl>     <dbl>    <dbl>
#> 1 scale(X)              3.98   3.93      1         10.5     5.34
#> 2 eigen_scale_naive(X)  1      1         3.90       1       1.16
#> 3 eigen_scale(X)        1.16   1.17      3.37       1       1
```

## Scale matrix in-place

Scaling matrix in-place requires almost no extra RAM (just needed to
store means/sds for each column). Thus, the memory footprint is reduced
by a factor of n = 1e4 (the number of rows).

``` r
fun_bench_inplace<- function(n = 1e4, p = 1e3)
{
  X <- matrix(rnorm(n*p), n, p)
  bench::mark(eigen_scale(X), eigen_scale_inplace(X), check = FALSE)
}
fun_bench()
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 3 x 6
#>   expression             min median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>           <dbl>  <dbl>     <dbl>     <dbl>    <dbl>
#> 1 scale(X)              3.91   3.89      1         10.5     5.39
#> 2 eigen_scale_naive(X)  1      1         3.71       1       1   
#> 3 eigen_scale(X)        1.06   1.18      3.40       1       1.22
```
