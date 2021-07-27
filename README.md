
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

## Example

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
#> 1 scale(X)              6.32   5.98      1         10.5     2.48
#> 2 eigen_scale_naive(X)  1      1         6.04       1       1   
#> 3 eigen_scale(X)        1.26   1.22      4.90       1       1.01
```
