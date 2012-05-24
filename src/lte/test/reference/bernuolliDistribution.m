% Generation of the quantiles of the Bernoulli distribution at 95%
% related to phy error model test

n = 1000;
p_vect = [0.33 0.11 0.02 0.3 0.55 0.14];
for i=1:length(p_vect)
  p = p_vect(i)
  cdf = 0.0;
  kmin = 0.0;
  kmax = 0.0;
  lambda = n * p
  for k=1:n
    pk = bincoeff(n,k)* p^k * (1-p)^(n-k);
    cdf = cdf + pk;
    if cdf > 0.025 & kmin == 0.0
      kmin = lambda - k
    end
    if cdf > 0.975 & kmax == 0.0
      kmax = k - lambda
    end
  end

end


