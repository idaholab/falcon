subroutine dpoly_val_horner ( n, c, x, cx )

!*****************************************************************************80
!
!! DPOLY_VAL_HORNER evaluates a polynomial using Horner's method.
!
!  Modified:
!
!    08 August 1999
!
!  Author:
!
!    John Burkardt
!
!  Parameters:
!
!    Input, integer N, the dimension of C.
!
!    Input, real ( kind = 8 ) C(0:N), the polynomial coefficients.
!    C(I) is the coefficient of X**I.
!
!    Input, real ( kind = 8 ) X, the point at which the polynomial
!    is to be evaluated.
!
!    Output, real ( kind = 8 ) CX, the value of the polynomial at X.
!
  implicit none

  integer n

  real ( kind = 8 ) c(0:n)
  real ( kind = 8 ) cx
  integer i
  real ( kind = 8 ) x

  cx = c(n)
  do i = n - 1, 0, -1
    cx = cx * x + c(i)
  end do

  return
end
