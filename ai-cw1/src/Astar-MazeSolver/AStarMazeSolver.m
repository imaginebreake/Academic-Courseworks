function [] = AStarMazeSolver(maze)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % AStarMazeSolver Function
    %	This script is the main controller of the maze generation program.
    %% Assignment Information

    %	Assignment:         COMP1037_CW1_1819
    %	Author:             Tianyi GAO, csytg1@nottingham.edu.cn
    %  	Student ID:         20028309
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    maze_output = maze;

    %% Get size of maze
    MAX_X = size(maze, 2);
    MAX_Y = size(maze, 1);

    %% find exit and intrance
    % bound: [X val, Y val]
    bound = zeros(2 * MAX_Y + 2 * MAX_X, 2);
    intrance = [-1, -1];
    exit = [-1, -1];
    k = 1;

    for i = 1:MAX_X
        bound(k, 1) = i;
        bound(k, 2) = 1;
        bound(k + 1, 1) = i;
        bound(k + 1, 2) = MAX_Y;
        k = k + 2;
    end

    for j = 1:MAX_Y
        bound(k, 1) = 1;
        bound(k, 2) = j;
        bound(k + 1, 1) = MAX_X;
        bound(k + 1, 2) = j;
        k = k + 2;
    end

    for i = 1:size(bound, 1)

        if (maze(bound(i, 1), bound(i, 2)) == 3)
            intrance = [bound(i, 1), bound(i, 2)];
        elseif (maze(bound(i, 1), bound(i, 2)) == 4)
            exit = [bound(i, 1), bound(i, 2)];
        end

    end

    xStart = intrance(1, 2);
    yStart = intrance(1, 1);
    xTarget = exit(1, 2);
    yTarget = exit(1, 1);
    % OBSTACLE: [X val, Y val]
    OBSTACLE = [];
    k = 1;

    for i = 1:MAX_X

        for j = 1:MAX_Y

            if ((maze(i, j) == 0) || (maze(i, j) == 8))
                OBSTACLE(k, 1) = j;
                OBSTACLE(k, 2) = i;
                k = k + 1;
            end

        end

    end

    OBST_COUNT = size(OBSTACLE, 1);
    OBST_COUNT = OBST_COUNT + 1;
    OBSTACLE(OBST_COUNT, :) = [xStart, yStart];

    %% add the starting node as the first node (root node) in QUEUE
    % QUEUE: [0/1, X val, Y val, Parent X val, Parent Y val, g(n),h(n), f(n)]
    xNode = xStart;
    yNode = yStart;
    QUEUE = [];
    QUEUE_COUNT = 1;
    NoPath = 1; % assume there exists a path
    path_cost = 0; % cost g(n): start node to the current node n
    goal_distance = distance(xNode, yNode, xTarget, yTarget); % cost h(n): heuristic cost of n
    QUEUE(QUEUE_COUNT, :) = insert(xNode, yNode, xNode, yNode, path_cost, goal_distance, goal_distance);
    QUEUE(QUEUE_COUNT, 1) = 0;

    %% Start the search
    while ((xNode ~= xTarget || yNode ~= yTarget) && NoPath == 1)
        % show step update
        if not(((xNode == xStart) && (yNode == yStart)) || ((xNode == xTarget) && (yNode == yTarget)))
            maze_output(yNode, xNode) = 5;
            dispMaze(maze_output);
        end

        % expand the current node to obtain child nodes
        exp = expand(xNode, yNode, path_cost, xTarget, yTarget, OBSTACLE, MAX_X, MAX_Y);
        exp_count = size(exp, 1);
        % Update QUEUE with child nodes; exp: [X val, Y val, g(n), h(n), f(n)]
        for i = 1:exp_count
            flag = 0;

            for j = 1:QUEUE_COUNT
                % same position
                if (exp(i, 1) == QUEUE(j, 2) && exp(i, 2) == QUEUE(j, 3))
                    % update the f(n) of node on QUEUE
                    QUEUE(j, 8) = min(QUEUE(j, 8), exp(i, 5));
                    % this is true means QUEUE(j,8) is already updated
                    if QUEUE(j, 8) == exp(i, 5)
                        % update parents, g(n) and h(n)
                        QUEUE(j, 4) = xNode;
                        QUEUE(j, 5) = yNode;
                        QUEUE(j, 6) = exp(i, 3);
                        QUEUE(j, 7) = exp(i, 4);
                    end % end of minimum f(n) check

                    flag = 1;
                end

            end

            if flag == 0
                QUEUE_COUNT = QUEUE_COUNT + 1;
                QUEUE(QUEUE_COUNT, :) = insert(exp(i, 1), exp(i, 2), xNode, yNode, exp(i, 3), exp(i, 4), exp(i, 5));
            end % end of insert new element into QUEUE

        end

        % A*: find the node in QUEUE with the smallest f(n), returned by min_fn
        index_min_node = min_fn(QUEUE, QUEUE_COUNT);

        if (index_min_node ~= -1)
            % set current node (xNode, yNode) to the node with minimum f(n)
            xNode = QUEUE(index_min_node, 2);
            yNode = QUEUE(index_min_node, 3);
            path_cost = QUEUE(index_min_node, 6); % cost g(n)
            % move the node to OBSTACLE
            OBST_COUNT = OBST_COUNT + 1;
            OBSTACLE(OBST_COUNT, 1) = xNode;
            OBSTACLE(OBST_COUNT, 2) = yNode;
            QUEUE(index_min_node, 1) = 0;
        else
            NoPath = 0; % there is no path!
        end
        
    end

    %% Output / plot your route
    if NoPath == 1
        result();
    elseif NoPath == 0
        fprintf("No Path!\n");
    end
