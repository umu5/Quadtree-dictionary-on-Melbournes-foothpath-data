# Quadtree-dictionary-on-Melbournes-foothpath-data

Supporting Point Region Queries: for this satge value is always 3
An example execution of the program might be:

#make -B dict3
#./dict3 3 dataset_2.csv output.txt 144.969 -37.7975 144.971 -37.7955

(/dict3 stage datafile outputfile start_longitude start_latitude end_longitude end_latitude) -> explains above command

Program read data from the data file specified in the command line argument above. Dict3 used Dict1 and Dict2 to read and store the data.
Construct a quadtree from the stored data.
Accept co-ordinate pairs from stdin, search the constructed quadtree for the point region containing the co-ordinate pair and print all matching records to the output file.
In addition to outputting the record(s) to the output file, the list of quadrant directions followed from the root until the correct point region is found gets output to stdout.
This approach inserts each footpath's (start_lon, start_lat) and (end_lon, end_lat) pairs into the quadtree, allowing the footpath to be found from its start or end point.
Where multiple footpaths are present in the found point region, footpaths are printed in order of footpath_id.

-----------------------------------------------------------------------------------------------------------------------------------------------------------

Supporting Range Queries: For this stage value is always 4
quadtree supports range queries given by (x, y) co-ordinate pairs.
An example execution of the program might be:

#make -B dict4
#./dict4 4 dataset_2.csv output.txt 144.968 -37.797 144.977 -37.79

(./dict4 stage datafile outputfile start_longitude start_latitude end_longitude end_latitude) -> explains above command

Just as stage 3 above, we read in the dataset, store it in a dictionary and construct a quadtree on the stored data.
For Stage 4, we  accept sets of pairs of co-ordinate long double type values from stdin, and efficiently use the quadtree to find all footpaths which are within the bounds of the query.
Output to stdout should include all directions searched in order, with each branch potentially containing points within the query bounds fully explored before proceeding to the next possible branch. Where multiple directions are possible, quadrants must be searched in the order SW , NW , NE, SE .
Similar to Stage 3, where multiple footpaths are returned by the range query, these should be sorted by footpath_id. Output each footpath only once for each query, even if both its "start" and "end" points both occur in the searched region
