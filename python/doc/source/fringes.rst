The Fringes application
***********************

This is an application written in **Python** for fringe pattern
analysis. A fringe pattern is a two-dimensional (2D) image that
modulates phase information. Its basic model is the following:

.. math::

   I(x,y) = a(x,y) + b(x,y)\cos(\phi(x,y))

where :math:`a(x,y)` is the background illumination, :math:`b(x,y)`
the constrast or modulation term and :math:`\phi(x,y)` is the
modulating phase.

Here we present the documentation of modules that conform the
**Fringes** application.

.. automodule:: fringes
.. autofunction:: fringes.main
.. automodule:: gui
.. automodule:: gui.mainwindow
.. autoclass:: MainWindow
.. automethod:: MainWindow.__init__
.. automethod:: MainWindow.connectSignals
