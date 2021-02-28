clear
clc
close all

alphas = [];
for sample = 1:13
    %%
    lambda1_filename = ['data/l_angle_all_',num2str(sample),'.csv'];
    sigma_exp_filename = ['data/stress_all_',num2str(sample),'.csv'];
    
    alpha0 = [0; 0; 9.7; 8.7; 6; 4; 0.11];
    lb = [0; 0; 0; 0; 2; 0; 0];
    ub = [inf; inf; inf; inf; inf; inf; 1];
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
%     opts = optimoptions('fmincon', ...
%         'SpecifyObjectiveGradient', true, ...
%         'Algorithm', 'interior-point', ...
%         'Display', 'iter');
    
    %%
    
    alpha = fmincon(fun, alpha0, A, b, Aeq, beq, lb, ub,nonlcon,opts);
    alphas = [alphas, alpha([1,2,4,6,7])];
    
    %% Compare
    
    opts = delimitedTextImportOptions;
    opts.Delimiter = ' ';
    opts.ConsecutiveDelimitersRule = 'join';
    opts.LeadingDelimitersRule = 'ignore';
    opts = setvartype(opts,'double');
    lambda = readmatrix(lambda1_filename, opts);
    sigma_exp = readmatrix(sigma_exp_filename, opts);
    sigma_fit = readmatrix('sigma.csv', opts);
    
    [lambda_split, sigma_exp_split] = splitCombined(lambda(:,1),sigma_exp);
    [~, sigma_fit_split] = splitCombined(lambda(:,1),sigma_fit);
    
    figure
    legends = cell(2*length(lambda_split),1);
    for i = 1:length(lambda_split)
        plot(lambda_split{i}, sigma_exp_split{i})
        hold on
        plot(lambda_split{i}, sigma_fit_split{i})
        legends{2*i-1} = ['experiment ', num2str(i)];
        legends{2*i} = ['fitted ', num2str(i)];
    end
    legend(legends)
    title(sprintf('mu_1 = %.2E, mu_2 = %.2E, \nmu_4 = %.2E, beta_4 = %.2E, rho = %.2E',...
        alpha(1), alpha(2), alpha(4), alpha(6), alpha(7)))
    xlabel('lambda_1')
    ylabel('sigma_1')
    ylim([0,inf])
    
    hold off
    saveas(gcf,['fitted_',num2str(sample),'.png']);
end