SUBROUTINE psat_new_rkp (T, P)

    real*8, intent(in) :: T !temperature
    real*8, intent(out) :: P  ! Saturation pressure

  
    real*8, save :: A(6),pc
    real*8 :: SC, Paln, tao
    
    
!   SAVE A
    DATA A/ &
      -7.85951783d0, 1.84408259d0,  -11.7866497d0, 22.6807411d0,&
      -15.9618719d0, 1.80122502d0/
    pc=22.064

    if (TC .LT. 1.d0 .OR. TC .GT. 500.d0) then
      ierr = 1
      return
    end if
    
    SC=(TC+273.15)/647.096
    tao = 1.D0-SC
    Paln= A(1) * tao + A(2)* tao ** 1.5 + A(3) * tao ** 3 &
         + A(4) * tao ** 3.5 +  A(5) * tao ** 4 &
         + A(6) * tao ** 7.5
    Paln = Paln / SC
    P = dexp(Paln) * pc

    P=P*1.d6
   ! ierr = 0

  end subroutine psat_new_rkp
