module water_steam_module_PH
  use water_eos_module
  implicit none

  private 
  
  real*8, private, parameter::  fmwh2o = 18.01534d0
  real*8, private, parameter::  epi_p = 1D-6, epi_h=1D-4
 
  public water_steam_prop_PH, wateos_PH, steameos_PH
  
  contains 

!*************************************************************

  Subroutine wateos_PH(p, h, T, dw, hx, energyscale, ierr)
  implicit none
  
  real*8 p,h, energyscale
  real*8 T,dw, hw
  integer ierr
  
  real*8 tx0, tx, dt, txd
  real*8 hx, hxd, dwmol
  integer itr
  
  tx0 = T ! get initial guess
  
  if (tx0 <1D0) then ! obtain initial guess
    tx0 = 100D0 ! or use constant cp 
  endif 

  itr= 0  
  tx=tx0
  dt=1D-7
  
  do 
   call wateos_noderiv(tx, p, dw, dwmol,hw, energyscale, ierr)
   hx= hw / fmwh2o
   if(abs((hx-h)/h) <= 1D-10) exit 
   
   txd = tx + dt
   call wateos_noderiv(txd, p, dw, dwmol,hw, energyscale, ierr)
   hxd= hw / fmwh2o
   tx = tx + ( h - hx)*dt / (hxd-hx)
!   print *, itr, tx, h,hx, hxd
   itr =itr+1
   if(itr>100)then
      print *,'Wateos_PH: Newton iteration does not converge within 100 times. STOP'
      print *, TX0, h, tx, hx-h
      stop
    endif
   enddo
  t=tx  
  End Subroutine wateos_PH


!*************************************************************

  Subroutine steameos_PH(p, h, T, dg, hx, energyscale, ierr)
  implicit none
  
  real*8 p,h, energyscale
  real*8 T, dg, dgmol, dgp,dgt, hg, hgp,hgt
  integer ierr
  
  real*8 tx0, tx, dt,txd
  real*8 hx, hxd
  integer itr
  
  tx0 = T ! get initial guess
  
  if (tx0 <1D0) then ! obtain initial guess
    tx0 = 100D0 ! or use constant cp 
  endif 

  itr= 0  
  tx=tx0
  dt=1D-7
  
  do 
   call steameos(tx, p, 0D0, dg, dgmol, dgp,dgt,hg,hgp,hgt,energyscale,ierr)
   hx = hg / fmwh2o
   if(abs((hx-h)/h) <= 1D-10) exit 
   
   txd = tx + dt
   call steameos(txd, p, 0D0, dg, dgmol, dgp,dgt,hg,hgp,hgt,energyscale,ierr)
   hxd=hg / fmwh2o
   tx = tx + ( h - hx)*dt / (hxd-hx)
   itr =itr+1
   if(itr>100)then
      print *,'steameos_PH: Newton iteration does not converge within 100 times. STOP'
      print *,TX0, h, tx, hx-h
      stop
    endif
   enddo
   t=tx  
  End Subroutine steameos_PH
!*************************************************************


 

!*************************************************************
 
!*************************************************************
! Subroutine
!-------------------------------------------------------------  
  subroutine water_steam_prop_PH(p, h, T_est,T, Sw, Den, dTdp, dTdh, dDendp, dDendh,  ierror)
  implicit none      
  
  real*8, intent(in)  :: p, h, T_est ! Pscal, KJ/Kg, C (initial guess)
  real*8, intent(out) :: T, Sw, Den, dTdp, dTdh, dDendp,dDendh
  integer,intent(out) :: ierror  ! ierr = 10  critical point   
 
  real*8 Ts 
  real*8 dw, dwmol,hw
  real*8 dg, dgmol, dgp,dgt,hg,hgp,hgt
  integer ierr
  integer :: iphase = 0
  real*8 delp, delh
  real*8 :: energyscale=1D-3 !KJ/Kg
  
  real*8 dw0, dw1, hw0,hw1, sw0, sw1
  real*8 dg0, dg1, hg0,hg1, h1
  real*8 t0,t1, d0,d1

 

