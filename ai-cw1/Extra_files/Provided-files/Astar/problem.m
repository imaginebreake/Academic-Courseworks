%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Define the problem: map, location of target, start and obstacles
% Copyright 2009-2010 The MathWorks, Inc.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Define a 2-D map arrary to store the coordinates of the map and the objects
MAX_X = 20;
MAX_Y = 20;
MAP = 2 * (ones(MAX_X, MAX_Y));

% Obtain Obstacle, Target and Robot Position
% MAP with input values: Obstacle = -1, Target = 0, Robot = 1, Space = 2
x_val = 1;
y_val = 1;
axis([1 MAX_X+1 1 MAX_Y+1])
grid on;
hold on;

% Target location selection
pause(1);

xlabel('Select Target: Left mouse click', 'Color', 'red');
but = 0;
while (but ~= 1) % Repeat until the Left button is not clicked
    [xval, yval, but] = ginput(1);
end
xval = floor(xval);
yval = floor(yval);
xTarget = xval;
yTarget = yval;
MAP(xval, yval) = 0;
plot(xval + .5, yval + .5, 'gd'); % plot Target
text(xval + 1, yval + .5, 'Target')

% Obstacle location selection
pause(1);
xlabel('Select Obstacles: Left mouse click; Last obstacle: Right mouse click', 'Color', 'blue');
while but == 1
    [xval, yval, but] = ginput(1);
    xval = floor(xval);
    yval = floor(yval);
    MAP(xval, yval) = -1;
    plot(xval + .5, yval + .5, 'ro'); % plot the Obstacles
end
pause(1);

% Start location selection
xlabel('Select initial position: Left mouse click', 'Color', 'black');
but = 0;
while (but ~= 1)
    [xval, yval, but] = ginput(1);
    xval = floor(xval);
    yval = floor(yval);
end
xStart = xval;
yStart = yval;
MAP(xval, yval) = 1;
plot(xval + .5,yval + .5,'bo'); % plot the Start