% Initialization
clear ; close all; clc

% Traverse Animal Image Folder
fprintf('Loading Images ...\n')

imgPath = '.\animals\';
imgDir  = dir([imgPath '*.jpg']);

% pre alloc matrix
X = zeros(40000, 3000);
y = zeros(3, 3000);

for i = 1:length(imgDir)
    % read image file
    img = imread([imgPath imgDir(i).name]);
    % to gray scale
    img_gray = rgb2gray(img);
    % resize to 200x200
    img_resize = imresize(img_gray, [200 200]);
    % transfer 2d image to 1d
    img_1d = reshape(img_resize,[1,40000]);
    % add 1d img to X
    X(:,i) = img_1d;

    % find y label
    if ~isempty(strfind(imgDir(i).name, "cat"))
        y(:,i) = [1 0 0];
    elseif ~isempty(strfind(imgDir(i).name, "dog"))
        y(:,i) = [0 1 0];
    else
        y(:,i) = [0 0 1];
    end
end

% save variables
fprintf("X and y saved!\n");
save('data.mat', 'X', 'y');