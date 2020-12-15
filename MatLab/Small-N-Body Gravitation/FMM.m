
%% A single level 2-D FMM algorithm by Angelo Tafuni.
clear all;
close all;
clc;
%% Initial data
prompt = 'Number of sources, N, to be allocated in the unit box [def = 10,000]: ';
N = input(prompt);        
% Number of sources, N.
if isempty(N)
   N=10000;
elseif N<8 
   disp(' ')
   display('Try a bigger N to generate at least four boxes!');   
   disp(' ')
end
N_b=round(sqrt(N));    
% Number of boxes, M, based on N = O(M^2).
L = round(sqrt(N_b));     
% Rounding the box size to fit the unit box.
M_disp = L^2;
% Number of generated boxes after rounding the box size.
m = rand(N,3);            
% Sources matrix, m = [x, y, q].
x = m(:,1);               
% Sources x-position vector.
y = m(:,2);               
% Sources y-position vector.
x_min = 0;                
% Minimum x grid point.
y_min = x_min;            
% Minimum y grid point.
x_max = 1;                
% Maximum x grid point.
y_max = x_max;            
% Maximum y grid point.
%% Computation via FMM
% - Mesh generation.
disp(' ')
display('Generating the mesh...');
fprintf('# of generated boxes M, based on N = O(M^2): %d ', M_disp);
disp(' ')
tic
x_edg = linspace(x_min,x_max,round(L)+1);
y_edg = linspace(y_min,y_max,round(L)+1);
x_cen1 = zeros(1,length(x_edg)-1);
y_cen1 = zeros(1,length(y_edg)-1);
% - Indexing box centers.
for aa = 1:length(x_edg)-1
    x_cen1(aa) = 0.5*(x_edg(aa)+x_edg(aa+1));
end
for bb = 1:length(y_edg)-1
    y_cen1(bb) = 0.5*(x_edg(bb)+x_edg(bb+1));
end
[x_cen2,y_cen2] = meshgrid(x_cen1,y_cen1);
xy_cen = [x_cen2(:) y_cen2(:)];
centers = cell(L,L);
count = 1;
for cc = 1:L
    for dd = 1:L
        centers{cc,dd} = xy_cen(count,1:2);
        count = count+1;
    end
end
% - Indexing boxes and source positions.
[n_x,M_x] = histc(x,x_edg);
[n_y,M_y] = histc(y,y_edg);
M = sortrows([m M_x M_y],[4,5]);
boxes = cell(L,L);
for ee = 1:L
    for ff = 1:L
        boxes{ee,ff} = M(M(:,4)==ee & M(:,5)==ff,:);
    end
end
toc
% - Constructing the moments of charge distribution wrt the box center.
disp(' ')
display('Using FMM to compute all pairwise interactions...');
prompt = ' - Input the desired multipole expansion accuracy [def=1e-12]: ';
eps = input(prompt);
if isempty(eps)
   eps = 0.000000000001;
elseif eps <= 0
    disp(' ')
    display('   This is not an acceptable value!');
end
fprintf('   Epsilon = %d', eps);
p = ceil(-log2(eps));
p_vec = (0:1:p);
disp(' ')
display(' - Constructing the Laurent series at each box center...');
tic
dist_pow = cell(L,L);
Q_k = cell(L,L);
for gg = 1:L
    for hh = 1:L
        dist = sqrt(sum((boxes{gg,hh}(:,1:2)-bsxfun(@times,ones...
               (size(boxes{gg,hh},1),1),centers{gg,hh})).^2,2));           
        % Distance of all sources in box{gg,hh} wrt the box center.
        dist_pow{gg,hh} = bsxfun(@times, kron(dist,ones(1,p+1))...
        .^kron(p_vec,ones(length(dist),1)), boxes{gg,hh}(:,3));            
        % All p-powers of "dist" times qj.
        Q_k{gg,hh} = sum(dist_pow{gg,hh},1);                               
        % Moments of charge distribution wrt the box center.
    end
end
% - Evaluation of the potential using multipole expansion at all relevant 
%   targets.
display(' - Evaluating the potential...');
[pr,qr] = meshgrid(1:L,1:L);
pairs = [qr(:) pr(:)];
ind = cell(L,L);
flag = cell(L,L);
int_list = cell(L,L);
n_neigh = cell(L,L);
for ii = 1:L
    for jj = 1:L
        ind{ii,jj} = abs(kron([ii,jj],ones(length(pairs),1))-pairs);
        flag{ii,jj} = sum((ind{ii,jj} >= kron([2,2], ...
                      ones(length(pairs),1))),2);
        int_list{ii,jj} = find(flag{ii,jj});
        % Vector containing the box number of boxes in the interaction
        % list of box{ii,jj}.
        n_neigh{ii,jj} = find(flag{ii,jj}==0);
        % Vector containing the box number of near neighbors of box {ii,jj}.
    end
end
phi_INT = cell(L,L);
dist_pow2 = cell(L,L);
for kk = 1:L
    for ll = 1:L
        n_par = size(boxes{kk,ll},1);
        temp = size(int_list{kk,ll},1);
        phi_INT{kk,ll}=zeros(n_par,1);
        for mm = 1:temp
            for mmm = 1:n_par
                dist2 = sqrt(sum((boxes{kk,ll}(mmm,1:2)-...
                centers{int_list{kk,ll}(mm)}).^2,2));
                dist_pow2{kk,ll} = (dist2+zeros(1,p+1)).^(p_vec+1);
                phi_INT{kk,ll}(mmm,1) = phi_INT{kk,ll}(mmm,1)+ ...
                                       (sum((Q_k{int_list{kk,ll}(mm)}./ ...
                                       dist_pow2{kk,ll}),2));
            end
        end
    end
