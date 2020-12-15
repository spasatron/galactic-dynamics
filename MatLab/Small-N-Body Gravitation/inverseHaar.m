function output = inverseHaar(x, depth)
    output = x;
    for i = 1:depth
        output = inverseHaarTransform(output, i);
    end
end






function output = inverseHaarTransform(x, depth)
    output = x;
    A = [1/sqrt(2), 1/sqrt(2); 1/sqrt(2), -1/sqrt(2)];
    s = output;
    s(1:2:2^(depth)) = [output(1:2^(depth-1))];
    s(2:2:2^(depth)) = [output(2^(depth-1)+1:2^depth)];
    for j = 1:2:2^(depth)
        output(j:j+1) = [A.'*s(j:j+1)]; 
    end
    
end