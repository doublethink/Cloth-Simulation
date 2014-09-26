Cloth-Simulation
================

Using Explict Euler Integration

This is a cloth simulation project I created for COMP308 at Victoria University.
It uses Explicit Euler Intergration which is an old method (see: computationally expensive) for cloth sim but I really like how fluid it looks and how you can adjust the metrics of each cloth to have different properties (i.e. stiff or otherwise). Play with the constructors for each new Cloth() to see what I mean. Adjusting the number of nodes for each cloth will also give it different properties.

Uses Blinn-Phong shading. 

Dependencies:
Linux
lGL -lglut -lGLU -ljpeg -lpng12 -lm -lGLEW --all nessisary to compile

Controls:
Fullscreen - 'f'
Camera controls - 'up-arrow','down-arrow','left-arrow','right-arrow'
Zoom - 'home','end'

New cloth - 'n'
Start/stop wind - 'm'
More wind - ','
Less wind = '.'

Old spotlight controls: 'w''s''a''d'

To Do:
-Multithreaded per cloth
-GPU node position calculation