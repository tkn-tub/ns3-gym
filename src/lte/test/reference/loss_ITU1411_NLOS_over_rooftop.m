function g = loss_ITU1411_NLOS_over_rooftop (d, hb, hm, hr, f, l, b, st_w, phi, big)

  %%
  %% function g = loss_ITU1411_LOS(d, c)
  %%
  %% returns the loss at d meters for f frequency and mobile height m and
  %% base station height of hb
  
  assert(isscalar(f));
  assert(f > 0);


  lambda = 300000000.0 / f;
  
  Dhb = hb - hr;
  ds = (lambda.*(d).^2)/Dhb^2;

  if (l>ds)
    if (big==1)
      kf = 1.5*((f/925)-1);
    else
      kf = 0.7*((f/925)-1);
    endif
    if (hb>hr)
      kd = 18;
      ka = 54;
      Lbsh = -18*log10(1+Dhb);
    else
      kd = 18 - 15*(Dhb/hr);
      Lbsh = 0;
      if (d>=500)
        ka = 54-0.8*Dhb;
      else
        ka = 54-1.6*Dhb;
      endif
    endif
    Lmsd = Lbsh + ka + kd.*log10(d./1000) + kf*log10(f) - 9*log10(b);
  else
    theta = atan (Dhb /b);
    rho = sqrt(Dhb^2 + b^2);
    if (hb-hr<1)
       Qm = b./d;
    elseif (hb>hr)
       Qm = 2.35*((dhb./d)*sqrt(b/lamda))^0.9;
    else
       Qm = (b/2*pi.*d)*sqrt(lambda/rho)*((1/theta)-(1/(2*pi+theta)));
    endif
    
    Lmsd = -10*log(Qm^2);
  endif
  Dhm = hr-hm;
  if (phi<35)
    Lori = -10+(0.354*phi);
  elseif (phi<55)
    Lori = 2.5+0.075*(phi-35);
  else
    Lori = 4- 0.114*(phi-55);
  endif
  
  Lrts = -8.2 -10*log10(st_w) + 10*log10(f) + 20*log10(Dhm) + Lori;

  Lbf = 32.4 +20*log10(d/1000) + 20*log10(f);
    

  g = zeros(size(d));
  if (Lrts+Lmsd>0)
    L = Lbf + Lrts + Lmsd;
  else
    L0 Lbf
  endif
  g(find(d > 0)) = L;

  g(find(d <= 0)) = 1;
  