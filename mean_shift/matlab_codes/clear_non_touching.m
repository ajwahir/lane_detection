function [cleared_vess] = clear_non_touching(im,HEIGHT,WIDTH)
limit = 150;

LabeledThresholdedImage = bwlabel(im);
ssMaj  = regionprops(LabeledThresholdedImage, 'MajorAxisLength');
MajorAxisLengths = cat(1, ssMaj.MajorAxisLength);
% MinorAxisLengths = cat(1, ssMaj.MinorAxisLength);

ShortBlobsIndx = find(MajorAxisLengths < limit);

% Remove all the short  blobs
LabeledThresholdedImage = LabeledThresholdedImage(:);
for mm = 1: length(ShortBlobsIndx)
    LabeledThresholdedImage(find(LabeledThresholdedImage == ShortBlobsIndx(mm))) = 0;
end
cleared_vess = reshape(LabeledThresholdedImage,HEIGHT,WIDTH);