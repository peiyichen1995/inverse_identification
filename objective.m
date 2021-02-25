function [f,g,H] = objective(x, lambda1_filename, sigma_exp_filename)

writematrix([x;1],'x.csv','Delimiter',' ');

system(['./objective ', lambda1_filename, ' ', sigma_exp_filename]);

opts = delimitedTextImportOptions;
opts.Delimiter = ' ';
opts.ConsecutiveDelimitersRule = 'join';
opts.LeadingDelimitersRule = 'ignore';
opts = setvartype(opts,'double');
f = readmatrix('f.csv', opts);
g = readmatrix('g.csv', opts);
H = readmatrix('H.csv', opts);