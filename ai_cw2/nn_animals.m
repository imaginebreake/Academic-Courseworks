% Initialization
clear ; close all; clc

% load X and y
load('data.mat');

% create feedforward network
net = patternnet([1500 256 16 16]);
net.trainFcn = 'trainscg';
net.trainParam.epochs = 1000;
net.trainParam.goal = 0.1;
net.trainParam.show = 100;
net.trainParam.lr = 0.05;

% divided into training, validation and testing simulate
net.divideParam.trainRatio = 0.8;
net.divideParam.valRatio = 0;
net.divideParam.testRatio = 0.2;

% split data
rand_indices = randperm(size(X, 2));
                                             
testData = X(:, rand_indices(1:600));
testLabels = y(:, rand_indices(1:600));
trainData = X(:, rand_indices(601:end));
trainLabels = y(:, rand_indices(601:end));

% train data
net = train(net, trainData, trainLabels);

% show network
view(net);

% save network
save('patternnet.mat', 'net');

% test performance
preds = net(testData);
%% est = vec2ind(preds) - 1;
%% tar = vec2ind(testLabels') - 1;

% plot confusion matrix of test data
plotconfusion(testLabels,preds);