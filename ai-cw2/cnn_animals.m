% load X and y
load('data.mat')

X = X';
y = y';

% transform X to cnn type
X = reshape(X, [200,200,1,size(X,1)]);

% tranform one-hot to class number
y_cnn = zeros(3000,1);
for i = 1:size(y,1)
    if y(i,1) == 1
        y_cnn(i,1) = 1;
    elseif y(i,2) == 1
        y_cnn(i,1) = 2;
    else
        y_cnn(i,1) = 3;
    end
end
y = y_cnn;

% split data
rand_indices = randperm(size(X, 4));
                                             
testData = X(:, :, :, rand_indices(1:600));
testLabels = y(rand_indices(1:600), :);
trainData = X(:, :, :, rand_indices(601:end));
trainLabels = y(rand_indices(601:end), :);

% network layers
layers = [
    imageInputLayer([200 200 1])
    
    convolution2dLayer(3,8)
    reluLayer
    maxPooling2dLayer(2,'Stride',2)
    
    convolution2dLayer(3,16)
    reluLayer
    maxPooling2dLayer(2,'Stride',2)
    
    convolution2dLayer(3,32)
    reluLayer
    
    fullyConnectedLayer(3)
    softmaxLayer
    classificationLayer];

% training options
options = trainingOptions('sgdm','MaxEpochs',10, ...
	'InitialLearnRate',0.0001);  

% train data
net = trainNetwork(trainData,categorical(trainLabels),layers,options);

% save network
save('cnn.mat', 'net');


% test performance
testPred = classify(net,testData);
accuracy = sum(testPred == categorical(testLabels))/numel(testLabels);
fprintf("accuracy of CNN : %f\n", accuracy);

