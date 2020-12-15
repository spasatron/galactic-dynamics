function output = haar(x, depth)
    output = x;
    for i = depth:-1:1
        output = haarTransform(output, i);
    end
end

function output = haarTransform(x, depth)
    output = x;
     A = [1/sqrt(2), 1/sqrt(2); 1/sqrt(2), -1/sqrt(2)];
    for j = 1:2:(2^(depth)-1)
       output(j:j+1, 1) = [A*x(j:j+1, 1)]; 
    end
    
    output(1:2^depth) = [output(1:2:2^(depth)); output(2:2:2^(depth))]
    
end

