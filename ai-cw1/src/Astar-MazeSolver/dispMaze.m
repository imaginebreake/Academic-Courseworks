function [] = dispMaze(maze)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % ENGR 132 Program Description
    %	This function This function formats the matrix to be graphed via
    %   heatmap. This is a two dimensional graph in which there are different
    %   colors based on the value in the matrix. That way, the walls can be
    %   grey, paths blue, and start/end orange.
    %
    % Function Call
    %   function [] = dispMaze(maze)
    %
    % Input Arguments
    %	1. maze: Current maze
    %
    % Output Arguments
    %	1. None
    %
    % Assignment Information
    %	Assignment:         MATLAB Individual Project
    %	Author:             Ryan Schwartz, schwar95@purdue.edu
    %  	Team ID:            001-07
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %% INITIALIZATION ---
    % Color map

    % 0->obstacle 1->path 3->intrance 4->exit 5->visited 6->answer 8->bound
    cmap = [.12 .39 1; 1 1 1; 0 0 0; 1 .5 0; .65 1 0; 1 0 0; 0 0 0; 0 0 0; .65 .65 .65];
    % cmap = [.10 .52 .82;0.86 0.956 1; 0 0 0; 1 .5 0; .65 1 0; 1 0.717 0.835; .76 .094 .357; 0 0 0; .65 .65 .65];

    %% CALCULATIONS ---
    % Create live imagery of map
    hmo = imagesc(maze);
    colormap(cmap);
    set(gca, 'XColor', 'none');
    set(gca, 'Ycolor', 'none');
    drawnow

    %% FORMATTED TEXT & FIGURE DISPLAYS ---

    %% COMMAND WINDOW OUTPUTS ---

    %% ACADEMIC INTEGRITY STATEMENT ---
    % I/We have not used source code obtained from any other unauthorized
    % source, either modified or unmodified.  Neither have I/we provided
    % access to my/our code to another. The project I/we am/are submitting
    % is my/our own original work.
    %
