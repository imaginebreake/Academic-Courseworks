%% Matlab Tutorial Script - Session 1 
% (c) 2011 David Weiss

%% Matlab Basics
%{
   - Current folder: shows all files under the selected folder
   - Command window: type individual commands, click Up: history of command
   - Workspace: stores all current variales in use
   - Editor: matlab script .m, blocks of commands (code) which you can
     "Run", "Run Section". type "edit" or Ctrl+N in command window to open Editor

    % Set and choose a working folder to save all your G51FAI work
    % try each of the following commands, or Run Section, for help use "doc xx"
    % Tutorial "Working in The Development Environment" @ MathWorks
%}

% Enter the following commands individually in the Command Window, check your workspace
2 + 3 % simply output the result, stored in the default vector ans
x = 2 
y = 3 % variables x and y are stored in your current workspace
x + y
x * y
z % what happened?
log(x)
exp(x)
x = 5; % suppress output with semi-colons
log(x)


%% Matlab: Mostly used commands
clear % empty your workspace
clc % clear your command window
% comment line; %% section marker: click the "Run Section" button to run
% the section where your cursor is
doc sort % help document of sort
doc plot


%% Matlab: every variable is a matrix
x = 5;
y = [1 2 3; 4 5 6]
y = [1 2 3
    4 5 6]
y = [1 2 3 ... % elipses (...) breaks lines without breaking commands
    4 5 6]

% basic matrix initializations:
x = ones(3, 5)
x = zeros(3, 5)
x = nan(3, 5)
x = inf(3, 5)
x = eye(5)

% length vs. size vs. numel
size(x)     % size of each dimension of an n-dimensional array
length(x)   % size of largest dimension
numel(x)    % number of elements
ndims(x)    % dimensions of x


%% Matrix: easy creation
x = rand(3, 5)   % uniform(0,1)
x = randn(3, 5)  % normal(0,1)

A = round(rand(2) * 10)  % what does this command do?
At = A'     % transposition!! very important.
C = [A A]   % check values of A and C in workspace
C = horzcat(A, A)
D = [A; A]
E = vertcat(B, B)
F = [A At; At A]
G = repmat(A, [2 3])  % arbitrary replication of matrices by 2 X 3

x = 1 : 10
x = 1 : 2 : 10  % start:stepsize:end
x = 1 : -1 : 10 % what  happened?
x = 10 : -1 : 1
x = 0 : 0.1 : 1


%% Matrix: easy operations on data

A = 2 * ones(4)
A + A % matrix addition
A + 1 
A * A  % linear algebra: matrix multiplication
A' % matrix transpose
A .* A % element-wise multiplication
A./A % element-wise division
A.^A % element-wise power

log(A) % element-wise logarithm (natural)
exp(A) % element-wise exponentiate (natural)

A = 1 : 24
B = reshape(A, [4 6]) % counts down columns, then across rows
C = reshape(A, [6 4])
D = reshape(A, [5 5]) % what happened? dimensions don't match


%% Matrix: easy operations over dimension and elements

sum(A) % VECTORIZING operation: coverts matrix to vectors, vectors to scalars
sum(A, 1) % operate over dimension 1 rows: produce 1 value per row
sum(A, 2) % operate over dimension 2 columns: produce 1 value per column

x = 10 - (1 : 6)
x(1 : 3)
x(4 : end)
x([4 5 6])

B(1 : 4) % get elements #1:4
B(:, 1 : 4) % get columns 1:4
B(2 : 3, 1 : 4) % get submatrix from rows 2,3, columns 1:4 in B.


%% Matrix: logical indexing

x > 5
x == 7 % 1: true, 0: false

x > 5 % compare these operations
find(x > 5)
x(x > 5)
x(find(x > 5))

x == 7
find(x == 7)
x(find(x == 7))
x(x == 7)

B(B < 15) = 0 % check B before this operation

B(B < 15) = rand(numel(find(B < 15)), 1) % break down the command to check the output


%% Matlab: Documentation

save workspace.mat % saves everything in workspace, check your folder
clear % check your workspace
load workspace.mat % re-loads everything, check your workspace
save workspace.mat B C % saves only B and C
load workspace.mat B  % loads only B

%% Matlab: Control Logic
% You'll use sme of these in Coursework I

for i = 1:10
    i
end

a = 1:5
s = 0
for i = 1 : numel(a)
    s = s + a(i)
end
sum(a)

i = 0
while i < 10
    i = i + 1
end

i = 3
if i < 5
    x = sum(a)
else
    x = 5
end

if i < 5 & i > 2
    i
end

if i < 5
    if i > 2
        i
    end
end
        

%% Matlab: Plotting
a = [10 20 30]
b = [20 50 100]
bar(a, b, 0.5, 'b') % could configure color, width etc. see "doc bar"

x = -pi : 0.1 : pi;
plot(sin(x))

% Plot a sine curve, where we specify both x and y values.
plot(x, sin(x)) % You can edit and save the figure in the new window
plot(x, sin(x), '-ro')
doc plot

figure(1); % Create another plotting area
plot(x, sin(x))
figure(2); % what happends if you dont have this command?
plot(x, cos(x));

close(1);
close('all');

plot(x, sin(x), '-ro');
hold on; % allow to draw on an existing plot
plot(x, cos(x), '-bx')
plot(x, tan(x), '-g.')
hold off;

ylim([-1 1]); % manually specify axes
legend({'Sin', 'Cos', 'Tan'});
ylim([-2 2]);

y1 = sin(x);
y2 = sin(5*x);

figure
subplot(2, 2, 1); % partition the figure space into 2 * 2 divisions
plot(x, y1)
subplot(2, 2, 2); % plot at the next position
plot(x, y2)

figure('Name', 'Histogram Plot');
hist(randn(10000, 1), 100); % histogram with 100 bins
print -djpeg -r72 histogram.jpg % print and save the current plot in workspace
bar([sin(-5:1:5)' cos(-5:1:5)']) % Why do we need to transpose?
legend({'Sin', 'Cos'});
print -dpdf barchart.pdf


%% Matlab: strings and characters, struct

str = 'this is a string variable'
str(2) % a string is a matrix too
str(5 : end)

double(str) % char is just a number that corresponds to a character
char(str - 1) % match back to characters, Sheldon must be good at this!

% if strings are arrays, how to make array of strings? answer: CELL arrays.
clear % empty workspace
a = {'string 1', 'string 2', 'string 3'}
a{2, 1} = 'string 3'

% structs: to organize multiple variables of different types
s.a = rand(5)
s.b = rand(3)
s.c = rand(10)
s.d = 'hello!'
s.d

model.theta = 5
model.alpha = -1
model.verbose = true
model.fit = 3;


%% Matlab: functions

%{
create and save another .m file with the following content in your folder
function dist = distance(x1, y1, x2, y2)
dist = sqrt((x1-x2)^2 + (y1-y2)^2);
%}

d = distance(0, 0, 3, 4)    % what does this function do?

%% What's next?
%{
    % Learn a bit more @ Mathworks, useful for writing reports
    % Tutorial "Using Basic Plotting Functions"
    % Tutorial "Getting Started with MATLAB"
%}