end
% The cell phi_INT contains the field induced by all sources within the
% interaction list of box {kk,ll} containing the specific target {mmm}.
phi_NN = cell(L,L);
for nn=1:L                  
    for oo=1:L
        n_part = size(boxes{nn,oo},1);
        temp3 = size(n_neigh{nn,oo},1);
        phi_NN{nn,oo} = zeros(n_part,1);
            for pp = 1:temp3  
                pair_dist = zeros(size(boxes{n_neigh{nn,oo}(pp)},1),1);
                for qq = 1:n_part 
                       pair_dist = sqrt(sum((bsxfun(@times,ones(size ...
                                   (boxes{n_neigh{nn,oo}(pp)},1),2), ...
                                   boxes{nn,oo}(qq,1:2))-boxes{n_neigh ...
                                   {nn,oo}(pp)}(:,1:2)).^2,2));
                       pair_used = boxes{n_neigh{nn,oo}(pp)}(:,3)./ ...
                                   pair_dist;
                       pair_used(isinf(pair_used)) = 0;
                       phi_NN{nn,oo}(qq,1) = phi_NN{nn,oo}(qq,1)+ ...
                                             sum(pair_used,1);  
                end
            end        
    end
end
% The cell phi_NN contains the field induced by all sources within the
% near neighbors of box {kk,ll} containing the specific target {qq}.
cat_phi=cat(3,phi_INT,phi_NN);
phi_FMM=cell(L,L);
for rr=1:L
    for ss=1:L
        phi_FMM{rr,ss}=cat_phi{rr,ss,1}+cat_phi{rr,ss,2};
    end
end
% The cell phi_FMM is the total field induced by all sources at the
% specific target {qq}. It is the sum of phi_INT and phi_NN.
toc
%% Computation via direct sum
disp(' ')
display('Using direct summation now...');
tic
phi_DIR = cell(L,L);
for tt = 1:L
    for uu = 1:L
        n_part2 = size(boxes{tt,uu},1);
        phi_DIR{tt,uu} = zeros(n_part2,1);
        for vv = 1:L*L
            pair_dist2 = zeros(size(boxes{vv},1),1);
            for ww = 1:n_part2
                pair_dist2 = sqrt(sum((bsxfun(@times,ones(size(boxes{vv} ...
                             ,1),2),boxes{tt,uu}(ww,1:2))-boxes{vv} ...
                             (:,1:2)).^2,2));
                pair_used2 = boxes{vv}(:,3)./pair_dist2;
                pair_used2(isinf(pair_used2)) = 0;
                phi_DIR{tt,uu}(ww,1) =  phi_DIR{tt,uu}(ww,1)+ ...
                                        sum(pair_used2,1);
            end
        end
    end
end
toc
% The cell phi_DIR is the total field induced by all sources at the
% specific target {qq} using direct summations.
%{
% The double for loop below also computes direct summations. However it 
% runs slower with respect to the one used above (for N~1,500 and greater).
tic
phi_DIR2=zeros(1,N);
for xx=1:N
   for yy=1:N
       if xx~=yy
           phi_DIR2(xx)=phi_DIR2(xx)+M(yy,3)/...
           norm([M(xx,1),M(xx,2)]-[M(yy,1),M(yy,2)]);
       end
    end
end
toc
%}
%% Percentage error difference between FMM and Direct solutions
perc_error = cell(L,L);
for zz = 1:L
    for zzz = 1:L
        perc_error{zz,zzz} = abs(2*(phi_DIR{zz,zzz}-phi_FMM{zz,zzz})./ ...
                             (phi_DIR{zz,zzz}+phi_FMM{zz,zzz})*100);
    end
end
% "perc_error" contains the percentage difference between the field
% computed with FMM and direct summations. The structure and numbering of
% this cell follows exactly the boxes cell.
%% Visualization of the generated mesh
disp(' ')
if N<20000
    display('Plotting the generated mesh...');
    figure
    axes_format = axes('YTick',y_edg,'XTick',x_edg);
    xlim(axes_format,[x_min x_max]);
    ylim(axes_format,[x_min x_max]);
    grid(axes_format,'on');
    hold(axes_format,'all');
    str=sprintf('Unit box containing %d sources and the generated mesh', N);
    title(str)
    for aaa=1:N
        plot(m(aaa,1),m(aaa,2),'or')
    end
    hold on
    for bbb=1:length(xy_cen)
        plot(xy_cen(bbb,1),xy_cen(bbb,2),'+b')
    end
else prompt='Do you want to skip the plot? y/n [y]: ';
    if input(prompt,'s')=='n'
        figure
        axes_format=axes('YTick',y_edg,'XTick',x_edg);
        xlim(axes_format,[x_min x_max]);
        ylim(axes_format,[x_min x_max]);
        grid(axes_format,'on');
        hold(axes_format,'all');
        str=sprintf('Unit box containing %d sources and the generated mesh' ...
                    , N);
        title(str)
        for aaa=1:N
           plot(m(aaa,1),m(aaa,2),'or')
        end
        hold on
        for bbb=1:length(xy_cen)
             plot(xy_cen(bbb,1),xy_cen(bbb,2),'+b')
        end
    end
end    
disp(' ')
display('Done.');
disp(' ')
