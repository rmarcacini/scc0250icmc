# Define a simple Gaussian surface
# sphinx_gallery_thumbnail_number = 2
%matplotlib inline
from pyvista import set_plot_theme
set_plot_theme('document')
import pyvista as pv
import numpy as np
n = 20
x = np.linspace(-200, 200, num=n) + np.random.uniform(-5, 5, size=n)
y = np.linspace(-200, 200, num=n) + np.random.uniform(-5, 5, size=n)
xx, yy = np.meshgrid(x, y)
A, b = 100, 100
zz = A * np.exp(-0.5 * ((xx / b) ** 2.0 + (yy / b) ** 2.0))

# Get the points as a 2D NumPy array (N by 3)
points = np.c_[xx.reshape(-1), yy.reshape(-1), zz.reshape(-1)]
points[0:5, :]
cloud = pv.PolyData(points)
cloud.plot(point_size=15)
surf = cloud.delaunay_2d()
surf.plot(show_edges=True)
