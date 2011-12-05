function g = loss_ITU1411_NLOS_street_canyons (d, f, w1, w2, x1, x2, alpha)

  %%
  %% function g = loss_ITU1411_NLOS_street_canyons(d, c)
  %%
  %% returns the loss at d meters for f frequency and mobile height m and
  %% base station height of hb
  
  assert(isscalar(f));
  assert(f > 0);


  lambda = 300000000.0 / f;


  if (alpha <= 0.33)
    f_alpha = -41+110*alpha;
  elseif (alpha <=0.42)
    f_alpha = -13.94+28*alpha;
  elseif (alpha <=0.71)
    f_alpha = -5.33+7.51*alpha;
  else
    f_alpha = 0;
  endif

  Lr = -20*log10(x1+x2) + (x1*x2*(f_alpha/(w1*w2))) - 20*log10((4*pi)/lambda);

  Da = -1*(40/(2*pi))*(atan(x2/w2)+atan(x1/w1)-(pi/2));

  Ld = -10*log10(x2*x1*(x1+x2))+ 2*Da + 0.1*(90-(alpha*(180/pi))) -20*log10((4*pi)/lambda);

  
  
  g(find(d > 0)) = -10*log10(10^(Lr/10)+10^(Ld/10));

  g(find(d <= 0)) = 1;
  