clear
clc
close all

%%
lambda1_filename = 'data/circ_l_13.csv';
sigma_exp_filename = 'data/circ_sigma_13.csv';

alpha0 = [1.2; 0.1; 20; 11];
lb = [0; 0; 0; 0];
ub = [];
A = [];
b = [];
Aeq = [];
beq = [];
nonlcon = [];

fun = @(alpha) objective(alpha, lambda1_filename, sigma_exp_filename);
opts = optimoptions('fmincon', ...
    'SpecifyObjectiveGradient', true, ...
    'HessianFcn', 'objective', ...
    'Algorithm', 'trust-region-reflective', ...
    'Display', 'iter');

%%

alpha = fmincon(fun, alpha0, A, b, Aeq, beq, lb, ub,nonlcon,opts);

%% Compare

opts = delimitedTextImportOptions;
opts.Delimiter = ' ';
opts.ConsecutiveDelimitersRule = 'join';
opts.LeadingDelimitersRule = 'ignore';
opts = setvartype(opts,'double');
lambda = readmatrix(lambda1_filename, opts);
sigma_exp = readmatrix(sigma_exp_filename, opts);
sigma_fit = readmatrix('sigma.csv', opts);
plot(lambda, sigma_exp)
hold on
plot(lambda, sigma_fit)
legend('experiment', 'fitted')