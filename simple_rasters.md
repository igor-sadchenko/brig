**simple\_rasters** is a way of storing tilemaps with the pyramids in any database that supports geometric data. It adds a single table with metadata:

<table border='1'><tr><td align='center'><b>column</b>
</td><td align='center'><b>type</b>
</td><td align='center'><b>notes</b>
</td></tr>

<tr><td> schema<br>
</td><td> text<br>
</td><td> together define the column containing images<br>
</td></tr>

<tr><td> table<br>
</td><td> text<br>
</td></tr>

<tr><td> raster<br>
</td><td> text<br>
</td></tr>

<tr><td> base_schema<br>
</td><td> text<br>
</td><td> together define the base raster coverage<br>
</td></tr>

<tr><td> base_table<br>
</td><td> text<br>
</td></tr>

<tr><td> base_raster<br>
</td><td> text<br>
</td></tr>

<tr><td> geometry<br>
</td><td> text<br>
</td><td> the column containing the bounding boxes<br>
</td></tr>

<tr><td> resolution_x<br>
</td><td> double<br>
</td><td> size of a pixel in geounits<br>
</td></tr>

<tr><td> resolution_y<br>
</td><td> double<br>
</td></tr>

</table>