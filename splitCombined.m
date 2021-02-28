function [lambdas, sigmas] = splitCombined(lambda, sigma)

count = 1;
current_lambda = lambda(1);
current_sigma = sigma(1);
for i = 2:length(lambda)
    if lambda(i) > current_lambda(end)
        current_lambda = [current_lambda; lambda(i)];
        current_sigma = [current_sigma; sigma(i)];
    else
        lambdas{count} = current_lambda;
        sigmas{count} = current_sigma;
        count = count + 1;
        current_lambda = lambda(i);
        current_sigma = sigma(i);
    end
end
lambdas{count} = current_lambda;
sigmas{count} = current_sigma;
