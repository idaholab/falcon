subroutine d_gtle_range_p ( x, a, b, ok )

!***********************************************************************
!
!! D_GTLE_RANGE checks that A < X <= B.
!
!  Modified:
!
!    31 January 2002
!
!  Author:
!
!    John Burkardt
!
!  Parameters:
!
!    Input, real ( kind = 8 ) X, the value to be checked.
!
!    Input, real ( kind = 8 ) A, B, the left and right endpoints.
!
!    Output, logical OK, is TRUE if A < X <= B.
!
  implicit none

  real ( kind = 8 ) a
  real ( kind = 8 ) b
  logical ok
  real ( kind = 8 ) x

  if ( x <= a ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) '  The input pressure is out of range-low.'
    write ( *, '(a,g14.6)' ) '  The minimum legal value is ', a
    write ( *, '(a,g14.6)' ) '  The input value is         ', x
    write ( *, '(a)' ) '  Program Terminated.'
    ok = .false.
  else if ( b < x ) then
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) '  The input pressure is out of range-high.'
    write ( *, '(a,g14.6)' ) '  The maximum legal value is ', b
    write ( *, '(a,g14.6)' ) '  The input value is         ', x
    write ( *, '(a)' ) '  Program Terminated.'
    ok = .false.
  else
    ok = .true.
  end if

  return
end
