function cleared_vessels=getvessels(im,mask,image_name)


Z = zeros(20,3);

[HEIGHT WIDTH CHANNEL] = size(im);
maskForGDRange = zeros(HEIGHT,WIDTH);

for row = 1:HEIGHT
    for col = 1:WIDTH
        if (row-302)*(row-302) + (col-350)*(col-350) > 62500
            maskForGDRange(row,col) = 1;
        end
    end
end

mask = logical(mask);
se = strel('disk',2);
mask=imerode(mask,se);
% im = double(im(:,:,2)); %for vessels
% im = double(im(:,:,3)); %for 3d
im=double(im);

[vess1] = MatchFilterWithGaussDerivative([],1,im,1.5,1.5,9,5,41,201,8,mask,maskForGDRange,3,40);

cleared_vessels=clear_non_touching(vess1,HEIGHT,WIDTH);
% cleared_vessels=imdilate(cleared_vessels,se);
% figure;imshow(cleared_vessels);
% % imwrite(cleared_vessels,['G:\DR_Paper\codes\vessels\' 'vess_' image_name '.png']);

end

