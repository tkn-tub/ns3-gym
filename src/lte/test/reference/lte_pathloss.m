clear all;
close all;

%% LTE pathloss model
%% ITU1411, ITU1238, COST231, OH, etc.

f = 2160e6;  % carrier freq Hz, EARFCN = 500 (downlink)
d = 100;
hb = 30;
hm = 1;
hr = 20;
g = loss_COST231_large_cities_urban (d, hb, hm, f);
disp ("The value of COST231 for large cities is:"), disp (g)

g = loss_COST231_small_cities_urban (d, hb, hm, f);
disp ("The value of COST231 for small cities is:"), disp (g)

g = loss_OH_large_cities_urban (d, hb, hm, f);
disp ("The value of OH for large cities is:"), disp (g)

g = loss_OH_small_cities_urban (d, hb, hm, f);
disp ("The value of OH for small cities is:"), disp (g)

g = loss_OH_suburban (d, hb, hm, f);
disp ("The value of OH in suburban is:"), disp (g)

g = loss_OH_openareas (d, hb, hm, f);
disp ("The value of OH in openareas is:"), disp (g)

g = loss_OH_2_6GHz (d);
disp ("The value of OH at 2.6 GHz is:"), disp (g)

g = loss_ITU1411_LOS (d, hb, hm, hr, f);
disp ("The value of ITU1411 in LOS is:"), disp (g)

l = 80;
b = 50;
st_w = 20;
phi = 45;
big = 1; % metropolitan centre
g = loss_ITU1411_NLOS_over_rooftop (d, hb, hm, hr, f, l, b, st_w, phi, big);
disp ("The value of ITU1411 in NLOS over the roof-top is:"), disp (g)

w1 = 30;
w2 = 30;
x1 = 40;
x2 = 40;
alpha = 0.5;
g = loss_ITU1411_NLOS_street_canyons (d, f, w1, w2, x1, x2, alpha);
disp ("The value of ITU1411 in NLOS within street canyons is:"), disp (g)

g = loss_ITU1238 (d, hb, hm, f);
disp ("The value of ITU1238 is:"), disp (g)


%%snr = txPsd + g - kT - nf ; % dB

      


