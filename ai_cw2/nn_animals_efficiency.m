% Initialization
clear ; close all; clc

% load X and y
load('data.mat')

%% efficiency

% normalize X
X_normalized = X / 255;

% PCA X
[coef,score,latent,t2] = pca(X_normalized');
X_PCA = score';

% create feedforward network
net = patternnet([256 128 16]);
net.trainFcn = 'trainscg';
net.trainParam.epochs = 1000;
net.trainParam.goal = 0.1;
net.trainParam.show = 100;
net.trainParam.lr = 0.05;

% divided into training, validation and testing simulate
net.divideParam.trainRatio = 0.8;
net.divideParam.valRatio = 0;
net.divideParam.testRatio = 0.2;

rand_indices = randperm(size(X_PCA, 2));
                                             
testData = X_PCA(:, rand_indices(1:600));
testLabels = y(:, rand_indices(1:600));
trainData = X_PCA(:, rand_indices(601:end));
trainLabels = y(:, rand_indices(601:end));

% train data
net = train(net, trainData, trainLabels);

% show network
view(net);

% save network
save('patternnet_pca.mat', 'net');

% test performance
preds = net(testData);
%% est = vec2ind(preds) - 1;
%% tar = vec2ind(testLabels') - 1;

% plot confusion matrix of test data
plotconfusion(testLabels,preds);