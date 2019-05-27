%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Optimal path: starting from the last node, backtrack to
% its parent nodes until it reaches the start node
% 04-26-2005    Copyright 2009-2010 The MathWorks, Inc.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Optimal_path = [];
QUEUE_COUNT = size(QUEUE, 1);
num_of_exp = QUEUE_COUNT;
num_of_dis = 0;

for i = 1:QUEUE_COUNT

    if (QUEUE(i, 1) == 0)
        num_of_dis = num_of_dis + 1;
    end

end

xval = QUEUE(QUEUE_COUNT, 2);
yval = QUEUE(QUEUE_COUNT, 3);

temp = QUEUE_COUNT;
% get the last node
while (((xval ~= xTarget) || (yval ~= yTarget)) && temp > 1)
    temp = temp - 1;
    xval = QUEUE(temp, 2);
    yval = QUEUE(temp, 3);
end

path_cost_fin = 0;
i = 1;
Optimal_path(i, 1) = xval;
Optimal_path(i, 2) = yval;

if ((xval == xTarget) && (yval == yTarget))
    inode = 0;
    % Traverse QUEUE and determine the parent nodes
    parent_x = QUEUE(index(QUEUE, xval, yval), 4);
    parent_y = QUEUE(index(QUEUE, xval, yval), 5);

    while (parent_x ~= xStart || parent_y ~= yStart)
        i = i + 1;
        Optimal_path(i, 1) = parent_x; % store nodes on the optimal path
        Optimal_path(i, 2) = parent_y;
        maze_output(parent_y, parent_x) = 6;
        dispMaze(maze_output);
        % fprintf("Optimal : %d %d\n", parent_x, parent_y);
        inode = index(QUEUE, parent_x, parent_y); % find the grandparents :)
        parent_x = QUEUE(inode, 4);
        parent_y = QUEUE(inode, 5);

        if (QUEUE(inode, 6) > path_cost_fin)
            path_cost_fin = QUEUE(inode, 6);
        end

    end

    Optimal_path(i + 1, 1) = xStart; % add start node to the optimal path
    Optimal_path(i + 1, 2) = yStart;
else
    pause(1);
    h = msgbox('Oops! No path exists to the Target!', 'warn');
    uiwait(h, 5);
end
figure(2);
c = categorical({'total path cost','number of nodes discovered','number of nodes expanded'});
prices = [path_cost_fin num_of_dis num_of_exp];
bar(c,prices)
fprintf("total path cost : %d\nnumber of nodes discovered : %d\nnumber of nodes expanded : %d\n", path_cost_fin, num_of_dis, num_of_exp);
