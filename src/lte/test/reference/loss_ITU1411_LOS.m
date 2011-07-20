function g = loss_ITU1411_LOS (d, hb, hm, hr, f)

  %%
  %% function g = loss_ITU1411_LOS(d, c)
  %%
  %% returns the loss at d meters for f frequency and mobile height m and
  %% base station height of hb
  
  assert(isscalar(f));
  assert(f > 0);


  lambda = 300000000.0 / f;
  Lbp = abs(20*log10(lambda^2/(8*pi*hb*hm)));
  Rbp = (4*hb*hm) / lambda;
  if (d<=Rbp)
    Ll = Lbp + 20.*log10(d./Rbp);
    Lu = Lbp + 20 + 25.*log10(d./Rbp);
  else
    Ll = Lbp + 40.*log10(d./Rbp);
    Lu = Lbp + 20 + 40.*log10(d./Rbp);
  endif

  Lbp
  Rbp
lambda
  g = zeros(size(d));
  g(find(d > 0)) = (Ll.+Lu)./2;

  g(find(d <= 0)) = 1;
  