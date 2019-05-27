% This function returns the index of the location of a node in QUEUE
% Copyright 2009-2010 The MathWorks, Inc.

function n_index = getSibling(QUEUE, xval, yval)
    n_index = -1;

    for i = 1:size(QUEUE, 1)

        if (QUEUE(i, 4) == xval) && (QUEUE(i, 5) == yval) && (QUEUE(i, 1) == 1)
            n_index = i;
            break;
        end

    end

end
