 function [vess] = MatchFilterWithGaussDerivative(k,num,im,sigmaForMF,sigmaForGD,yLengthForMF,yLengthForGD,...
    tForMatchfilterRes,tForGaussDerRes, numOfDirections,mask,maskForGDRange,c_value,t)
%
%    Retinal vessel extraction by matched filter with 
%    first-order derivative of Gaussian
%  
%               Inputs:
%                     k                  -   ignore, used for testing purposes only
%                     num                -   image number
%                     im                 -   input image
%                     sigmaForMF         -   scale value of MF
%                     sigmaForGD         -   scale value of FDOG
%                     yLengthForMF       -   length of neighborhood along y-axis of MF 
%                     yLengthForGD       -   length of neighborhood along y-axis of FDOG 
%                     tForMatchfilterRes -   threshold value of MF
%                     tForGaussDerRes    -   threshold value of FDOG
%                     numOfDirections    -   number of orientations
%                     mask               -   image mask
%                     maskForGDRange     -   another image mask
%                     c_value            -   c value
%                     t                  -   threshold value of MF-FDOG
% 
%               Output:
%                     vess     -   vessels extracted

% AUTHOR    Bob Zhang <yibo@pami.uwaterloo.ca>

if isa(im,'double')~=1 
    im = double(im);
end

[rows,cols] = size(im);

MatchFilterRes(rows,cols,numOfDirections) = 0;
GaussDerivativeRes(rows,cols,numOfDirections) = 0;

K2=tForMatchfilterRes;
S2=ones(K2,K2)/(K2^2);
for i = 0:numOfDirections-1
    matchFilterKernel = MatchFilterAndGaussDerKernel(sigmaForMF,yLengthForMF, pi/numOfDirections*i,0);
    gaussDerivativeFilterKernel = MatchFilterAndGaussDerKernel(sigmaForGD,yLengthForGD, pi/numOfDirections*i,1);
    MatchFilterRes(:,:,i+1) = conv2(im,matchFilterKernel,'same');
    RDF = conv2(im,gaussDerivativeFilterKernel,'same');
    GaussDerivativeRes(:,:,i+1) = abs(conv2(RDF,S2,'same'));
end

[maxMatchFilterRes] = max(MatchFilterRes,[],3);
accoGaussDerivativeRes = min(GaussDerivativeRes,[],3);

averageGD = conv2(accoGaussDerivativeRes,S2,'same');
meanGD = mean(mean(averageGD));
normalGD = averageGD / meanGD;
big = normalGD >= 5;
small = normalGD < 5;
% 
normalGD = normalGD .*small + big*5;
% 
% W = 2 + exp(0.01*normalGD);
%W = 2 + averageGD / 2;

W = c_value + normalGD / 2;
W = W .* ~maskForGDRange;
W = W + maskForGDRange * c_value;

K1 = tForGaussDerRes;
S1=ones(K1,K1)/(K1^2);
averageMF = conv2(maxMatchFilterRes,S1,'same');

vess = (maxMatchFilterRes - W.*averageMF)>0;

% averageGD = conv2(accoGaussDerivativeRes,S2,'same');
% meanGD = mean(mean(averageGD));
% normalGD = averageGD / median(median(averageGD));
% big = normalGD > 10;
% small = normalGD < 10;
% 
% normalGD = normalGD .*small + big*2;
% 
% W = 40 + exp(1.5*normalGD);
% %K1 = 81;
% %S1=ones(K1,K1)/(K1^2);
% %averageMF = conv2(maxMatchFilterRes,S1,'same');
% vess = maxMatchFilterRes > W;

vess = vess & mask;
se = strel('square',3);
vess = imclose(vess,se);
vess = bwmorph(vess,'close');
[L num] = bwlabel(vess, 8);
prop = regionprops(L, 'Area');
idx = find([prop.Area] > t);
vess = ismember(L,idx);
