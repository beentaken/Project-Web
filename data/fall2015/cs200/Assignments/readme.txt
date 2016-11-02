* Place 'glut32.dll' in:
      C:\WINDOWS\system32

* Place 'glut32.lib' in:
      C:\Program Files\Microsoft Visual Studio 12.0\VC\lib

* Place 'glut.h' in:
      C:\Program Files\Microsoft Visual Studio 12.0\VC\include\GL


If you are working on a DigiPen machine (and don't have administrative
privileges), you can do the following.  In the folder that you are
working in:
* Create a folder named GL
* Place a copy of 'glut.h' into the GL folder
* Keep a copy of 'glut32.lib' and 'glut32.dll' in the current folder
* Compile your code from the Visual Studio Command Prompt using
      cl /EHsc /I. <file(s)>
      cd \Users\tardis\Desktop\CS200\Assignments