! determine phase condition    
  call TSAT( p, T_est, Ts)
 
  call wateos_noderiv(Ts,p, dw, dwmol,hw, energyscale, ierr)
  call steameos(Ts, p, 0D0, dg, dgmol, dgp,dgt,hg,hgp,hgt,energyscale,ierr)
  hw=hw/fmwh2o; hg=hg/fmwh2o
  
  if(h >= hg)then
      iphase = 2
    else if(h <= hw)then
      iphase=1
    else
      iphase=3
  endif      
 
  print *,'h ph: ', p,t,h,hw,hg,iphase
  select case(iphase)
  case(1) ! water only
    T= Ts-10D0
    call wateos_PH(p, h, T, dw, hw, energyscale, ierr)
    Sw=1D0
    dg=0D0
    hg=0D0
    hw=hw/fmwh2o
    delp=epi_p
    delh=-epi_h
    Den=dw
  case(2)
    T= Ts+10D0
    call steameos_PH(p, h, T, dg, hg, energyscale, ierr)
    Sw=0D0
    dw=0D0
    hw=0D0
    hg=hg/fmwh2o
    delp=-epi_p
    delh=epi_h
    Den=Dg   
  case(3)
    T=Ts
    sw = 1D0/(1D0 -dw/dg*(hw-h)/(hg-h))
    Den= sw*dw+(1D0-sw)*dg 
    delp = epi_p
    delh = epi_h 
   end select
    

  select case (iphase)   
  case(1)
   t0= T
   call  wateos_noderiv( T, p, dw0, dwmol,hw0, energyscale, ierr) 
   call  wateos_PH(p+delp, h, T, dw1, hw1, energyscale, ierr) 
   t1=T
   dDendp=(dw1-dw0)/delp
   dTdp=(t1-t0)/delp
   call  wateos_PH(p, h+delh, T, dw1, hw1, energyscale, ierr)
   t1=T
   dDendh=(dw1-dw0)/delh
   dTdh=(t1-t0)/delh
  case(2)
   t0=T
   call steameos(T, p, 0D0, dg0, dgmol, dgp,dgt,hg0,hgp,hgt,energyscale,ierr)
   call steameos_PH(p+delp, h, T, dg1, hg1, energyscale, ierr)
   t1=T
   dDendp=(dg1-dg0)/delp
   dTdp=(t1-t0)/delp
   call  steameos_PH(p, h+delh, T, dw1, hw1, energyscale, ierr)
   t1=T
   dDendh=(dg1-dg0)/delh
   dTdh=(t1-t0)/delh
  case(3)
   t0=Ts
   dw0=dw; hw0=hw; sw0=sw
   dg0=dg; hg0=hg
   d0= Den
   call TSAT( p+delp, Ts, t1)
   dTdp=(t1-t0)/delp
   call wateos_noderiv(t1,p+delp, dw, dwmol,hw, energyscale, ierr)
   call steameos(t1, p+delp, 0D0, dg, dgmol, dgp,dgt,hg,hgp,hgt,energyscale,ierr)
   hw=hw/fmwh2o; hg=hg/fmwh2o
   sw1 = 1D0/(1D0 -dw/dg*(hw-h)/(hg-h))
   d1=sw1*dw+(1D0-sw1)*dg 
   dDendp=(d1-d0)/delp
   
   dTdh=0D0
   h1= h +delh
   sw1 = 1D0/(1D0 -dw0/dg0*(hw0-h1)/(hg0-h1))
   d1=sw1*dw0+(1D0-sw1)*dg0 
   dDendh=(d1-d0)/delh
   sw=sw0
  end select
  T=T0
              
 end subroutine water_steam_prop_PH
end module water_steam_module_PH 
 