========================================================================
  GilmanTunnel project application.
========================================================================
////////////////////////////////////////////////////////////////////////
General Installation tips:
(1) Download Visual Studio 2015 and pull the project from
https://github.com/mattkae/GilmanTunnel.git
using Visual Studio's built-in Git commands.
(2) Ensure that you have OpenGL, OpenCV, and the Kinect 1.0 SDK
downloaded onto your computer, and ensure that they are in the
proper file paths (Project -> Properties -> Configuration -> Linker -> etc...)
(3) After linking all of the libraries correctly, you can build a Win32
application in Release Mode by clicking the play button at the top of your
Visual Studio application.

////////////////////////////////////////////////////////////////////////
Usage:
With your Kinect plugged into your computer, everything should load. If it
fails to load, check the command prompt to debug the issue. Afterward,
you can switch in between Application modes using keys 1 through 4.
1 = RGB Stream
2 = Depth Stream
3 = Art Gallery
4 = Particle Highway

////////////////////////////////////////////////////////////////////////
If you have any issues, feel free to make alterations to the code and
suggest them on Git!
