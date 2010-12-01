% results from c++
maxN = 10;
t = cell(maxN,1);
z = cell(maxN,1);


t{1} = [0,0.211325,0.788675,1];
z{1} = [-3,-1.47838,0.399027,0.251762];

t{2} = [0,0.105663,0.394338,0.5,0.605662,0.894338,1];
z{2} = [-3,-1.9503,-0.40786,-0.32842,-0.171381,0.134733,0.201785];

t{3} = [0,0.0704417,0.262892,0.333333,0.403775,0.596225,0.666667,0.737108,0.929558,1];
z{3} = [-3,-2.19476,-0.874521,-0.713281,-0.534788,-0.175222,-0.090494,-0.0140359,0.15381,0.200224];

t{4} = [0,0.0528313,0.197169,0.25,0.302831,0.447169,0.5,0.552831,0.697169,0.75,0.802831,0.947169,1];
z{4} = [-3,-2.3465,-1.19163,-0.999711,-0.813831,-0.429021,-0.3332,-0.246793,-0.0543765,7.53502e-05,0.0497711,0.165226,0.200048];

t{5} = [0,0.042265,0.157735,0.2,0.242265,0.357735,0.4,0.442265,0.557735,0.6,0.642265,0.757735,0.8,0.842265,0.957735,1];
z{5} = [-3,-2.45023,-1.42378,-1.22212,-1.03574,-0.640998,-0.53841,-0.445733,-0.23675,-0.17644,-0.121195,0.00817704,0.0476392,0.0842676,0.172241,0.200014];

t{6} = [0,0.0352208,0.131446,0.166667,0.201887,0.298113,0.333333,0.368554,0.464779,0.5,0.535221,0.631446,0.666667,0.701887,0.798113,0.833333,0.868554,0.964779,1];
z{6} = [-3,-2.5257,-1.60191,-1.39996,-1.2166,-0.820718,-0.714263,-0.617804,-0.397866,-0.333319,-0.273982,-0.134021,-0.0908996,-0.0507603,0.046136,0.0769299,0.105874,0.176932,0.200005];

t{7} = [0,0.0301893,0.112668,0.142857,0.173046,0.255525,0.285714,0.315904,0.398382,0.428571,0.458761,0.541239,0.571429,0.601618,0.684096,0.714286,0.744475,0.826954,0.857143,0.887332,0.969811,1];
z{7} = [-3,-2.58307,-1.74327,-1.54544,-1.36691,-0.975013,-0.866655,-0.768127,-0.541241,-0.473677,-0.411356,-0.263386,-0.217386,-0.174454,-0.0703269,-0.0370335,-0.005675,0.0715804,0.0967769,0.120679,0.180276,0.200002];

t{8} = [0,0.0264156,0.0985844,0.125,0.151416,0.223584,0.25,0.276416,0.348584,0.375,0.401416,0.473584,0.5,0.526416,0.598584,0.625,0.651416,0.723584,0.75,0.776416,0.848584,0.875,0.901416,0.973584,1];
z{8} = [-3,-2.62814,-1.85834,-1.66666,-1.49381,-1.10891,-0.999994,-0.900589,-0.66965,-0.599996,-0.53554,-0.381581,-0.333331,-0.288185,-0.178214,-0.142855,-0.109487,-0.0270083,1.5484e-06,0.0256632,0.089813,0.111112,0.131458,0.182777,0.200001];

t{9} = [0,0.0234806,0.0876306,0.111111,0.134592,0.198742,0.222222,0.245703,0.309853,0.333333,0.356814,0.420964,0.444444,0.467925,0.532075,0.555556,0.579036,0.643186,0.666667,0.690147,0.754297,0.777778,0.801258,0.865408,0.888889,0.912369,0.976519,1];
z{9} = [-3,-2.66448,-1.9539,-1.76923,-1.6024,-1.22621,-1.11764,-1.0182,-0.785318,-0.714283,-0.64835,-0.489991,-0.439998,-0.393112,-0.278438,-0.241378,-0.20634,-0.119466,-0.0909082,-0.0637365,0.0043538,0.0270278,0.0487116,0.103516,0.121952,0.139656,0.184718,0.200001];

t{10} = [0,0.0211325,0.0788675,0.1,0.121133,0.178868,0.2,0.221133,0.278867,0.3,0.321133,0.378868,0.4,0.421133,0.478868,0.5,0.521132,0.578867,0.6,0.621133,0.678868,0.7,0.721133,0.778868,0.8,0.821133,0.878868,0.9,0.921133,0.978868,1];
z{10} = [-3,-2.6944,-2.03457,-1.85714,-1.69637,-1.3298,-1.22222,-1.12332,-0.890051,-0.81818,-0.751281,-0.589784,-0.538461,-0.49022,-0.371789,-0.333333,-0.296911,-0.206347,-0.17647,-0.148004,-0.0765057,-0.0526311,-0.0297734,0.0281062,0.0476194,0.0663761,0.11419,0.130435,0.146103,0.186266,0.2];

styles = {'b','g','r','c','m','y','k','b.','g.','r.'};
name = {'1','2','3','4','5','6','7','8','9','10'};

figure(1)
clf
hold on
for i=1:maxN
    plot(t{i},z{i},styles{i});
end
legend(name)

z_analytic = cell(maxN,1);
err = nan(maxN,1);
err_last = nan(maxN,1);
for i=1:maxN
    z_analytic{i} = (4*t{i} -3) ./ (4*t{i} + 1);
    err(i) = max(z_analytic{i} - z{i});
    err_last(i) = abs(z_analytic{i}(end) - z{i}(end));
end

figure(2)
clf;
semilogy(1:maxN,err_last)
grid on

%plot(t,z);
