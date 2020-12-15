Dimension = 2;
N = 2000;
G = 100;
epsilon = 10^(-20);
dt = .001;


r = abs(normrnd(0, 1, [1, N]));
theta = 2*pi*rand([1, N]);

p = [cos(theta).*r; sin(theta).*r];
a = zeros(Dimension, N);
%v = zeros(Dimension, N);
%p = zeros(Dimension, N);
%p = 5*rand([Dimension, N]) - 2.5;
v = 7*[r.*p(2, :) - mean(p(2, :)); -r.*p(1, :) + mean(p(1, :))];





 M(1000) = struct('cdata',[],'colormap',[]);
 h = figure;
for T = 1:1000
    

    
for particle = 1:N
    
    for particle2 = 1:N
        a(:, particle) = a(:, particle) + G*(p(:, particle2) - p(:, particle))/(norm(particle2 - particle)^2 + epsilon^2)^(3/2);
    end
end

for particle = 1:N
    v(:, particle) = v(:, particle) + dt*a(:, particle);
    p(:, particle) = p(:, particle) + dt*v(:, particle);
end
a = zeros(Dimension, N);
   
    axis tight manual
    ax = gca;
    ax.NextPlot = 'replaceChildren';
    
   
    
        xlim([-5 5])
        ylim([-5 5])
        scatter(p(1, :), p(2, :), 5, 'filled')
        drawnow
        M(T) = getframe;
    
    h.Visible = 'on';


end




