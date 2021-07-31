
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
#> 1 scale(X)              3.99   3.99      1         10.5     5.29
#> 2 eigen_scale_naive(X)  1      1         3.95       1       1.16
#> 3 eigen_scale(X)        1.15   1.18      3.40       1       1
```

## Scale matrix in-place

Scaling matrix in-place requires almost no extra RAM (just needed to
store means/sds for each column). Thus, the memory footprint is reduced
by a factor of n = 1e4 (the number of rows).

This exercise was motivated by this
[SO](https://stackoverflow.com/questions/27935124/is-it-okay-to-modify-a-mapped-matrix-in-rcppeigen)
question. In general, one expects from scaling function to take one
input matrix of raw data (to keep untouched) and create another new
matrix of per-column scaled data.

``` r
fun_bench_inplace<- function(n = 1e4, p = 1e3)
{
  X <- matrix(rnorm(n*p), n, p)
  bench::mark(eigen_scale(X), eigen_scale_inplace(X), check = FALSE)
}
fun_bench_inplace()
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 2 x 6
#>   expression                  min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>             <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 eigen_scale(X)            227ms    254ms      3.94    76.3MB     3.94
#> 2 eigen_scale_inplace(X)   63.2ms   63.6ms     15.7     5.02KB     0
```

The extra RAM used by `eigen_scale` function (compared to
`eigen_scale_inplace`) is equal to the size of matrix `X` (76Mb).

``` r
n <- 1e4
p <- 1e3
X <- matrix(rnorm(n*p), n, p)
object.size(X) %>% print(units = "auto")
#> 76.3 Mb
rm(X)
```
